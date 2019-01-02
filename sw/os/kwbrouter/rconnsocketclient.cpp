/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    rconnsocketclient.cpp
 * @brief   Implementation of a route over a socket connection.
 *
 * This implements opening and closing as well as sending and receiving messages
 * over a socket connection. It connects to a socket server.
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

#include <stdio.h>
#include <string.h>

// include 
#include "kwb_defines.h"
// shared
#include "log.h"

#include "rconnsocketclient.h"

// --- Local functions ---------------------------------------------------------

// will be called from /ref MESSAGE_SOCKET when a complete message has been
// received and is passed to the common OnIncommingMessage() handler for all
// type of connections.
static void incommingMessageHdl(msg_t* message, void* reference, void* arg)
{
    RConnSocketClient* sockcon = (RConnSocketClient*)arg;
    sockcon->OnIncommingMessage(message);
}

// will be called from /ref MESSAGE_SOCKET when a connection is closed. 
static void closeConnectionHdl(const char* address, uint16_t port, void* reference, void* arg)
{
    RConnSocketClient* sockcon = (RConnSocketClient*)arg;
    sockcon->OnConnectionClosed();
}

// --- Class member functions --------------------------------------------------

/**
 * @todo consolidate constructors into one with default values.
 */
RConnSocketClient::RConnSocketClient() : ep(NULL),
                                         socket(NULL)
{
    msg_s_init(&this->local_socket);
    this->socket = &this->local_socket;
    memset(this->name, 0, sizeof(name));
    snprintf(this->name, sizeof(name-1), "USOCK");
}

/**
 * Initializes a socket connection which has been accepted by the socket server.
 *
 * @param[in]   server      Pointer to the server.
 * @param[in]   ep          Endpoint of established socket-connection.
 * @todo remove address and port from interface. It is only used for logging purposes. Let server do it.
 */
RConnSocketClient::RConnSocketClient(msg_socket_t* server, msg_endpoint_t* ep, const char* address, uint16_t port)
{
    this->socket = server;
    this->ep = ep;
    this->ioloop = server->ioloop;
    memset(this->name, 0, sizeof(name));
    snprintf(this->name, sizeof(name-1), "USOCK");
    msg_s_set_incomming_handler(this->socket, incommingMessageHdl, this);
    msg_s_set_closeconnection_handler(ep, closeConnectionHdl, this);
    log_msg(LOG_STATUS, "%s New client socket connection %s:%d", this->name, address, port);
}

/**
 * The destructor closes the socket connection if still open.
 */
RConnSocketClient::~RConnSocketClient()
{
    this->Close();
}

/** 
 * Open a socket connection either to a UNIX socket or to a TCP socket.
 *
 * @param[in]   address     Path and name to unix-socket ot TCP/IP address.
 * @param[in]   port        0 for unix-socket or well-known TCP/IP port number.
 *
 * @returns 0 if successfull, otherwise error-code.
 */
int RConnSocketClient::Open(const char* address, int port)
{
    int retval;

    this->socket = &this->local_socket;
    msg_s_set_incomming_handler(this->socket, incommingMessageHdl, this);
    retval = msg_s_open_client(this->socket, ioloop, address, port);
    if (retval == eERR_NONE) {
        this->ep = this->socket->first_ep; // clients only have one endpoint
        msg_s_set_closeconnection_handler(this->ep, closeConnectionHdl, this);
        log_msg(KWB_LOG_STATUS, "%s open connection to %s:%d", this->GetName(), address, port);
    } else {
        log_error("%s opening connection to %s:%d failed", this->GetName(), address, port);
    }
    
    return retval;
}

/**
 * Actively close an established socket connection.
 */
void RConnSocketClient::Close()
{
    if (this->socket != NULL && this->ep != NULL) {
        msg_s_close_connection(this->socket, this->ep);
        this->ep = NULL;
    }
    log_msg(KWB_LOG_STATUS, "%s close connection", this->GetName());
}

/**
 * Send a KWB message over established socket connection.
 *
 * @param[in]   message     Message to be sent.
 *
 * @returns 0 if successfull, otherwise error-code.
 */
int RConnSocketClient::Send(msg_t* message)
{
    log_msg(LOG_VERBOSE1, "%6s <-- message sent", this->GetName());
    msg_log("SOCKETSEND", *message);
    return msg_s_send(ep, message);
}

/**
 * Called by external "OnClose" event
 */
void RConnSocketClient::OnConnectionClosed()
{
    this->ep = NULL;
    RouteConnection::OnConnectionClosed();
}

/** @} */