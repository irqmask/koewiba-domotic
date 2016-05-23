/**
 * @addtogroup KWBSCHEDULER
 *
 * @{
 * @file    kwbscheduler_main.c
 * @brief   Operating system version of the bus scheduler.
 *
 * kwbscheduler schedules the messages on the bus. This is the operating system
 * version.
 * kwbscheduler can connect in two ways to a bus:
 * - via direct serial connection to a physical bus
 * - via vbusd connection to a virtual bus. The virtual bus may be connected
 *   with vbusd to a physical bus.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

 ///@todo remove printfs
 ///@todo react on rerrors of send/receive function properly -> man pages

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
  #include <unistd.h>
#endif

#include "message_bus.h"
#include "bus_scheduler.h"
#include "ioloop.h"
#include "log.h"
#include "prjtypes.h"
#include "vos.h"
#include "system.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct options {
    char        serial_device[256];
    int         serial_baudrate;
    char        vbusd_address[256];
    uint16_t    vbusd_port;
    bool        serial_device_set;
    bool        vbusd_address_set;
    uint16_t    own_node_address;
} options_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void print_usage (void)
{
    printf("\nkwbscheduler - KoeWiBa bus scheduler\n");
    printf("\nUsage:\n");
    printf("kwbscheduler [-d <device>] [-b <baudrate>] [-v <vbusd address>] [-w <vbusd port>]\n\n");
    printf("Arguments:\n");
    printf(" -d <device>         Device of serial bus connection.\n");
    printf(" -b <baudrate>       Baudrate of serial bus connection. Default: 38400\n");
    printf(" -v <vbusd address>  Address of vbusd. Default: /tmp/vbusd.usk\n");
    printf(" -w <vbusd port>     Port number of vbusd. Default: 0\n");
}

static void set_options (options_t*     options,
                         const char*    serial_device,
                         int            serial_baudrate,
                         const char*    vbusd_address,
                         uint16_t       vbusd_port,
                         uint16_t       own_node_address
                        )
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
    options->own_node_address = own_node_address;
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
            case 'n':
                options->own_node_address = atoi(optarg);
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
            strnlen_s(options->serial_device, sizeof(options->serial_device)) < 2) {
            log_error("Invalid serial device path!\n");
            break;
        }
        if (options->vbusd_address_set &&
            strnlen_s(options->vbusd_address, sizeof(options->vbusd_address)) < 2) {
            log_error("Invalid vbusd address!\n");
            break;
        }
        if (options->own_node_address == 0 ||
            options->own_node_address == 0xFF ||
            options->own_node_address == 0xFFFF) {
            log_error("Invalid own node address %d. It has to be 1..254, 256..65534\n",
                    options->own_node_address);
            break;
        }
        rc = true;
    } while (0);
    return rc;
}

void init_scheduling (msg_bus_t* busscheduler, uint16_t own_node_address)
{
    msg_b_init(busscheduler, 0);
    
    clk_initialize();
    bus_configure(&busscheduler->bus, own_node_address);
    bus_scheduler_initialize(&busscheduler->bus, &busscheduler->scheduler, 0);
}

int32_t do_scheduling (void* arg)
{
    uint16_t sender = 0;
    uint8_t length = 0, message[BUS_MAXRECVMSGLEN];
    msg_bus_t* busscheduler = (msg_bus_t*)arg;

    if (bus_schedule_and_get_message(&busscheduler->bus, &busscheduler->scheduler)) {
        bus_read_message(&busscheduler->bus, &sender, &length, message);
    }
    return clk_timers_next_expiration();
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(int argc, char* argv[])
{
    int         rc = eERR_NONE;
    options_t   options;
    bool        end_application = false;
    ioloop_t    mainloop;
    msg_bus_t   busscheduler;


    printf("kwbscheduler...\n");
    setbuf(stdout, NULL);       // disable buffering of stdout

    do {
        ioloop_init(&mainloop);
        set_options(&options, "", 38400, "/tmp/vbusd.usk", 0, 1);
        if (!parse_commandline_options(argc, argv, &options) ||
            !validate_options(&options)) {
            print_usage();
            rc = eERR_BAD_PARAMETER;
            break;
        }
        init_scheduling(&busscheduler, options.own_node_address);
        if (options.serial_device_set) {
            rc = vos_open_serial(&busscheduler.vos, options.serial_device, options.serial_baudrate);
        } else {
            rc = vos_open_vbusd(&busscheduler.vos, options.vbusd_address, options.vbusd_port);
        }
        if (rc != eERR_NONE) break;

        // schedule on incomming bytes and after timer expiration
        ioloop_register_fd(&mainloop, busscheduler.vos.fd, eIOLOOP_EV_READ, do_scheduling, &busscheduler);
        ioloop_register_timer(&mainloop, 10, true, eIOLOOP_EV_TIMER, do_scheduling, &busscheduler);

        while (!end_application) {
            ioloop_run_once(&mainloop);
        }

        vos_close(&busscheduler.vos);
    } while (0);

    return rc;
}
