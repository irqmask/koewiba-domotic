/**
 * @addtogroup KWBFIRMWARE
 *
 * @{
 * @file    kwbfirmware_main.c
 * @brief   Update firmware of a module.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <safe_lib.h>

#include "error_codes.h"
#include "ioloop.h"
#include "firmwareupdate.h"
#include "message_serial.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct options {
    char        serial_device[256];
    int         serial_baudrate;
    char        router_address[256];
    uint16_t    router_port;
    bool        serial_device_set;
    bool        router_address_set;
    char        filename[256];
    uint16_t    node_address;
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
                         uint16_t       node_address)
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
    options->node_address = node_address;
}

static bool parse_commandline_options (int argc, char* argv[], options_t* options)
{
    bool                    rc = true;
    int                     c;

    while (1) {
        c = getopt(argc, argv, "d:b:a:p:n:f:");
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
        case 'n':
            options->node_address = atoi(optarg);
            break;
        case 'f':
            strcpy_s(options->filename, sizeof(options->filename), optarg);
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
        if (strnlen_s(options->router_address, sizeof(options->router_address)) < 2) {
            fprintf(stderr, "Missing router address!\n");
            break;
        }
        if (!options->serial_device_set || strnlen_s(options->serial_device, sizeof(options->serial_device)) < 2) {
            fprintf(stderr, "Invalid or missing serial connection device!\n");
            break;
        }
        if (strnlen_s(options->filename, sizeof(options->filename)) < 2) {
            fprintf(stderr, "Filename of firmware needed!\n");
            break;
        }
        if (options->node_address == 0 || options->node_address == 65535) {
            fprintf(stderr, "Only node addresses from 1 to 65534 allowed!\n");
            break;
        }
        rc = true;
    } while (0);
    return rc;
}

static void print_usage (void)
{
    printf("\nUsage:\n");
    printf("kwbfirmware [-a <address>] [-p <port>] [-d <device>] [-b <baudrate>] [-n <node address>] [-f <path and filename to hex-file>]\n\n");
    printf("Arguments:\n");
    printf(" -a <address>        Address of kwbrouter server. Default: /tmp/kwbr.usk\n");
    printf(" -p <port>           Port number of kwbrouter server. Default: 0\n");
    printf(" -d <device>         Device of serial connection.\n");
    printf(" -b <baudrate>       Baudrate of serial connection. Default: 57600\n");
    printf(" -n <node address>   Node address of module to update.\n");
    printf(" -f <filename>       Filename of firmware to update.\n");
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int                 rc = eERR_NONE;
    options_t           options;
    bool                end_application = false;
    ioloop_t            mainloop;
    firmwareupdate_t    firmware;

    do {
        printf("\nkwbfirmware...\n");

        // set default options for kwbrouter
        set_options(&options,
                    "/dev/ttyUSB0",                 // no serial device, use vbusd as default
                    57600,              // baudrate, if not given
                    "/tmp/kwbr.usk",    // default address of vbusd socket
                    0,                  // port 0: use unix sockets
                    2);                 // own node address
        // parse and validate commandline options
        if (parse_commandline_options(argc, argv, &options) == false ||
            validate_options(&options) == false) {
            print_usage();
            rc = eERR_BAD_PARAMETER;
            break;
        }

        ioloop_init(&mainloop);
        ioloop_set_default_timeout(&mainloop, 1);

        firmware_update_init(&firmware, &mainloop, options.serial_device, options.serial_baudrate);
        rc = firmware_update_start(&firmware, options.filename, options.node_address);
        if (rc != eERR_NONE) break;

        printf("entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);
            rc = firmware_update_run(&firmware);
            if (rc != eRUNNING) {
                if (rc == eMSG_ERR_BUSY) {
                    sys_sleep_ms(100);
                } else {
                    // end or error
                    end_application = true;
                }
            }
        }
        firmware_update_close(&firmware);
    } while (0);
    return rc;
}

/** @} */
