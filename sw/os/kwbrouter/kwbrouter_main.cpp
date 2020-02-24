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
 * @todo Add possibility to have two servers simultaneously, one for unix 
 *       sockets, one for tcp sockets.
 * @todo Add possibility for router beeing a client to another router.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
#include <safe_lib.h>
#endif
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

//! Stores default and interpreted command-line arguments
typedef struct options {
    char        serial_device[256];         //!< Name of the serial device connection.
    int         serial_baudrate;            //!< Baudrate for the serial connection.
    char        unix_address[256];          //!< Address of the unix socket for
                                            //!< local socket communication.
    char        remote_router_address[256]; //!< TCP address of the router to connect to.
    uint16_t    router_server_port;         //!< Listening port of the router
    bool        serial_device_set;          //!< Flag, if a serial device is configured.
    bool        router_server_configured;   //!< Flag, if router as server is configured.
    bool        router_client_configured;   //!< Flag, if router as client is configured.
    uint16_t    own_node_address;           //!< own node address
} options_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Set kwbrouter run options via function call.
 * Function can be used to set default parameters.
 *
 * @param[out]  options         Stucture where options are stored in.
 * @param[in]   serial_device   Device of serial connection to RS232 gateway.
 * @param[in]   serial_baudrate Baudrate of serial connection to RS232 gateway.
 * @param[in]   unix_address    Address of file for unix sockets.
 * @param[in]   remote_router_address   Address of peer kwbrouter which acts as a server.
 *                                      With this address set, this kwbrouter is a client to another kwbrouter which acts as server.
 * @param[in]   router_server_port      Portnumber to use when listzening as server or connecting to as client.
 * @param[in]   own_node_id     Address of node over which this software communicates to the bus.
 */
static void set_options (options_t*     options,
                         const char*    serial_device,
                         int            serial_baudrate,
                         const char*    unix_address,
                         const char*    remote_router_address,
                         uint16_t       router_server_port,
                         uint16_t       own_node_id)
{
    memset(options, 0, sizeof(options_t));

    if (serial_device != NULL) {
        strcpy_s(options->serial_device, sizeof(options->serial_device), serial_device);
    }
    options->serial_baudrate = serial_baudrate;

    if (unix_address != NULL) {
        strcpy_s(options->unix_address, sizeof(options->unix_address), unix_address);
    }

    if (remote_router_address != NULL) {
        strcpy_s(options->remote_router_address, sizeof(options->remote_router_address), remote_router_address);
    }
    options->router_server_port = router_server_port;
    options->own_node_address = own_node_id;
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
    bool router_address_set = false;
    bool router_port_set = false;


    while (1) {
        c = getopt(argc, argv, "d:b:a:p:o:");
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
            log_msg(KWB_LOG_INFO, "remote router address %s", optarg);
            strcpy_s(options->remote_router_address, sizeof(options->remote_router_address), optarg);
            router_address_set = true;
            break;
        case 'p':
            log_msg(KWB_LOG_INFO, "router server port %s", optarg);
            options->router_server_port = atoi(optarg);
            router_port_set = true;
            break;
        case 'o':
            options->own_node_address = atoi(optarg);
            break;
        default:
            rc = false;
            break;
        }
    }
    if (router_address_set && router_port_set) {
        options->router_server_configured = false;
        options->router_client_configured = true;
    }
    else if (router_port_set) {
        options->router_server_configured = true;
        options->router_client_configured = false;
    }
    else {
        options->router_server_configured = false;
        options->router_client_configured = false;
    }

    return rc;
}

/**
 * Check for missing and given arguments which are mutual exclusive and validate the values.
 *
 * @param[in] options   Structure of options to check.
 * @returns true, if the options are valid, otherwise false.
 */
