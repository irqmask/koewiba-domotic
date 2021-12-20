/* OBSOLETE, DO NOT USE FOR FUTURE PROJECTS, REPLACE IN EXISTING PROJECTS WITH
 * connection_socket.
 *
 * @addtogroup MESSAGE_SOCKET
 * @brief Transmit bus-messages between KWB applications via unix or inet
 * sockets.
 *
 * Bus messages are transmitted or received via unix or inet sockets between
 * applications or over the network.
 *
 * @{
 * @file    message_socket.h
 * @brief   Transmit bus-messages between KWB applications.
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
#ifndef _MESSAGE_SOCKET_H_
#define _MESSAGE_SOCKET_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#include "ioloop.h"
#include "message.h"
#include "syssocket.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct msg_endpoint msg_endpoint_t;

//! Structure holding all information for a socket connection for kwb messages.
typedef struct msg_socket {
    sys_fd_t            well_known_fd;  //!< Listening server's file descriptor
    msg_endpoint_t     *first_ep;       //!< head of endpoint list
    ioloop_t           *ioloop;         //!< pointer to ioloop
    char                address[256];   //!< address of remote server or path to
    //!< unix socket file
    uint16_t            port;           //!< server port (0 for unix socket server)
    msg_conn_func_t     new_connection_handler; //!< called whenever a new client
    //!< connected to the server
    void               *new_connection_arg;     //!< additional argument for
    //!< new_connection_handler callback
    msg_incom_func_t    incomming_handler;      //!< called whenever a message
    //!< is received.
    void               *incomming_arg;          //!< additional argument for
    //!< incoming handler callback.
} msg_socket_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_s_init(msg_socket_t *msg_router);

int msg_s_open_server(msg_socket_t   *msg_socket,
                      ioloop_t       *ioloop,
                      const char     *address,
                      uint16_t        port);

int msg_s_open_client(msg_socket_t   *msg_socket,
                      ioloop_t       *ioloop,
                      const char     *address,
                      uint16_t        port);

void msg_s_close_connection(msg_socket_t *msg_socket,
                            msg_endpoint_t *ep);

void msg_s_set_newconnection_handler(msg_socket_t *msg_socket,
                                     msg_conn_func_t func,
                                     void *arg);

void msg_s_set_closeconnection_handler(msg_endpoint_t *ep,
                                       msg_conn_func_t func,
                                       void *arg);

void msg_s_set_incomming_handler(msg_socket_t *msg_socket,
                                 msg_incom_func_t func,
                                 void *arg);

msg_endpoint_t *msg_s_get_endpoint(msg_socket_t   *msg_socket,
                                   int             index);

int msg_s_send(msg_endpoint_t    *recv_ep,
               msg_t             *message);

#ifdef __cplusplus
}
#endif

#endif /* _MESSAGE_SOCKET_H_ */
/** @} */
