/**
 * @addtogroup VBUSD
 *
 * @{
 * @file    vbusd_main.c
 * @brief   Virtual bus deamon..
 * vbusd implements a virtual bus, where clients can connect to. It also
 * supports a real bus connection to a serial device. So it is possible to run
 * bus applications like kwbrouter and diagnosis applications like kwbmonitor
 * on the same computer with only one serial connection.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <assert.h>
#include <getopt.h>
#include <malloc.h>
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

#include "ioloop.h"
#include "sysserial.h"
#include "syssocket.h"
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
} options_t;

// --- Local variables ---------------------------------------------------------

typedef enum vbusd_cl_type {
    eVBUSD_TYPE_UNKNOWN,
    eVBUSD_TYPE_SERIAL,
    eVBUSD_TYPE_SOCKET,
    eVBUSD_TYPE_LAST
} vbusd_cl_type_t;

typedef struct vbusd_client vbusd_client_t;

typedef struct vbusd_clients {
    vbusd_client_t*     first_client;
    sys_fd_t            server_fd;
    ioloop_t*           ioloop;
} vbusd_clients_t;

typedef struct vbusd_client {
    vbusd_clients_t*    clients;
    sys_fd_t            fd;
    vbusd_cl_type_t     type;
    vbusd_client_t*     next;
} vbusd_client_t;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void vbusd_close_conn (vbusd_client_t*  client);

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
            strnlen_s(options->serial_device, sizeof(options->serial_device)) < 2) {
            fprintf(stderr, "Invalid serial device path!\n");
            break;
        }
        if (options->vbusd_address_set &&
            strnlen_s(options->vbusd_address, sizeof(options->vbusd_address)) < 2) {
            fprintf(stderr, "Invalid vbusd address!\n");
            break;
        }
        rc = true;
    } while (0);
    return rc;
}

static void print_usage (void)
{
    printf("\nvbusd - Virtual Bus Deamon\n");
    printf("\nUsage:\n");
    printf("vbusd [-d <device>] [-b <baudrate>] [-v <vbusd address>] [-w <vbusd port>]\n\n");
    printf("Arguments:\n");
    printf(" -d <device>         Device of serial bus connection.\n");
    printf(" -b <baudrate>       Baudrate of serial bus connection. Default: 38400\n");
    printf(" -v <vbusd address>  Address of vbusd. Default: /tmp/vbusd.usk\n");
    printf(" -w <vbusd port>     Port number of vbusd. Default: 0\n");
}

static vbusd_client_t* new_client (vbusd_clients_t* clients)
{
    vbusd_client_t* client;

    do {
        if ((client = (vbusd_client_t*)calloc(1, sizeof(vbusd_client_t))) == NULL) {
            break;
        }

        client->clients = clients;
        client->next = clients->first_client;
        clients->first_client = client;
    } while (0);
    return client;
}

static void delete_client (vbusd_client_t* client)
{
    vbusd_clients_t *clients = client->clients;
    vbusd_client_t  *current, *previous;

    current = clients->first_client;
    previous = current;
    while (current != NULL) {
        if (current == client) {
            if (current == clients->first_client) {
                clients->first_client = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            break;
        }
        previous = current;
        current = current->next;
    }
}

static int32_t byte_received (void* arg)
{
    vbusd_client_t  *client, *current_client = (vbusd_client_t*)arg;
    vbusd_clients_t *clients = current_client->clients;
    uint8_t         bytes[32];
    int             rc = eERR_NONE, len = 1;

    // read bytes
    switch (current_client->type) {
    case eVBUSD_TYPE_SERIAL:
        if (sys_serial_recv(current_client->fd, bytes, len) < 0) {
            return 0;
        }
        break;
    case eVBUSD_TYPE_SOCKET:
        if ((rc = sys_socket_recv(current_client->fd, bytes, len)) != len) {
            // connection closed?
            if (rc == 0) vbusd_close_conn(current_client);
            return 0;
        }
        break;
    default:
        break;
    }

    // distribute bytes to clients
    client = clients->first_client;
    while (client != NULL) {
        if (client != current_client) {
            switch (client->type) {
            case eVBUSD_TYPE_SERIAL:
                sys_serial_send(client->fd, bytes, len);
                break;
            case eVBUSD_TYPE_SOCKET:
                sys_socket_send(client->fd, bytes, len);
                break;
            default:
                break;
            }
        }
        client = client->next;
    }
    return 0;
}

static int32_t accept_client (void* arg)
{
    vbusd_clients_t*    clients = (vbusd_clients_t*)arg;
    vbusd_client_t*     client;
    sys_fd_t            fd;

    do {
        if ((client = new_client(clients)) == NULL) {
            break;
        }

        if ((fd = sys_socket_accept(clients->server_fd)) <= INVALID_FD) {
            delete_client(client);
            break;
        }

        client->fd = fd;
        client->type = eVBUSD_TYPE_SOCKET;
        ioloop_register_fd(clients->ioloop, fd, eIOLOOP_EV_READ, byte_received, client);
        fprintf(stderr, "vbusd socket connection accepted.\n");
    } while (0);
    return 0;
}

static void vbusd_init (vbusd_clients_t*  clients, ioloop_t* ioloop)
{
    assert(clients != NULL);
    assert(ioloop != NULL);

    memset(clients, 0, sizeof(vbusd_clients_t));
    clients->ioloop = ioloop;
}

static int vbusd_open_server (vbusd_clients_t*  clients,
                              const char*       address,
                              uint16_t          port)
{
    int             rc = eERR_NONE;
    sys_fd_t        fd;

    do {
        if (port == 0) {
            if ((fd = sys_socket_open_server_unix(address)) <= INVALID_FD) {
                rc = eSYS_ERR_SYSTEM;
                break;
            }
        } else {
            // TODO insert tcp code here
        }
        clients->server_fd = fd;
        ioloop_register_fd(clients->ioloop, fd, eIOLOOP_EV_READ, accept_client, clients);
        fprintf(stderr, "vbusd socket server listening.\n");
    } while (0);
    return rc;
}

static int vbusd_open_serial (vbusd_clients_t*  clients,
                              ioloop_t*         ioloop,
                              const char*       device,
                              int               baudrate)
{
    int             rc = eERR_NONE;
    sys_fd_t        fd;
    vbusd_client_t* client;

    do {
        if ((client = new_client(clients)) == NULL) {
            rc = eSYS_ERR_MALLOC;
            break;
        }
        if ((fd = sys_serial_open(device)) <= INVALID_FD) {
            delete_client(client);
            rc = eSYS_ERR_SYSTEM;
            break;
        }

        if ((rc = sys_serial_set_params(fd, sys_serial_baudrate(baudrate),
                                        eSYS_SER_DB_8,
                                        eSYS_SER_P_NONE,
                                        eSYS_SER_SB_1)) != eERR_NONE) {
            sys_serial_close(fd);
            delete_client(client);
            break;
        }
        client->fd = fd;
        client->type = eVBUSD_TYPE_SERIAL;

        ioloop_register_fd(ioloop, fd, eIOLOOP_EV_READ, byte_received, clients);
        fprintf(stderr, "vbusd serial connection opened.\n");
    } while (0);
    return rc;
}

static void vbusd_close_conn (vbusd_client_t*  client)
{
    if (client->fd > INVALID_FD) {
        ioloop_unregister_fd(client->clients->ioloop, client->fd);
        switch (client->type) {
        case eVBUSD_TYPE_SERIAL:
            sys_serial_close(client->fd);
            fprintf(stderr, "vbusd serial connection closed.\n");
            break;
        case eVBUSD_TYPE_SOCKET:
            sys_socket_close(client->fd);
            fprintf(stderr, "vbusd socket connection closed.\n");
            break;
        default:
            break;
        }
    }
    delete_client(client);
}

static void vbusd_close_all (vbusd_clients_t* clients)
{
    vbusd_client_t* client;

    // close all client connections
    client = clients->first_client;
    while (client != NULL) {
        vbusd_close_conn(client);
        client = clients->first_client;
    }

    // close socket server_fd
    if (clients->server_fd > INVALID_FD) {
        ioloop_unregister_fd(clients->ioloop, clients->server_fd);
        sys_socket_close(clients->server_fd);
    }
    clients->server_fd = INVALID_FD;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;
    bool            end_application = false;
    options_t       options;
    ioloop_t        mainloop;
    vbusd_clients_t clients;

    do {
        printf("\nvbusd...\n");

        // set default options for kwbrouter
        set_options(&options,
                    "",                 // no serial device, use vbusd as default
                    38400,              // baudrate, if not given
                    "/tmp/vbusd.usk",   // default address of vbusd socket
                    0);                 // port 0: use unix sockets

        // parse and validate commandline options
        if (parse_commandline_options(argc, argv, &options) == false ||
            validate_options(&options) == false) {
            print_usage();
            rc = eSYS_ERR_BAD_PARAMETER;
            break;
        }

        ioloop_init (&mainloop);
        vbusd_init (&clients, &mainloop);

        if ((rc = vbusd_open_server(&clients,
                                    options.vbusd_address,
                                    options.vbusd_port)) != eERR_NONE) {
            break;
        }

        if (options.serial_device_set) {
            if ((rc = vbusd_open_serial(&clients,
                                        &mainloop,
                                        options.serial_device,
                                        options.serial_baudrate)) != eERR_NONE) {
                break;
            }
        }

        printf("entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);
        }
    } while (0);

    vbusd_close_all(&clients);

    return rc;
}

/** @} */