static bool validate_options(options_t* options)
{
    bool    rc = false;

    do {
        // minimum address is "/a": unix socket with name "a" in the root directory
        if (strnlen(options->unix_address, sizeof(options->unix_address)) < 2) {
            log_error("Missing unix socket address!");
            break;
        }
        if (options->router_client_configured &&
            strnlen(options->remote_router_address, sizeof(options->remote_router_address)) < 2) {
            log_error("Missing remote router address!");
            break;
        }
        if (options->router_server_configured && options->router_server_port == 0) {
            log_error("Missing router server port!");
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

/**
 * Explain briefly the command-line arguments of the application.
 */
static void print_usage (void)
{
    fprintf(stderr, "\nUsage:\n");
    fprintf(stderr, "kwbrouter [-a <address>] [-p <port>] [-d <device>] [-b <baudrate>] [-v <vbusd address>] [-w <vbusd port>] [-n <node address>]\n\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, " -a <remote_address> As client: Address of remote kwbrouter server. e.g. 192.168.178.100:54321\n");
    fprintf(stderr, " -p <server_port>    As server: Port number to listen to.\n");
    fprintf(stderr, " -d <device>         Device of serial bus connection. e.g. /dev/ttyUSB0\n");
    fprintf(stderr, " -b <baudrate>       Baudrate of serial bus connection. Default: 57600\n");
    fprintf(stderr, " -o <node address>   Own node address (has to be the same address as connected gateway module)\n");
}

/**
 * Name speeks for itself. A file is needed to open a unix socket otherwise
 * you'll get "file not found" error.
 * @todo consider moving this into /ref MESSAGE_SOCKET module.
 */
static void create_unix_socket_file(options_t* options)
{
    FILE* file_handle;

    file_handle = fopen(options->remote_router_address, "w+");
    if (file_handle != NULL) fclose(file_handle);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Entry point into kwbrouter application.
 */
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
                    "",                 // no default tcp-client address
                    0,                  // no default server port
                    0);                 // no defaul own node address.

        // parse and validate commandline options
        if (parse_commandline_options(argc, argv, &options) == false ||
            validate_options(&options) == false) {
            print_usage();
            rc = eERR_BAD_PARAMETER;
            break;
        }

        ioloop_init(&mainloop);

        Router* router = new Router();

        RConnSerial* serconn = nullptr;
        do {
            if (!options.serial_device_set) break;
            serconn = new RConnSerial(&mainloop);
            if (serconn == nullptr) {
                log_error("Unable to allocate serial connection object!");
                break;
            }
            serconn->SetSegmentAddress(options.own_node_address);
            rc = serconn->Open(options.serial_device, options.serial_baudrate);
            if (rc != 0) {
                log_error("Opening serial connection to %s failed with error %d!", options.serial_device, rc);
                delete serconn;
                serconn = nullptr;
                break;
            }
            router->AddConnection(serconn);
        } while (0);

        RConnSocketClient* sockconn = nullptr;
        do {
            if (!options.router_client_configured) break;
            sockconn = new RConnSocketClient(&mainloop, options.remote_router_address, options.router_server_port);
            if (sockconn == nullptr) {
                log_error("Unable to allocate socket connection object!");
                break;
            }
            rc = sockconn->Open(options.remote_router_address, options.router_server_port);
            if (rc != 0) {
                log_error("Opening socket connection to %s failed with error %d!", options.remote_router_address, rc);
                delete sockconn;
                sockconn = nullptr;
                break;
            }
            router->AddConnection(sockconn);
        } while (0);

        SocketServer* unix_socket_server = nullptr;
        do {
            // open new server and let-every connection auto-connect to router
            create_unix_socket_file(&options);
            unix_socket_server = new SocketServer(&mainloop, router);
            if (unix_socket_server == nullptr) {
                log_error("Unable to allocate unix-socket connection object!");
                break;
            }
            rc = unix_socket_server->Open(options.unix_address, 0);
            if (rc != 0) {
                log_error("Unable to open unix-socket connection at %s, error %d!", options.unix_address, rc);
                delete unix_socket_server;
                unix_socket_server = nullptr;
                break;
            }
        } while (0);

        SocketServer* socket_server = nullptr;
        do {
            if (!options.router_server_configured) break;
            socket_server = new SocketServer(&mainloop, router);
            if (socket_server == nullptr) {
                log_error("Unable to allocate socket server object!");
                break;
            }
            rc = socket_server->Open("", options.router_server_port);
            if (rc != 0) {
                log_error("Unable to open socket server listening at port %d, error %d!", options.router_server_port, rc);
                delete socket_server;
                socket_server = nullptr;
                break;
            }
        } while (0);

        log_msg(KWB_LOG_STATUS, "entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);
        }

        if (unix_socket_server != nullptr) {
            unix_socket_server->Close();
            delete unix_socket_server;
        }

        if (socket_server != nullptr) {
            socket_server->Close();
            delete socket_server;
        }

        if (sockconn != nullptr) {
            router->RemoveConnection(sockconn);
            sockconn->Close();
            delete sockconn;
        }

        if (serconn != nullptr) {
            router->RemoveConnection(serconn);
            serconn->Close();
            delete serconn;
        }

        if (router != nullptr) {
            delete router;
        }
    } while (0);
    return rc;
}

/** @} */
