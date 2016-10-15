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

typedef struct options {
    char        serial_device[256];
    int         serial_baudrate;
    char        vbusd_address[256];
    uint16_t    vbusd_port;
    bool        serial_device_set;
    bool        vbusd_address_set;
} options_t;

// --- Local variables ---------------------------------------------------------

static bus_history_t    g_bus_history;
static vos_t            g_vos;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

sys_thread_func (reader_thread)
{
    uint8_t new_byte = 0;
    int     bytesread;

    monitor_init(&g_bus_history);

    while (1) {
        bytesread = vos_recv(&g_vos, &new_byte, 1);
        if (bytesread > 0) {
            monitor_parse_message(new_byte, &g_bus_history);
        } else {
            sys_sleep_ms(1);
        }
    }
    return 0;
}

static void print_usage (void)
{
    printf("\nkwbmonitor - KoeWiBa bus monitor on a byte level\n");
    printf("\nUsage:\n");
    printf("kwbmonitor [-d <device>] [-b <baudrate>] [-v <vbusd address>] [-w <vbusd port>]\n\n");
    printf("Arguments:\n");
    printf(" -d <device>         Device of serial bus connection.\n");
    printf(" -b <baudrate>       Baudrate of serial bus connection. Default: 38400\n");
    printf(" -v <vbusd address>  Address of vbusd. Default: /tmp/vbusd.usk\n");
    printf(" -w <vbusd port>     Port number of vbusd. Default: 0\n");

    #ifdef PRJCONF_WINDOWS
    printf("\n" \
           "NOTE: serial ports enumerated greater or equal to COM10\n" \
           "      should be stated as follows: \\\\.\\COM10\n");
    #endif // PRJCONF_WINDOWS
}

static void set_options (options_t*     options,
                         const char*    serial_device,
                         int            serial_baudrate,
                         const char*    vbusd_address,
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

static bool parse_commandline_options (int          argc,
                                       char*        argv[],
                                       options_t*   options)
{
    bool                    rc = true;
    int                     c;

    while (1) {
        c = getopt(argc, argv, "d:b:v:w:");
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
            case 'v':
                printf("vbusd address %s\n", optarg);
                strcpy_s(options->vbusd_address, sizeof(options->vbusd_address), optarg);
                options->vbusd_address_set = true;
                break;
            case 'w':
                printf("vbusd port %s\n", optarg);
                options->vbusd_port = atoi(optarg);
                break;
            default:
                rc = false;
                break;
        }
    }
    return rc;
}

static bool validate_options (options_t* options)
{
    bool    rc = false,
    serial_device_set = false,
    vbusd_address_set = false;

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

int main(int argc, char* argv[])
{
    char cc;
    int rc = eERR_NONE;
    options_t options;
    bool running = true;

    printf("kwbmonitor...\n");
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
        } else {
            rc = vos_open_vbusd(&g_vos, options.vbusd_address, options.vbusd_port);
        }
        if (rc != eERR_NONE) break;

        sys_thread_start(&reader_thread);
        while(running) {
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
