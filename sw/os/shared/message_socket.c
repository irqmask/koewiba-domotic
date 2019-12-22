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

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "ioloop.h"
#include "log.h"
#include "message_socket.h"
#include "syssocket.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Endpoint of established socket connection.
 * An endpoint structure stores all the information about an established 
 * point-to-point connection. Endpoints are stored in a linked list.
 */
typedef struct msg_endpoint {
    msg_endpoint_t* next;           //!< Next element in linked list.
    msg_ep_type_t   type;           //!< Connection-type. See #msg_ep_type_t.
    msg_socket_t*   msg_socket;     //!< Information about public socket server.
    sys_fd_t        fd;             //!< Handle to established connection.
    //! handler for closed connections, called when connection is closed.
    msg_conn_func_t close_connection_handler;   
    //! Optional argument, conveyed to the closed connection handler, when called.
    void*           close_connection_arg;
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
            log_sys_error("new message transport endpoint could not be created");
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

static int32_t msg_read (void* arg)
{
    int             rc;
    msg_endpoint_t* ep = (msg_endpoint_t*)arg;
    msg_socket_t*   msg_socket = ep->msg_socket;
    msg_t           message;

    do {
        // reveive message
        rc = sys_socket_recv(ep->fd, &message, sizeof(message));
        if (rc == 0) {
            // connection closed
            msg_s_close_connection(msg_socket, ep);
            log_info("connection closed");
            break;
        }

        if (rc != sizeof(message)) {
            log_sys_error("invaid size of incomming message");
            break;
        }

        // handle message
        if (msg_socket->incomming_handler != NULL) {
            msg_socket->incomming_handler(&message, ep, msg_socket->incomming_arg);
        }
    } while (0);
    return 0;
}

static int32_t msg_accept_endpoint (void* arg)
{
    msg_socket_t*   msg_socket = (msg_socket_t*)arg;
    msg_endpoint_t* ep = NULL;
    char address[256];
    uint16_t port = 0;

    do {
        ep = msg_new_endpoint(msg_socket, eMSG_EP_COMM);
        if (ep == NULL) break;

        // get file descriptor of new client connection
        ep->fd = sys_socket_accept(msg_socket->well_known_fd);
        if (ep->fd <= INVALID_FD) {
            log_sys_error("server not accepting new endpoint");
            msg_delete_endpoint(msg_socket, ep);
            break;
        }

        // get address and port of accepted connection and pass it to appications
        // new connection handler.
        sys_socket_get_name(ep->fd, address, sizeof(address), &port);
        if (msg_socket->new_connection_handler != NULL) {
            msg_socket->new_connection_handler(address, port, ep, msg_socket->new_connection_arg);
        }

        // register new connection to ioloop
        ioloop_register_fd(msg_socket->ioloop, ep->fd, eIOLOOP_EV_READ, msg_read, (void*)ep);

        log_info("connection accepted from %s:%d\n", address, port);
    } while (0);
    return 0;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_s_init (msg_socket_t* msg_socket)
{
    assert(msg_socket != NULL);

    memset(msg_socket, 0, sizeof(msg_socket_t));
    msg_socket->well_known_fd = INVALID_FD;
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

        if (port == 0) {
            if (address == NULL) {
                log_error("unix socket server address not set!");
                rc = eERR_BAD_PARAMETER;
                break;
            }
            strncpy_s(msg_socket->address,
                      sizeof(msg_socket->address),
                      address,
                      strlen(address));
            fd = sys_socket_open_server_unix(msg_socket->address);
            if (fd <= INVALID_FD) {
                log_sys_error("unable to open unix socket server at path=%s", address);
                rc = eERR_SYSTEM;
                break;
            }
        } else {
            fd = sys_socket_open_server_tcp(msg_socket->port);
            if (fd <= INVALID_FD) {
                log_sys_error("unable to open tcp socket server at port=%d", port);
                rc = eERR_SYSTEM;
                break;
            }
        }
        sys_socket_set_blocking(fd, false);
        msg_socket->ioloop = ioloop;
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

        strncpy_s(msg_socket->address,
                  sizeof(msg_socket->address),
                  address,
                  strlen(address));
        msg_socket->port = port;

        if (port == 0) {
            fd = sys_socket_open_client_unix(msg_socket->address);
            if (fd <= INVALID_FD) {
                log_sys_error("unable to connect client to unix socket server! address=%s",
                              msg_socket->address);
                rc = eERR_SYSTEM;
                break;
            }
        } else {
            fd = sys_socket_open_client_tcp(msg_socket->address, msg_socket->port);
            if (fd <= INVALID_FD) {
                log_sys_error("unable to connect client to tcp socket server! address=%s port=%d",
                              msg_socket->address, msg_socket->port);
                rc = eERR_SYSTEM;
                break;
            }
        }

        msg_socket->ioloop = ioloop;
        sys_socket_set_blocking(fd, false);
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

    if (ep->fd != INVALID_FD) {
        ioloop_unregister_fd(msg_socket->ioloop, ep->fd, eIOLOOP_EV_UNKNOWN);
        sys_socket_close (ep->fd);
        ep->fd = INVALID_FD;
        if (ep->close_connection_handler != NULL) {
            ep->close_connection_handler(msg_socket->address,
                                        msg_socket->port,
                                        ep,
                                        ep->close_connection_arg);
        }
    }
    msg_delete_endpoint(msg_socket, ep);
}

void msg_s_set_newconnection_handler (msg_socket_t* msg_socket, msg_conn_func_t func, void* arg)
{
    assert(msg_socket != NULL);

    msg_socket->new_connection_handler = func;
    msg_socket->new_connection_arg = arg;
}

void msg_s_set_closeconnection_handler (msg_endpoint_t* ep, msg_conn_func_t func, void* arg)
{
    assert(ep != NULL);

    ep->close_connection_handler = func;
    ep->close_connection_arg = arg;
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

int msg_s_send (msg_endpoint_t* recv_ep, msg_t* message)
{
    assert(recv_ep != NULL);
    assert(message != NULL);

    sys_socket_send(recv_ep->fd, (void*)message, sizeof(msg_t));

    return eERR_NONE;
}

/** @} */
