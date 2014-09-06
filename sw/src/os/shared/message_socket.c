/**
 * @addtogroup MESSAGE_SOCKET
 *
 * @{
 * @file    message_socket.c
 * @brief   Transmit bus-messages between KWB applications via unix or inet
 * sockets.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "ioloop.h"
#include "message_socket.h"
#include "syssocket.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct msg_endpoint {
    msg_endpoint_t* next;
    msg_ep_type_t   type;
    msg_socket_t*   msg_socket;
    sys_fd_t        fd;
} msg_endpoint_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static msg_endpoint_t* msg_new_endpoint (msg_socket_t* msg_socket,
                                         msg_ep_type_t type)
{
    msg_endpoint_t* ep = NULL;

    do {
        ep = (msg_endpoint_t*)calloc(1, sizeof(msg_endpoint_t));
        if (ep == NULL) {
            perror("new message transport endpoint could not be created");
            break;
        }

        ep->fd = INVALID_FD;
        ep->msg_socket = msg_socket;
        ep->type = type;

        // insert new endpoint at the beginning of the list of endpoints
        ep->next = msg_socket->first_ep;
        msg_socket->first_ep = ep;
    } while (0);

    return ep;
}

static void msg_delete_endpoint (msg_socket_t* msg_socket, msg_endpoint_t* ep_to_delete)
{
    msg_endpoint_t* ep;

    ep = msg_socket->first_ep;

    if (ep == NULL) return;
    if (ep == ep_to_delete) {
        msg_socket->first_ep = ep->next;
        free(ep);
    } else {
        while (ep->next != NULL && ep->next != ep_to_delete) {
            ep = ep->next;
        }
        if (ep->next != NULL && ep->next == ep_to_delete) {
            ep->next = ep_to_delete->next;
            free(ep_to_delete);
        }
    }
}

static void msg_read (void* arg)
{
    int             rc;
    msg_endpoint_t* ep = (msg_endpoint_t*)arg;
    msg_socket_t*   msg_socket = ep->msg_socket;
    msg_t           message;

    // reveive message
    rc = sys_socket_recv(ep->fd, &message, sizeof(message));
    if (rc == 0) {
        // connection closed
        msg_s_close_connection(msg_socket, ep);
        fprintf(stderr, "connection closed\n");
        return;
    }

    if (rc != sizeof(message)) {
        perror("invaid size of incomming message");
        return;
    }

    // handle message
    if (msg_socket->incomming_handler != NULL) {
        msg_socket->incomming_handler(&message, msg_socket->incomming_arg);
    }
}

static void msg_accept_endpoint (void* arg)
{
    msg_socket_t*   msg_socket = (msg_socket_t*)arg;
    msg_endpoint_t* ep = NULL;

    do {
        ep = msg_new_endpoint(msg_socket, eMSG_EP_COMM);
        if (ep == NULL) break;

        ep->fd = sys_socket_accept(msg_socket->well_known_fd);
        if (ep->fd <= INVALID_FD) {
            perror("server not accepting new endpoint");
            msg_delete_endpoint(msg_socket, ep);
            break;
        }

        // register new connection to ioloop
        ioloop_register_fd(msg_socket->ioloop, ep->fd, eIOLOOP_EV_READ, msg_read, (void*)ep);

        fprintf(stderr, "connection accepted\n");

    } while (0);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_s_init (msg_socket_t* msg_socket)
{
    assert(msg_socket != NULL);

    memset(msg_socket, 0, sizeof(msg_socket_t));
}

int msg_s_open_server (msg_socket_t*   msg_socket,
                       ioloop_t*       ioloop,
                       const char*     address,
                       uint16_t        port)
{
    int             rc = eERR_NONE;
    sys_fd_t        fd;

    do {
        assert(msg_socket != NULL);
        assert(ioloop != NULL);
        assert(address != NULL);

        msg_socket->ioloop = ioloop;

        strncpy_s(msg_socket->address,
                  sizeof(msg_socket->address),
                  address,
                  strlen(address));

        if (port == 0) {
            fd = sys_socket_open_server_unix(msg_socket->address);
            if (fd <= INVALID_FD) {
                rc = eSYS_ERR_SYSTEM;
                break;
            }
            sys_socket_set_blocking(fd, false);
        } else {
            rc = eERR_NOT_IMPLEMENTED;
            // TODO: insert tcp server code
            break;
        }

        msg_socket->well_known_fd = fd;
        ioloop_register_fd(ioloop, fd, eIOLOOP_EV_READ, msg_accept_endpoint, (void*)msg_socket);
    } while (0);
    return rc;
}

int msg_s_open_client (msg_socket_t*   msg_socket,
                       ioloop_t*       ioloop,
                       const char*     address,
                       uint16_t        port)
{
    int             rc = eERR_NONE;
    sys_fd_t        fd;
    msg_endpoint_t* ep = NULL;

    do {
        assert(msg_socket != NULL);
        assert(ioloop != NULL);
        assert(address != NULL);

        msg_socket->ioloop = ioloop;

        strncpy_s(msg_socket->address,
                  sizeof(msg_socket->address),
                  address,
                  strlen(address));

        if (port == 0) {
            fd = sys_socket_open_client_unix(msg_socket->address);
            if (fd <= INVALID_FD) {
                rc = eSYS_ERR_SYSTEM;
                break;
            }
            sys_socket_set_blocking(fd, false);
        } else {
            rc = eERR_NOT_IMPLEMENTED;
            // TODO: insert tcp client code
            break;
        }

        ep = msg_new_endpoint(msg_socket, eMSG_EP_COMM);
        if (ep == NULL) break;
        ep->fd = fd;
        ep->msg_socket = msg_socket;

        ioloop_register_fd(ioloop, fd, eIOLOOP_EV_READ, msg_read, (void*)ep);
    } while (0);
    return rc;
}

void msg_s_close_connection (msg_socket_t* msg_socket, msg_endpoint_t* ep)
{
    assert(msg_socket != NULL);
    assert(ep != NULL);

    ioloop_unregister_fd(msg_socket->ioloop, ep->fd);
    sys_socket_close (ep->fd);
    msg_delete_endpoint(msg_socket, ep);
}

void msg_s_set_incomming_handler (msg_socket_t* msg_socket, msg_incom_func_t func, void* arg)
{
    assert(msg_socket != NULL);

    msg_socket->incomming_handler = func;
    msg_socket->incomming_arg = arg;
}

msg_endpoint_t* msg_s_get_endpoint (msg_socket_t*   msg_socket,
                                    int             index,
                                    uint32_t        flags)
{
    msg_endpoint_t* ep = NULL;
    int             found_ep = 0;

    assert (msg_socket != NULL);

    do {
        ep = msg_socket->first_ep;
        if (flags == 0) flags = 0xFFFFFFFF;

        while (ep != NULL) {
            if ((1<<ep->type) & flags) {
                if (index == found_ep) break;
                found_ep++;
            }
            ep = ep->next;
        }
    } while (0);

    return ep;
}

void msg_s_send (msg_endpoint_t* recv_ep, msg_t* message)
{
    assert(recv_ep != NULL);
    assert(message != NULL);

    sys_socket_send(recv_ep->fd, (void*)message, sizeof(msg_t));
}

/** @} */