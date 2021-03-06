/**
 * @addtogroup KWBMONITOR
 *
 * @{
 * @file    kwbmonitor_main.c
 * @brief   Analyze KWB bus communication on a byte level.
 *
 * kwbmonitor can connect in two ways to a bus:
 * - via direct serial connection to a physical bus
 * - via vbusd connection to a virtual bus. The virtual bus may be connected
 *   with vbusd to a physical bus.
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

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <safe_lib.h>
    #include <unistd.h>
#endif

// include
#include "prjtypes.h"

// libsystem
#include "sysconsole.h"
#include "syserror.h"
#include "system.h"
#include "sysgetopt.h"
#include "systhread.h"

#include "kwbmonitor.h"
#include "vos.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Structure of kwbconfiguration runtime options.
 */
typedef struct options {
    char        serial_device[256]; //!< Device of serial connection to RS232 gateway.
    int         serial_baudrate;    //!< Baudrate of serial connection to RS232 gateway.
    char        vbusd_address[256]; //!< IPv4 address of virtual bus
    uint16_t    vbusd_port;         //!< IPv4 port of virtual bus
    bool        serial_device_set;  //!< Flag: if set, serial device has been
                                    //!< configured in the command line options.
    bool        vbusd_address_set;  //!< Flag: if set, virtual bus has been
                                    //!< configured in the command line options.
} options_t;

// --- Local variables ---------------------------------------------------------

static bus_history_t    g_bus_history;
static vos_t            g_vos;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Monitor functionality runs in a thread.
 */
sys_thread_func(reader_thread)
{
    uint8_t new_byte = 0;
    int     bytesread;

    monitor_init(&g_bus_history);

    while (1) {
        bytesread = vos_recv(&g_vos, &new_byte, 1);
        if (bytesread > 0) {
            monitor_parse_message(new_byte, &g_bus_history);
        }
        else {
            sys_sleep_ms(1);
        }
    }
    return 0;
}

//----------------------------------------------------------------------------
static void print_usage(void)
{
    fprintf(stderr, "\nkwbmonitor - KoeWiBa bus monitor on a byte level\n");
    fprintf(stderr, "\nUsage:\n");
    fprintf(stderr, "kwbmonitor [-d <device>] [-b <baudrate>] [-v <vbusd address>] [-w <vbusd port>]\n\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, " -d <device>         Device of serial bus connection.\n");
    fprintf(stderr, " -b <baudrate>       Baudrate of serial bus connection. Default: 38400\n");
    fprintf(stderr, " -v <vbusd address>  Address of vbusd. Default: /tmp/vbusd.usk\n");
    fprintf(stderr, " -w <vbusd port>     Port number of vbusd. Default: 0\n");

#ifdef PRJCONF_WINDOWS
    fprintf(stderr, "\n" \
            "NOTE: serial ports enumerated greater or equal to COM10\n" \
            "      should be stated as follows: \\\\.\\COM10\n");
#endif // PRJCONF_WINDOWS
}

//----------------------------------------------------------------------------
static void set_options(options_t     *options,
                        const char    *serial_device,
                        int            serial_baudrate,
                        const char    *vbusd_address,
                        uint16_t       vbusd_port)
{
    memset(options, 0, sizeof(options_t));

    if (serial_device != NULL) {
        strcpy_s(options->serial_device, sizeof(options->serial_device), serial_device);
    }
    options->serial_baudrate = serial_baudrate;
    if (vbusd_address != NULL) {
        strcpy_s(options->vbusd_address, sizeof(options->vbusd_address), vbusd_address);
    }
    options->vbusd_port = vbusd_port;
}

//----------------------------------------------------------------------------
static bool parse_commandline_options(int          argc,
                                      char        *argv[],
                                      options_t   *options)
{
    bool                    rc = true;
    int                     c;

    while (1) {
        c = getopt(argc, argv, "d:b:v:w:");
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'd':
            fprintf(stderr, "device %s\n", optarg);
            strcpy_s(options->serial_device, sizeof(options->serial_device), optarg);
            options->serial_device_set = true;
            break;
        case 'b':
            fprintf(stderr, "baudrate %s\n", optarg);
            options->serial_baudrate = atoi(optarg);
            break;
        case 'v':
            fprintf(stderr, "vbusd address %s\n", optarg);
            strcpy_s(options->vbusd_address, sizeof(options->vbusd_address), optarg);
            options->vbusd_address_set = true;
            break;
        case 'w':
            fprintf(stderr, "vbusd port %s\n", optarg);
            options->vbusd_port = atoi(optarg);
            break;
        default:
            rc = false;
            break;
        }
    }
    return rc;
}

//----------------------------------------------------------------------------
static bool validate_options(options_t *options)
{
    bool    rc = false;

    do {
        // minimum address is "/a": unix socket with name "a" in the root directory
        if (options->serial_device_set &&
            strnlen(options->serial_device, sizeof(options->serial_device)) < 2) {
            fprintf(stderr, "Invalid serial device path!\n");
            break;
        }
        if (options->vbusd_address_set &&
            strnlen(options->vbusd_address, sizeof(options->vbusd_address)) < 2) {
            fprintf(stderr, "Invalid vbusd address!\n");
            break;
        }
        rc = true;
    } while (0);
    return rc;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * main entry point of kwbmonitor.
 *
 * @param[in]   argc    Number of command line arguments.
 * @param[in]   argv    List of command line arguments.
 * @returns     0 if kwbmonitor run successfully.
 */
int main(int argc, char *argv[])
{
    char cc;
    int rc = eERR_NONE;
    options_t options;
    bool running = true;

    fprintf(stderr, "kwbmonitor...\n");
    setbuf(stdout, NULL);       // disable buffering of stdout

    do {
        set_options(&options, "", 38400, "/tmp/vbusd.usk", 0);
        if (!parse_commandline_options(argc, argv, &options) ||
            !validate_options(&options)) {
            print_usage();
            rc = eERR_BAD_PARAMETER;
            break;
        }

        if (options.serial_device_set) {
            rc = vos_open_serial(&g_vos, options.serial_device, options.serial_baudrate);
        }
        else {
            rc = vos_open_vbusd(&g_vos, options.vbusd_address, options.vbusd_port);
        }
        if (rc != eERR_NONE) {
            break;
        }

        sys_thread_start(&reader_thread);
        while (running) {
            cc = sys_con_getch();
            switch (cc) {
            case 27: //ESC
                running = false;
                break;

            case 'e':
            case 'E':
                monitor_toggle_display_empty_message();
                break;

            case 't':
            case 'T':
                monitor_toggle_display_token_message();
                break;

            default:
                break;
            }

            sys_sleep_ms(10);
        }

        vos_close(&g_vos);
    } while (0);

    return rc;
}

/** @} */
