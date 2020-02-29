/**
 * @addtogroup KWBFIRMWARE
 *
 * @{
 * @file    kwbfirmware_main.c
 * @brief   Update firmware of a module.
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
  #include <unistd.h>
#endif

// include
#include "prjtypes.h"

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

// os/shared
#include "ioloop.h"
#include "log.h"
#include "message_serial.h"

// os/libsystem
#include "sysgetopt.h"
#include "systime.h"

#include "firmwareupdate.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Structure of kwbfirmware runtime options.
 */
typedef struct options {
    char        serial_device[256]; //!< Device of serial connection to RS232 gateway.
    int         serial_baudrate;    //!< Baudrate of serial connection to RS232 gateway.
    char        router_address[256];//!< Address of kwbrouter server.
    uint16_t    router_port;        //!< Port number of kwbrouter server.
    bool        serial_device_set;  //!< Flag: if set, serial device has been
                                    //!< configured in the command line options.
    bool        router_address_set; //!< Flag: is set, router address has been
                                    //!< configured in the command line options.
    char        filename[256];
    uint16_t    node_address;       //!< own node address
} options_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Set kwbfirmware run options via function call.
 * Function can be used to set default parameters.
 *
 * @param[out]  options         Stucture where options are stored in.
 * @param[in]   serial_device   Device of serial connection to RS232 gateway.
 * @param[in]   serial_baudrate Baudrate of serial connection to RS232 gateway.
 * @param[in]   router_address  Address of kwbrouter server.
 * @param[in]   router_port     Port number of kwbrouter server.
 * @param[in]   node_address    Address of node of which the firmware will be
 *                              updated.
 */
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
        c = getopt(argc, argv, "d:b:a:p:n:f:v");
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
        case 'n':
            options->node_address = atoi(optarg);
            break;
        case 'f':
            strcpy_s(options->filename, sizeof(options->filename), optarg);
            break;
        case 'v':
            log_add_mask(KWB_LOG_VERBOSE1 | KWB_LOG_VERBOSE2);
            break;
        default:
            rc = false;
            break;
        }
    }
    return rc;
}

/**
 * Validate kwbfirmware runtime options.
 *
 * @param[in,out]   options Structure where options are stored in.
 * @returns         true if options are constient and usable, otherwise false.
 */
 static bool validate_options(options_t* options)
{
    bool    rc = false,
            serial_device_set = false,
            vbusd_address_set = false;

    do {
        // minimum address is "/a": unix socket with name "a" in the root directory
        if (strnlen(options->router_address, sizeof(options->router_address)) < 2) {
            log_error("Missing router address!\n");
            break;
        }
        if (!options->serial_device_set || strnlen(options->serial_device, sizeof(options->serial_device)) < 2) {
            log_error("Invalid or missing serial connection device!\n");
            break;
        }
        if (strnlen(options->filename, sizeof(options->filename)) < 2) {
            log_error("Filename of firmware needed!\n");
            break;
        }
        if (options->node_address == 0 || options->node_address == 65535) {
            log_error("Only node addresses from 1 to 65534 allowed!\n");
            break;
        }
        rc = true;
    } while (0);
    return rc;
}

/**
 * Print usage notes to command prompt.
 */
static void print_usage (void)
{
    fprintf(stderr, "\nUsage:\n");
    fprintf(stderr, "kwbfirmware [-a <address>] [-p <port>] [-d <device>] [-b <baudrate>] [-n <node address>] [-f <path and filename to hex-file>]\n\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, " -a <address>        Address of kwbrouter server. Default: /tmp/kwbr.usk\n");
    fprintf(stderr, " -p <port>           Port number of kwbrouter server. Default: 0\n");
    fprintf(stderr, " -d <device>         Device of serial connection.\n");
    fprintf(stderr, " -b <baudrate>       Baudrate of serial connection. Default: 57600\n");
    fprintf(stderr, " -n <node address>   Node address of module to update.\n");
    fprintf(stderr, " -f <filename>       Filename of firmware to update.\n");
    fprintf(stderr, " -v                  Verbose logging.\n");

    #ifdef PRJCONF_WINDOWS
    fprintf(stderr, "\n" \
                    "NOTE: serial ports enumerated greater or equal to COM10\n" \
                    "      should be stated as follows: \\\\.\\COM10\n");
    #endif // PRJCONF_WINDOWS
}

static void print_progress (uint8_t progress, void* arg)
{
    log_msg(LOG_STATUS, "Update progress %d", progress);
    log_msg(KWB_LOG_INFO, "Update progress %d", progress);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * main entry point of kwbfirmware.
 *
 * @param[in]   argc    Number of command line arguments.
 * @param[in]   argv    List of command line arguments.
 * @returns     0 if firmware has been updated in target node successfully.
 */
int main (int argc, char* argv[])
{
    int                 rc = eERR_NONE;
    options_t           options;
    bool                end_application = false;
    ioloop_t            mainloop;
    firmwareupdate_t    firmware;

    do {
        log_set_mask(LOG_STANDARD_MASK | LOG_STATUS | LOG_USERDEFINED | LOG_VERBOSE1 | LOG_VERBOSE2);
        
        // set default options for kwbrouter
        set_options(&options,
                    "/dev/ttyUSB0",                 // no serial device, use vbusd as default
                    57600,              // baudrate, if not given
                    "/tmp/kwbr.usk",    // default address of vbusd socket
                    0,                  // port 0: use unix sockets
                    0x0e);                 // own node address
        // parse and validate commandline options
        if (parse_commandline_options(argc, argv, &options) == false ||
            validate_options(&options) == false) {
            print_usage();
            rc = eERR_BAD_PARAMETER;
            break;
        }

        ioloop_init(&mainloop);
        ioloop_set_default_timeout(&mainloop, 1);

        rc = firmware_update_init(&firmware, &mainloop, options.serial_device, options.serial_baudrate);
        if (rc != eERR_NONE) break;

        firmware_register_progress_func(&firmware, print_progress, NULL);

        rc = firmware_update_start(&firmware, options.filename, options.node_address);
        if (rc != eERR_NONE) break;

        while (!end_application) {
            ioloop_run_once(&mainloop);
            rc = firmware_update_run(&firmware);
            if (rc != eRUNNING) {
                if (rc == eMSG_ERR_BUSY) {
                    sys_sleep_ms(100);
                } else if (rc == eERR_NONE) {
                    log_msg(LOG_STATUS, "FIRMWARE UPDATE SUCCESSFULL!");
                    log_msg(LOG_STATUS, "Bootloader flags %02X", firmware.bldflags);
                    end_application = true;
                } else {
                    log_msg(LOG_STATUS, "FIRMWARE UPDATE FAILED!");
                    log_msg(LOG_STATUS, "CRC expected %04X CRC calculated %04X", 
                            firmware.node_crc_calculated, firmware.node_crc_expected);
                    log_msg(LOG_STATUS, "Bootloader flags %02X", firmware.bldflags);
                    
                    end_application = true;
                }
            }
        }
        firmware_update_close(&firmware);
    } while (0);
    return rc;
}

/** @} */
