/**
 * @addtogroup KWBMQTTGATEWAY
 *
 * @{
 * @file    kwbmqttgateway_main.cpp
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

#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>

#include <mosquitto.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
extern "C" {
#include <safe_lib.h>
}
#endif

// include
#include "kwb_defines.h"
#include "prjtypes.h"

// shared
#include "crc16.h"

// os/include
#include "version.h"

// os/libkwb
#include "connection.h"
#include "connection_socket.h"
#include "exceptions.h"
#include "ioloop.h"
#include "log.h"
#include "message.h"

// os/libsystem
#include "sysgetopt.h"

#include "kwbmqttgateway.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Storage type selected options for this application.
 */
typedef struct options {
    //! Device of serial connection to RS232 gateway.
    char        serial_device[256];
    //! Baudrate of serial connection to RS232 gateway.
    int         serial_baudrate;
    //! Flag: if set, serial device has been configured in the command line options.
    bool        serial_device_set;
    //! address of tcp- or unix socket server.
    char        router_address[256];
    //! port of tcp socket server or 0 for unix socket server.
    uint16_t    router_port;
    //! flag if router address is set and valid.
    bool        router_address_set;
} options_t;

// --- Local variables ---------------------------------------------------------

//! stores globally used handles in this application.
app_handles_t       g_handles;
//! flag, if mainloop shall be exited.
bool                g_end_application = false;
//! currently hold connection to kwbrouter or serial gateway
std::shared_ptr<Connection> g_conn;
//! List of sent messages. Message will be removed if MQTT server echoed it back
std::vector<uint16_t> g_list_sent;

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
static void set_options(options_t     *options,
                        const char    *router_address,
                        uint16_t       router_port)
{
    memset(options, 0, sizeof(options_t));

    if (router_address != NULL) {
        strcpy_s(options->router_address, sizeof(options->router_address), router_address);
        options->router_address_set = true;
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
static bool parse_commandline_options(int argc, char *argv[], options_t *options)
{
    bool                    rc = true;
    int                     c;

    while (1) {
        c = getopt(argc, argv, "a:p");
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'a':
            log_msg(LOG_INFO, "router address %s", optarg);
            strcpy_s(options->router_address, sizeof(options->router_address), optarg);
            options->router_address_set = true;
            break;
        case 'p':
            log_msg(LOG_INFO, "router port %s", optarg);
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
static bool validate_options(options_t *options)
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
static void print_usage(void)
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
    log_msg(LOG_INFO, "MQTT Mosquitto client library version %d.%d.%d", major, minor, bugfix);
}

/**
 * Calculate checksum of mqtt topic and payload into single checksum@brief calc_mqtt_topic_and_payload_crc16
 * @param[in] message   MQTT message structure containing topic and payload.
 * @returns crc16 of topic and message
 */
uint16_t calc_mqtt_topic_and_payload_crc16(const char* topic, size_t topic_len, const char* payload, size_t payload_len)
{
    uint16_t crc = 0;

    for (size_t i=0; i<topic_len; i++) {
        crc = crc_16_next_byte(crc, topic[i]);
    }

    for (size_t i=0; i<payload_len; i++) {
        crc = crc_16_next_byte(crc, payload[i]);
    }
    return crc;
}

/**
 * Check if the message has just been sent.
 * @param message
 * @returns true, if message has been sent.
 * @note Removes message from sent messages list.
 */
bool check_for_echoes(uint16_t message_crc)
{
    bool rc = false;

    // check if crc is in list
    std::vector<uint16_t>::iterator found;
    found = std::find(g_list_sent.begin(), g_list_sent.end(), message_crc);
    if (found != g_list_sent.end()) {
        g_list_sent.erase(found);
        rc = true;
    }

    // limit length of list
    while (g_list_sent.size() > 100) {
        g_list_sent.erase(g_list_sent.begin());
    }
    return rc;
}

/**
 * Is called by mqtt library when a mqtt message has been received.
 *
 * @param[in]   mosq        (unused)Handle of mosquitto connection
 * @param[in]   userdata    (unused)
 * @param[in]   message     Received MQTT topic and MQTT message
 */
void on_mqtt_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    msg_t kwbmsg;

    if (message->payloadlen) {
        uint16_t crc = calc_mqtt_topic_and_payload_crc16(message->topic, strlen(message->topic), (char*)message->payload, message->payloadlen);
        if (!check_for_echoes(crc)) {
            log_msg(KWB_LOG_INTERCOMM, "MQTTRECV %s %s\n", message->topic, (char *)message->payload);
            memset(&kwbmsg, 0, sizeof(kwbmsg));
            if (mqtt2msg(message->topic, (const char*)message->payload, &kwbmsg) == eERR_NONE) {
                g_conn->send(kwbmsg);
            }
        } else {
            log_msg(KWB_LOG_INTERCOMM, "MQTTRECV %s %s (ECHOED)\n", message->topic, (char *)message->payload);
        }
    }
    else {
        log_msg(LOG_INFO, "MQTT %s (null)\n", message->topic);
    }
}

/**
 * Called when this client got connected to the MQTT server.
 *
 * @param[in]   mosq        Mosquitto handle.
 * @param[in]   userdata    Pointer to user defined data.
 * @param[in]   result      MQTT error: reason for disconnect.
 */
void on_mqtt_connect(struct mosquitto *mosq, void *userdata, int result)
{
    if (!result) {
        log_msg(LOG_STATUS, "MQTT connected. Result %d", result);
        /* Subscribe to broker information topics on successful connect. */
        mqtt2msg_subscribe(mosq);
    }
    else {
        log_error("MQTT connection failed! Result %d", result);
    }
}

/**
 * Called when this client gets disconnected from the MQTT server.
 *
 * @param[in]   mosq        Mosquitto handle.
 * @param[in]   userdata    Pointer to user defined data.
 * @param[in]   result      MQTT error: reason for disconnect.
 */
void on_mqtt_disconnect(struct mosquitto *mosq, void *userdata, int result)
{
    (void)mosq;
    (void)userdata;

    if (result != 0) {
        log_error("MQTT unexpected disconnected. Error code %d: %s", result,  mosquitto_strerror(result));
    }
    else {
        log_info("MQTT disconnected!");
    }
}

/**
 * Called on successful subscription of a topic.
 * Passed information is just logged.
 *
 * @param[in]   mosq        Mosquitto connection handle
 * @param[in]   userdata    (unused)
 * @param[in]   mid         (only logged)
 * @param[in]   qos_count   MQTT Quality of service count
 * @param[in]   granted_qos MQTT Granted qualitiy of service.
 */
void on_mqtt_topic_subscribed(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    int i;

    log_msg(LOG_STATUS, "MQTT subscribed (mid: %d): %d", mid, granted_qos[0]);
    for (i = 1; i < qos_count; i++) {
        log_msg(LOG_INFO, ", %d", granted_qos[i]);
    }
}

/**
 * Setup connection to mosquitto server.
 * @return eERR_None if successful, otherwise error code.
 */
int mosquitto_setup()
{
    struct mosquitto *mosq = NULL;
    const char *mqtt_address = "localhost";
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
    log_msg(LOG_INFO, "MQTT mosquitto_setup() connecting to ioloop");
    return mosquitto_connect_to_ioloop(&g_handles);
}

/**
 * Callback for incoming messages from kwbrouter.
 * Converts message to MQTT and forwards it to the MQTT broker.
 *
 * @param[in]   message     Received message.
 * @param[in]   reference   Optional reference, registered with this callback.
 */
void on_kwb_incoming_message(const msg_t &message, void *reference)
{
    char topic[256], msgtext[256];
    int mid = 0;
    int mrc;

    if (msg2mqtt(message, topic, sizeof(topic), msgtext, sizeof(msgtext)) != eERR_NONE) {
        return;
    }

    size_t msgtext_len = strnlen(msgtext, sizeof(msgtext));
    mrc = mosquitto_publish(g_handles.mosq, &mid, topic, msgtext_len, msgtext, 2, false);
    if (mrc != MOSQ_ERR_SUCCESS) {
        log_error("MQTTSEND mosquitto_publish() failed with errorcode %d, topic=%s message=%s", mrc, topic, msgtext);
        return;
    }

    uint16_t crc = calc_mqtt_topic_and_payload_crc16(topic, strlen(topic), msgtext, msgtext_len);
    g_list_sent.push_back(crc);
    log_msg(KWB_LOG_INTERCOMM, "MQTTSEND %s %s id %d", topic, msgtext, mid);
    // send message when fd is ready to accept data
    mosquitto_ioloop_suspend_write(&g_handles);
}

/**
 * This function is called when the connection to the kwbrouter is closed.
 *
 * @param[in]   uri         URI of connetion which was closed.
 * @param[in]   reference   Optional reference, registered with this callback.
 */
void on_kwb_close_connection(const std::string &uri, void *reference)
{
    log_msg(LOG_STATUS, "MQTT connection closed. address=%s", uri.c_str());
    g_end_application = true;
}

/**
 * Setup socket connection to kwbrouter.
 *
 * @param[in]   ioloop      ioloop to connect to.
 * @param[in]   options     Pointer to global options structure.
 *
 * @return 0 if successful, otherwise errorcode.
 */
int kwb_socket_setup(ioloop_t *ioloop, options_t *options)
{
    int retval = eERR_NONE;

    do {
        // setup connections
        try {
            if (options->router_address_set) {
                std::stringstream uriss;
                uriss << options->router_address << ":" << options->router_port;
                auto conn_socket = std::make_shared<ConnectionSocket>(ioloop, uriss.str());
                log_msg(LOG_STATUS, "Connected to router over socket interface %s", conn_socket->getName().c_str());
                g_conn = conn_socket;

                using std::placeholders::_1;
                using std::placeholders::_2;
                incom_func_t handle_incoming_message_func = std::bind(&on_kwb_incoming_message, _1, _2);
                g_conn->setIncomingHandler(handle_incoming_message_func);

                conn_func_t handle_connection_func = std::bind(&on_kwb_close_connection, _1, _2);
                g_conn->setConnectionHandler(handle_connection_func);
            }
        }
        catch (Exception &e) {
            log_error("Unable to connect! Exception %s\n", e.what());
            retval = eERR_UNSUPPORTED;
            break;
        }
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
int main(int argc, char *argv[])
{
    int             rc = eERR_NONE;
    options_t       options;
    ioloop_t        mainloop;

    do {
        // all logs on
        log_set_mask(0xFFFFFFFF & ~LOG_VERBOSE2);
        log_msg(LOG_INFO, "kwbmqttgateway...v%s", VERSION_PRODUCT);

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
        if (rc != eERR_NONE) {
            break;
        }

        rc = kwb_socket_setup(g_handles.ioloop, &options);
        if (rc != eERR_NONE) {
            break;
        }

        log_msg(LOG_STATUS, "entering mainloop...");
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
