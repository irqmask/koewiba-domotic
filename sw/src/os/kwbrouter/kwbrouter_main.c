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

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
  #include <unistd.h>
#endif

#include "bus.h"
#include "ioloop.h"
#include "message_bus.h"
#include "message_socket.h"
#include "router.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct options {
    char        serial_device[256];
    int         serial_baudrate;
    char        router_address[256];
    uint16_t    router_port;
    char        vbusd_address[256];
    uint16_t    vbusd_port;
    bool        serial_device_set;
    bool        router_address_set;
    bool        vbusd_address_set;
    uint16_t    own_node_address;
} options_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void set_options (options_t*     options,
                         const char*    serial_device,
                         int            serial_baudrate,
                         const char*    router_address,
                         uint16_t       router_port,
                         const char*    vbusd_address,
                         uint16_t       vbusd_port,
                         uint16_t       own_node_address)
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
    if (vbusd_address != NULL) {
        strcpy_s(options->vbusd_address, sizeof(options->vbusd_address), vbusd_address);
    }
    options->vbusd_port = vbusd_port;
    options->own_node_address = own_node_address;
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
            printf("device %s\n", optarg);
            strcpy_s(options->serial_device, sizeof(options->serial_device), optarg);
            options->serial_device_set = true;
            break;
        case 'b':
            printf("baudrate %s\n", optarg);
            options->serial_baudrate = atoi(optarg);
            break;
        case 'a':
            printf("router address %s\n", optarg);
            strcpy_s(options->router_address, sizeof(options->router_address), optarg);
            options->router_address_set = true;
            break;
        case 'p':
            printf("router port %s\n", optarg);
            options->router_port = atoi(optarg);
            break;
        case 'v':
            printf("vbusd address %s\n", optarg);
            strcpy_s(options->vbusd_address, sizeof(options->vbusd_address), optarg);
            options->vbusd_address_set = true;
            break;
        case 'w':
            printf("vbusd port %s\n", optarg);
            options->vbusd_port = atoi(optarg);
            break;
        case 'n':
            options->own_node_address = atoi(optarg);
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
        if (strnlen_s(options->router_address, sizeof(options->router_address)) < 2) {
            fprintf(stderr, "Missing router address!\n");
            break;
        }
        if (options->serial_device_set &&
            strnlen_s(options->serial_device, sizeof(options->serial_device)) < 2) {
            fprintf(stderr, "Invalid serial device path!\n");
            break;
        }
        if (options->vbusd_address_set &&
            strnlen_s(options->vbusd_address, sizeof(options->vbusd_address)) < 2) {
            fprintf(stderr, "Invalid vbusd address!\n");
            break;
        }

        if (options->serial_device_set && options->vbusd_address_set) {
            fprintf(stderr, "vbusd address and serial device at the same time not allowed!\n");
            break;
        }

        if (options->own_node_address == 0 || options->own_node_address == 65535) {
            fprintf(stderr, "Only node addresses from 1 to 65534 allowed!\n");
            break;
        }
        // activate standard vbusd connection if serial path is not given
        if (!options->serial_device_set) options->vbusd_address_set = true;
        rc = true;
    } while (0);
    return rc;
}

static void print_usage (void)
{
    printf("\nUsage:\n");
    printf("kwbrouter [-a <address>] [-p <port>] [-d <device>] [-b <baudrate>] [-v <vbusd address>] [-w <vbusd port>] [-n <node address>]\n\n");
    printf("Arguments:\n");
    printf(" -a <address>        Address of kwbrouter server. Default: /tmp/kwbr.usk\n");
    printf(" -p <port>           Port number of kwbrouter server. Default: 0\n");
    printf(" -d <device>         Device of serial bus connection.\n");
    printf(" -b <baudrate>       Baudrate of serial bus connection. Default: 38400\n");
    printf(" -v <vbusd address>  Address of vbusd. Default: /tmp/vbusd.usk\n");
    printf(" -w <vbusd port>     Port number of vbusd. Default: 0\n");
    printf(" -n <node address>   Own node address with which the kwbrouter appears on the bus.\n");
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;
    bool            end_application = false;
    options_t       options;
    ioloop_t        mainloop;
    msg_socket_t    msg_socket;
    msg_bus_t       msg_bus;
    msg_endpoint_t* ep;
    msg_t           msg;
    router_t        router;

    do {
        printf("\nkwbrouter...\n");

        // set default options for kwbrouter
        set_options(&options,
                    "",                 // no serial device, use vbusd as default
                    38400,              // baudrate, if not given
                    "/tmp/kwbr.usk",    // default address of vbusd socket
                    0,                  // port 0: use unix sockets
                    "/tmp/vbusd.usk",   // default address of vbusd socket
                    0,                  // port 0: use unix sockets
                    2);                 // own node address
        // parse and validate commandline options
        if (parse_commandline_options(argc, argv, &options) == false ||
            validate_options(&options) == false) {
            print_usage();
            rc = eSYS_ERR_BAD_PARAMETER;
            break;
        }
        ioloop_init (&mainloop);

        msg_s_init(&msg_socket);
        if ((rc = msg_s_open_server(&msg_socket, &mainloop, "/tmp/kwb.usk", 0)) != eERR_NONE) {
            break;
        }

        msg_b_init(&msg_bus, 0);
        if (options.serial_device_set) {
            rc = msg_b_open(&msg_bus, &mainloop, options.own_node_address, true,
                            options.serial_device, options.serial_baudrate);
        } else {
            rc = msg_b_open(&msg_bus, &mainloop, options.own_node_address, false,
                            options.vbusd_address, options.vbusd_port);
        }
        if (rc != eERR_NONE) break;

        route_init(&router);
        route_add(&router, 5, 10, "192.168.35.1", 5000, eROUTE_TYPE_SOCKET, NULL);
        route_add(&router, 1, 10, "192.168.35.2", 5000, eROUTE_TYPE_SOCKET, NULL);
        route_add(&router, 1, 10, "192.168.35.3", 5000, eROUTE_TYPE_SOCKET, NULL);
        route_add(&router, 1, 65535, "/tmp/kwb.usk", 0, eROUTE_TYPE_SOCKET, NULL);
        route_add(&router, 100, 10, "192.168.35.3", 4999, eROUTE_TYPE_SOCKET, NULL);
        route_add(&router, 6, 11, "192.168.35.4", 5000, eROUTE_TYPE_SOCKET, NULL);
        route_add(&router, 256, 511, "192.168.35.5", 50000, eROUTE_TYPE_SOCKET, NULL);

        msg.receiver = 15;
        msg.sender = 1;
        msg.length = 8;
        msg.data[0] = 'H';
        msg.data[1] = 'a';
        msg.data[2] = 'l';
        msg.data[3] = 'l';
        msg.data[4] = 'o';
        msg.data[5] = 'M';
        msg.data[6] = 's';
        msg.data[7] = 'g';

        printf("entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);

            ep = msg_s_get_endpoint(&msg_socket, 0, 1<<eMSG_EP_COMM);
            if (ep != NULL) {
                msg_s_send(ep, &msg);
                msg.sender++;
            }
        }
    } while (0);
    return rc;
}

/** @} */
