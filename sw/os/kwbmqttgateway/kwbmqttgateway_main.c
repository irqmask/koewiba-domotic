/*
 * kwbkouter - A router for koewiba-domotic messages.
 * Copyright (C) 2017  christian <irqmask@gmx.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
/**
 * @addtogroup KWBMQTTGATEWAY
 *
 * @{
 * @file    kwbmqttgateway_main.c
 * @brief   Gateway to convert koewiba message to mqtt and vice versa.
 * kwbmqttgateway routes commands comming from a kwbrouter to an mqtt broker.
 * Messages from another MQTT client are aubscribed for and transmitted to a
 * kwbrouter for distribution.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "safe_lib.h"
#include "mosquitto.h"

// include
#include "prjtypes.h"

// os/libsystem
#include "sysgetopt.h"

// os/shared
#include "ioloop.h"
#include "log.h"
#include "message.h"
#include "message_socket.h"

#include "kwbmqttgateway.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct options {
    char        serial_device[256];
    int         serial_baudrate;
    char        router_address[256];
    uint16_t    router_port;
    bool        serial_device_set;
    bool        router_address_set;

} options_t;

// --- Local variables ---------------------------------------------------------

app_handles_t       g_handles;
msg_socket_t        g_kwb_socket;
msg_endpoint_t*     g_kwb_socket_ep;
bool                g_end_application = false;

// --- Module global variables -------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void set_options (options_t*     options,
                         const char*    router_address,
                         uint16_t       router_port)
{
    memset(options, 0, sizeof(options_t));

    if (router_address != NULL) {
        strcpy_s(options->router_address, sizeof(options->router_address), router_address);
    }
    options->router_port = router_port;
}

static bool parse_commandline_options (int argc, char* argv[], options_t* options)
{
    bool                    rc = true;
    int                     c;

    while (1) {
        c = getopt(argc, argv, "a:p");
        if (c == -1) break;

        switch (c) {
        case 'a':
            printf("router address %s\n", optarg);
            strcpy_s(options->router_address, sizeof(options->router_address), optarg);
            options->router_address_set = true;
            break;
        case 'p':
            printf("router port %s\n", optarg);
            options->router_port = atoi(optarg);
            break;

        default:
            rc = false;
            break;
        }
    }
    return rc;
}

static bool validate_options(options_t* options)
{
    bool    rc = false;

    do {
        // minimum address is "/a": unix socket with name "a" in the root directory
        if (strnlen(options->router_address, sizeof(options->router_address)) < 2) {
            fprintf(stderr, "Missing router address!\n");
            break;
        }
        rc = true;
    } while (0);
    return rc;
}

static void print_usage (void)
{
    printf("\nUsage:\n");
    printf("kwbmqttgateway [-a <address>] [-p <port>]\n\n");
    printf("Arguments:\n");
    printf(" -a <address>        Address of kwbrouter server. Default: /tmp/kwbr.usk\n");
    printf(" -p <port>           Port number of kwbrouter server. Default: 0\n");
}

static void log_mqtt_version(void)
{
    int major = 0, minor = 0, bugfix = 0;

    mosquitto_lib_version(&major, &minor, &bugfix);
    log_msg(LOG_STATUS, "Mosquitto client library version %d.%d.%d", major, minor, bugfix);
}

void on_mqtt_message(struct mosquitto* mosq, void *userdata, const struct mosquitto_message *message)
{
    msg_t kwbmsg;

    if (message->payloadlen) {
        log_msg(LOG_STATUS, "MQTT %s %s\n", message->topic, message->payload);
        memset(&kwbmsg, 0, sizeof(kwbmsg));
        if (mqtt2msg(message->topic, message->payload, &kwbmsg) == eERR_NONE) {
            msg_s_send(g_kwb_socket_ep, &kwbmsg);
        }
    } else {
        log_msg(LOG_STATUS, "MQTT %s (null)\n", message->topic);
    }
}

void on_mqtt_connect(struct mosquitto* mosq, void *userdata, int result)
{
    if (!result) {
        log_msg(LOG_STATUS, "MQTT connected. Result %d", result);
        /* Subscribe to broker information topics on successful connect. */
        mqtt2msg_subscribe(mosq);
    } else {
        log_msg(LOG_ERROR, "MQTT connection failed! Result %d", result);
    }
}

