/*
 * kwbkouter - A router for koewiba-domotic messages.
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
 *
 */

#include "log.h"

#include "rconnsocketclient.h"

// --- Local functions ---------------------------------------------------------

static void incommingMessageHdl(msg_t* message, void* reference, void* arg)
{
    RConnSocketClient* sockcon = (RConnSocketClient*)arg;
    sockcon->OnIncommingMessage(message);
}

static void closeConnectionHdl(const char* address, uint16_t port, void* reference, void* arg)
{
    RConnSocketClient* sockcon = (RConnSocketClient*)arg;
    sockcon->OnConnectionClosed();
}

// --- Class member functions --------------------------------------------------

RConnSocketClient::RConnSocketClient() : ep(NULL),
                                         socket(NULL)
{
    msg_s_init(&this->local_socket);
    this->socket = &this->local_socket;
}

RConnSocketClient::RConnSocketClient(msg_socket_t* server, msg_endpoint_t* ep, const char* address, uint16_t port)
{
    this->socket = server;
    this->ep = ep;
    this->ioloop = server->ioloop;
    msg_s_set_incomming_handler(this->socket, incommingMessageHdl, this);
    msg_s_set_closeconnection_handler(ep, closeConnectionHdl, this);
    log_msg(LOG_STATUS, "New socket connection %s:%d", address, port);
}

RConnSocketClient::~RConnSocketClient()
{
    this->Close();
}

int RConnSocketClient::Open(const char* address, int port)
{
    int retval;

    this->socket = &this->local_socket;
    msg_s_set_incomming_handler(this->socket, incommingMessageHdl, this);
    retval = msg_s_open_client(this->socket, ioloop, address, port);
    if (retval == eERR_NONE) {
        this->ep = this->socket->first_ep; // clients only have one endpoint
        msg_s_set_closeconnection_handler(this->ep, closeConnectionHdl, this);
    }
    return retval;
}

void RConnSocketClient::Close()
{
    if (this->socket != NULL && this->ep != NULL) {
        msg_s_close_connection(this->socket, this->ep);
        this->ep = NULL;
    }
}

int RConnSocketClient::Send(msg_t* message)
{
    return msg_s_send(ep, message);
}

void RConnSocketClient::OnConnectionClosed()
{
    this->ep = NULL;
    RouteConnection::OnConnectionClosed();
}

