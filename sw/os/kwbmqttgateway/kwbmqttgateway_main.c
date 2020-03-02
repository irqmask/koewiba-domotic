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
/*
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
 */

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mosquitto.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
#include <safe_lib.h>
#endif

// include
#include "kwb_defines.h"
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

/**
 * Storage type selected options for this application.
 */
typedef struct options {
    char        router_address[256];    //!< address of tcp- or unix socket server.
    uint16_t    router_port;            //!< port of tcp socket server or 0 for
                                        //!< unix socket server.
    bool        router_address_set;     //!< flag if router address is set and valid.
} options_t;

// --- Local variables ---------------------------------------------------------

app_handles_t       g_handles;          //!< stores globally used handles in 
                                        //!< this application.
msg_socket_t        g_kwb_socket;       //!< handle to established socket connection.
msg_endpoint_t*     g_kwb_socket_ep;    //!< handle of endpoint of established socket connection.
bool                g_end_application = false;  //!< flag, if mainloop shall be exited.

// --- Module global variables -------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Set kwbmqttgateway run options via function call.
 * Function can be used to set default parameters.
 *
 * @param[out]  options         Stucture where options are stored in.
 * @param[in]   router_address  Address of kwbrouter server.
 * @param[in]   router_port     Port number of kwbrouter server.
 */
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

/**
 * Parse the command line options and save results in options.
 *
 * @param[in]   argc    Count of command line options.
 * @param[in]   argv    Command line arguments.
 * @param[out]  options Stucture where options are stored in.
 *
 * @returns true, if command line options have been parsed successfully or no
 *          options have been read (in this case, default parameters will be
 *          used).
 */
static bool parse_commandline_options (int argc, char* argv[], options_t* options)
{
    bool                    rc = true;
    int                     c;

    while (1) {
        c = getopt(argc, argv, "a:p");
        if (c == -1) break;

        switch (c) {
        case 'a':
            log_msg(KWB_LOG_INFO, "router address %s", optarg);
            strcpy_s(options->router_address, sizeof(options->router_address), optarg);
            options->router_address_set = true;
            break;
        case 'p':
            log_msg(KWB_LOG_INFO, "router port %s", optarg);
            options->router_port = atoi(optarg);
            break;

        default:
            rc = false;
            break;
        }
    }
    return rc;
}

/**
 * Check for missing and given arguments which are mutual exclusive and validate the values.
 *
 * @param[in] options   Structure of options to check.
 * @returns true, if the options are valid.
 */
static bool validate_options(options_t* options)
{
    bool    rc = false;

    do {
        // minimum address is "/a": unix socket with name "a" in the root directory
        if (strnlen(options->router_address, sizeof(options->router_address)) < 2) {
            log_error("Missing router address!");
            break;
        }
        rc = true;
    } while (0);
    return rc;
}

/**
 * Explain briefly the command-line arguments of the application
 */