void my_subscribe_callback(struct mosquitto* mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    int i;

    log_msg(LOG_STATUS, "MQTT subscribed (mid: %d): %d", mid, granted_qos[0]);
    for(i=1; i<qos_count; i++){
        log_msg(LOG_STATUS, ", %d", granted_qos[i]);
    }
}

int mosquitto_setup()
{
    uint16_t mid = 0;

    g_handles.mosq = mosquitto_new("kwbmqttgateway", true, NULL);
    if (!g_handles.mosq) {
        fprintf(stderr, "Error: Out of memory.\n");
        return eERR_MALLOC;
    }

    mosquitto_connect_callback_set(g_handles.mosq, on_mqtt_connect);
    mosquitto_message_callback_set(g_handles.mosq, on_mqtt_message);
    mosquitto_subscribe_callback_set(g_handles.mosq, my_subscribe_callback);

    if (mosquitto_connect(g_handles.mosq, "localhost", 1883, 60)){
        fprintf(stderr, "Unable to connect.\n");
        return eERR_SYSTEM;
    }

    return mosquitto_connect_to_ioloop(&g_handles);
}

void on_kwb_incomming_message(msg_t* message, void* reference, void* arg)
{
    char topic[256], msgtext[256];
    int mid = 0;
    int mrc;

    if (msg2mqtt(message, topic, sizeof(topic), msgtext, sizeof(msgtext)) != eERR_NONE) {
        return;
    }
    mrc = mosquitto_publish(g_handles.mosq, &mid, topic, strnlen(msgtext, sizeof(msgtext)), msgtext, 1, true);
    if (mrc != MOSQ_ERR_SUCCESS) {
        log_error("mosquitto_publish() failed with errorcode %d", mrc);
        return;
    }
    // send message when fd is ready to accept data
    mosquitto_ioloop_suspend_write(&g_handles);
}

void on_kwb_close_connection(const char* address, uint16_t port, void* reference, void* arg)
{
    g_end_application = true;
}

int kwb_socket_setup(ioloop_t* ioloop, options_t* options)
{
    int retval = eERR_NONE;
    msg_endpoint_t* msg_ep;

    do {
        msg_s_init(&g_kwb_socket);
        msg_s_set_incomming_handler(&g_kwb_socket, on_kwb_incomming_message, NULL);
        if ((retval = msg_s_open_client(&g_kwb_socket, ioloop, "/tmp/kwbr.usk", 0)) != eERR_NONE) {
            break;
        }
        g_kwb_socket_ep = msg_s_get_endpoint(&g_kwb_socket, 0, 0);
        msg_s_set_closeconnection_handler(g_kwb_socket_ep, on_kwb_close_connection, NULL);
    } while (0);

    return retval;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             mid, rc = eERR_NONE;
    options_t       options;
    ioloop_t        mainloop;

    do {
        printf("\nkwbmqttgateway...\n");

        g_handles.ioloop = &mainloop;
        // set default options for nkwbmqttgateway
        set_options(&options,
                    "/tmp/kwbr.usk",    // default address of nkwbmqttgateway socket
                    0);                 // port 0: use unix sockets

        // parse and validate commandline options
        if (parse_commandline_options(argc, argv, &options) == false ||
            validate_options(&options) == false) {
            print_usage();
            rc = eERR_BAD_PARAMETER;
            break;
        }
        // all logs on
        log_set_mask(0xFFFFFFFF);
        log_mqtt_version();

        ioloop_init(g_handles.ioloop);

        mosquitto_lib_init();
        rc = mosquitto_setup();
        if (rc != eERR_NONE) break;

        rc = kwb_socket_setup(g_handles.ioloop, &options);
        if (rc != eERR_NONE) break;

        printf("entering mainloop...\n");
        while (!g_end_application) {
            ioloop_run_once(g_handles.ioloop);
        }
        mosquitto_destroy(g_handles.mosq);
        mosquitto_lib_cleanup();
    } while (0);
    return rc;
}

/** @} */
