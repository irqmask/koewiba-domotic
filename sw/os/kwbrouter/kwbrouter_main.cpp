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
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    kwbrouter_main.c
 * @brief   Router for commands received from bus or different nets.
 * kwbrouter routs commands comming from a source to one or more destinations.
 * Source and destination could be kwbrouters on a different IP or serial RS485
 * connections or local UNIX sockets for IPC.
 * Other programs eg a webserver or kwbupload a firmware uploader can connect
 * to kwbrouter to receive or send messages.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "safe_lib.h"
}

// include
#include "kwb_defines.h"
#include "prjtypes.h"

// os/libsystem
#include "sysgetopt.h"

// os/shared
#include "ioloop.h"
#include "log.h"

#include "router.h"
#include "rconnserial.h"
#include "rconnsocketclient.h"
#include "socketserver.h"

using namespace std;

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

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void set_options (options_t*     options,
                         const char*    serial_device,
                         int            serial_baudrate,
                         const char*    router_address,
                         uint16_t       router_port)
{
    memset(options, 0, sizeof(options_t));

    if (serial_device != NULL) {
        strcpy_s(options->serial_device, sizeof(options->serial_device), serial_device);
    }
    options->serial_baudrate = serial_baudrate;
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
        c = getopt(argc, argv, "d:b:a:p:v:w:n:");
        if (c == -1) break;

        switch (c) {
        case 'd':
            log_msg(KWB_LOG_INFO, "device %s", optarg);
            strcpy_s(options->serial_device, sizeof(options->serial_device), optarg);
            options->serial_device_set = true;
            break;
        case 'b':
            log_msg(KWB_LOG_INFO, "baudrate %s", optarg);
            options->serial_baudrate = atoi(optarg);
            break;
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

static bool validate_options(options_t* options)
{
    bool    rc = false,
            serial_device_set = false,
            vbusd_address_set = false;

    do {
        // minimum address is "/a": unix socket with name "a" in the root directory
        if (strnlen(options->router_address, sizeof(options->router_address)) < 2) {
            log_error("Missing router address!");
            break;
        }
        if (options->serial_device_set &&
            strnlen(options->serial_device, sizeof(options->serial_device)) < 2) {
            log_error("Invalid serial device path!");
            break;
        }

        rc = true;
    } while (0);
    return rc;
}

static void print_usage (void)
{
    fprintf(stderr, "\nUsage:\n");
    fprintf(stderr, "kwbrouter [-a <address>] [-p <port>] [-d <device>] [-b <baudrate>] [-v <vbusd address>] [-w <vbusd port>] [-n <node address>]\n\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, " -a <address>        Address of kwbrouter server. Default: /tmp/kwbr.usk\n");
    fprintf(stderr, " -p <port>           Port number of kwbrouter server. Default: 0\n");
    fprintf(stderr, " -d <device>         Device of serial bus connection.\n");
    fprintf(stderr, " -b <baudrate>       Baudrate of serial bus connection. Default: 57600\n");
}

static void create_unix_socket_file(options_t* options)
{
    FILE* file_handle;

    file_handle = fopen(options->router_address, "w+");
    if (file_handle != NULL) fclose(file_handle);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;
    bool            end_application = false;
    options_t       options;
    ioloop_t        mainloop;

    do {
        // all logs on
        log_set_mask(0xFFFFFFFF);
        log_msg(KWB_LOG_INFO, "kwbrouter...");

        // set default options for kwbrouter
        set_options(&options,
                    "",                 // no serial device
                    57600,              // baudrate, if not given
                    "/tmp/kwbr.usk",    // default address of kwbrouter socket
                    0);                 // port 0: use unix sockets

        // parse and validate commandline options
        if (parse_commandline_options(argc, argv, &options) == false ||
            validate_options(&options) == false) {
            print_usage();
            rc = eERR_BAD_PARAMETER;
            break;
        }

        ioloop_init(&mainloop);

        Router* router = new Router();

        RConnSerial* serconn = new RConnSerial(&mainloop);
        serconn->Open("/dev/ttyUSB0", 57600);
        router->AddConnection(serconn);

        // open new server and let-every connection auto-connect to router
        create_unix_socket_file(&options);
        SocketServer* server = new SocketServer(&mainloop, router);
        server->Open(options.router_address, options.router_port);

       // RConnSocketServer server = new
 /*
        route_init(&router);

        // initialize network routing
        msg_s_init(&msg_socket);
        if ((rc = msg_s_open_server(&msg_socket, &mainloop, "/tmp/kwb.usk", 0)) != eERR_NONE) {
            break;
        }
        msg_s_set_incomming_handler(&msg_socket, handle_incomming_msg, &router);
        msg_s_set_newconnection_handler(&msg_socket, handle_new_connection, &router);
        // add route for every client of unix socket
        ep = msg_s_get_endpoint(&msg_socket, 0, eMSG_EP_COMM);

        // initialize bus routing
        msg_b_init(&msg_bus, 0);
        if (options.serial_device_set) {
            address = options.serial_device;
            port = options.serial_baudrate;
            serial = true;
        } else {
            address = options.vbusd_address;
            port = options.vbusd_port;
            serial = false;
        }
        rc = msg_b_open(&msg_bus, &mainloop, options.own_node_address, serial,
                        address, port);
        if (rc != eERR_NONE) break;
        msg_b_set_incomming_handler(&msg_bus, handle_incomming_msg, &router);
        route_add(&router, 1, 65535, address, port, eROUTE_TYPE_SERIAL, &msg_bus);
*/
        log_msg(KWB_LOG_STATUS, "entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);
        }

        server->Close();
        delete server;

        router->RemoveConnection(serconn);
        serconn->Close();
        delete serconn;

        delete router;
    } while (0);
    return rc;
}

/** @} */