static void print_usage (void)
{
    fprintf(stderr, "\nUsage:\n");
    fprintf(stderr, "kwbmqttgateway [-a <address>] [-p <port>]\n\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, " -a <address>        Address of kwbrouter server. Default: /tmp/kwbr.usk\n");
    fprintf(stderr, " -p <port>           Port number of kwbrouter server. Default: 0\n");
}

/**
 * Writes the version of the mosquitto client library to log-file.
 */
static void log_mqtt_version(void)
{
    int major = 0, minor = 0, bugfix = 0;

    mosquitto_lib_version(&major, &minor, &bugfix);
    log_msg(LOG_STATUS, "MQTT Mosquitto client library version %d.%d.%d", major, minor, bugfix);
}

/**
 * Is called by mqtt library when a mqtt message has been received.
 */
void on_mqtt_message(struct mosquitto* mosq, void *userdata, const struct mosquitto_message *message)
{
    msg_t kwbmsg;

    if (message->payloadlen) {
        log_msg(KWB_LOG_INTERCOMM, "MQTTRECV %s %s\n", message->topic, (char*)message->payload);
        memset(&kwbmsg, 0, sizeof(kwbmsg));
        if (mqtt2msg(message->topic, message->payload, &kwbmsg) == eERR_NONE) {
            msg_s_send(g_kwb_socket_ep, &kwbmsg);
        }
    } else {
        log_msg(LOG_STATUS, "MQTT %s (null)\n", message->topic);
    }
}

/**
 * Called when this client got connected to the MQTT server.
 * @param[in]   mosq        Mosquitto handle.
 * @param[in]   userdata    Pointer to user defined data.
 * @param[in]   result      MQTT error: reason for disconnect.
 */
void on_mqtt_connect(struct mosquitto* mosq, void *userdata, int result)
{
    if (!result) {
        log_msg(KWB_LOG_STATUS, "MQTT connected. Result %d", result);
        /* Subscribe to broker information topics on successful connect. */
        mqtt2msg_subscribe(mosq);
    } else {
        log_error("MQTT connection failed! Result %d", result);
    }
}

/**
 * Called when this client gets disconnected from the MQTT server.
 * @param[in]   mosq        Mosquitto handle.
 * @param[in]   userdata    Pointer to user defined data.
 * @param[in]   result      MQTT error: reason for disconnect.
 */
void on_mqtt_disconnect(struct mosquitto* mosq, void *userdata, int result)
{
    (void)mosq;
    (void)userdata;

    if (result != 0) {
        log_error("MQTT unexpected disconnected. Error code %d: %s", result,  mosquitto_strerror(result));
    } else {
        log_info("MQTT disconnected!");
    }
}

/**
 * Called on successful subscription of a topic.
 * Passed information is just logged.
 * @param[in]   mosq        Mosquitto connection handle
 * @param[in]   userdata
 * @param[in]   mid
 * @param[in]   qos_count
 * @param[in]   granted_qos
 */
void on_mqtt_topic_subscribed(struct mosquitto* mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    int i;

    log_msg(KWB_LOG_STATUS, "MQTT subscribed (mid: %d): %d", mid, granted_qos[0]);
    for(i=1; i<qos_count; i++){
        log_msg(LOG_STATUS, ", %d", granted_qos[i]);
    }
}

int mosquitto_setup()
{
    struct mosquitto* mosq = NULL;
    const char* mqtt_address = "localhost";
    const int mqtt_port = 1883;

    g_handles.mqtt_disconnected = true;
    g_handles.mosq = NULL;

    mosq = mosquitto_new("kwbmqttgateway", true, NULL);
    if (mosq == NULL) {
        log_error("MQTT Out of memory while opening mosquitto handle.");
        return eERR_RESOURCE;
    }

    mosquitto_connect_callback_set(mosq, on_mqtt_connect);
    mosquitto_message_callback_set(mosq, on_mqtt_message);
    mosquitto_subscribe_callback_set(mosq, on_mqtt_topic_subscribed);

    if (mosquitto_connect(mosq, mqtt_address, mqtt_port, 60)) {
        mosquitto_destroy(mosq);
        mosq = NULL;
        log_error("MQTT Unable to connect to mosquitto server %s:%d", mqtt_address, mqtt_port);
        return eERR_SYSTEM;
    }
    g_handles.mosq = mosq;
    g_handles.mqtt_disconnected = false;
    log_msg(KWB_LOG_INFO, "MQTT mosquitto_setup() connecting to ioloop");
    return mosquitto_connect_to_ioloop(&g_handles);
}

/**
 * Callback for incoming messages from kwbrouter.
 * Converts message to MQTT and forwards it to the MQTT broker.
 *
 * @param[in]   message     Received message.
 * @param[in]   reference   Optional reference, registered with this callback.
 * @param[in]   arg         Optional additional argument registered with this callback.
 */
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
        log_error("MQTTSEND mosquitto_publish() failed with errorcode %d, topic=%s message=%s", mrc, topic, msgtext);
        return;
    }
    log_msg(KWB_LOG_INTERCOMM, "MQTTSEND %s %s", topic, msgtext);
    // send message when fd is ready to accept data
    mosquitto_ioloop_suspend_write(&g_handles);
}

/**
 * This function is called when the connection to the kwbrouter is closed.
 * @param[in]   address     Address of connetion which was closed.
 * @param[in]   port        Portnumber of connection which was closed.
 * @param[in]   reference   Optional reference, registered with this callback.
 * @param[in]   arg         Optional additional argument registered with this callback.
 */
void on_kwb_close_connection(const char* address, uint16_t port, void* reference, void* arg)
{
    log_msg(KWB_LOG_STATUS, "MQTT connection closed. address=%s port=%d", address, port);
    g_end_application = true;
}

/**
 * Setup socket connection to kwbrouter.
 *
 * @param ioloop[in]    ioloop to connect to.
 * @param options[in]   Pointer to global options structure.
 *
 * @return 0 if successful, otherwise errorcode.
 */
int kwb_socket_setup(ioloop_t* ioloop, options_t* options)
{
    int retval = eERR_NONE;

    do {
        msg_s_init(&g_kwb_socket);
        msg_s_set_incomming_handler(&g_kwb_socket, on_kwb_incomming_message, NULL);
        if ((retval = msg_s_open_client(&g_kwb_socket, ioloop, "/tmp/kwbr.usk", 0)) != eERR_NONE) {
            break;
        }
        g_kwb_socket_ep = msg_s_get_endpoint(&g_kwb_socket, 0);
        msg_s_set_closeconnection_handler(g_kwb_socket_ep, on_kwb_close_connection, NULL);
    } while (0);

    return retval;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Main entry point of kwbmqttgateway.
 *
 * @param[in]   argc    Number of arguments
 * @param[in]   argv    List of arguments
 *
 * @return 0 if application ended without error.
 */
int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;
    options_t       options;
    ioloop_t        mainloop;

    do {
        // all logs on
        log_set_mask(0xFFFFFFFF & ~LOG_VERBOSE2);
        log_msg(KWB_LOG_INFO, "kwbmqttgateway...");

        g_handles.ioloop = &mainloop;
        // set default options for kwbmqttgateway
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
        log_mqtt_version();

        ioloop_init(g_handles.ioloop);

        mosquitto_lib_init();
        rc = mosquitto_setup();
        if (rc != eERR_NONE) break;

        rc = kwb_socket_setup(g_handles.ioloop, &options);
        if (rc != eERR_NONE) break;

        log_msg(KWB_LOG_STATUS, "entering mainloop...");
        while (!g_end_application) {
            ioloop_run_once(g_handles.ioloop);
            // issue a re-connect to mqtt server
            if (g_handles.mosq != NULL && g_handles.mqtt_disconnected) {
                g_handles.mqtt_disconnected = false;
                mosquitto_reconnect(g_handles.mosq);
            }
        }
        mosquitto_destroy(g_handles.mosq);
        mosquitto_lib_cleanup();
    } while (0);
    return rc;
}

/** @} */
