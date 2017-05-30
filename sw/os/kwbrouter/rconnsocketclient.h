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

#ifndef RCONNSOCKETCLIENT_H
#define RCONNSOCKETCLIENT_H

#include "message_socket.h"

#include "routeconnection.h"

class RConnSocketClient : public RouteConnection
{
private:
    ioloop_t*       ioloop;
    msg_endpoint_t* ep;
    msg_socket_t*   socket;
    msg_socket_t    local_socket;


public:
    RConnSocketClient();
    RConnSocketClient(msg_socket_t* server, msg_endpoint_t* ep, const char* address, uint16_t port);
    ~RConnSocketClient();

    int Open(const char* address, int port);
    void Close();

    int Send(msg_t* message);
    void OnMessageReceived(msg_t* message);
    void OnConnectionClosed();
};

#endif // RCONNSOCKETCLIENT_H
