/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    rconnsocketclient.h
 * @brief   Declaration of a route over a socket connection.
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
#ifndef RCONNSOCKETCLIENT_H
#define RCONNSOCKETCLIENT_H

#include "message_socket.h"

#include "routeconnection.h"

/**
 * This class implements a route for KWB messages over a socket
 * connection.
 */
class RConnSocketClient : public RouteConnection
{
private:
    ioloop_t*       ioloop;         //!< Store reference to ioloop.
    msg_endpoint_t* ep;             //!< Stores information about established socket connection.
    msg_socket_t*   socket;         //!< Stores information about public socket server.
    msg_socket_t    local_socket;   //!< Class local storage about socket server, if not 
                                    //!< given by constructor.

public:
    RConnSocketClient();
    RConnSocketClient(ioloop_t* ioloop, const char* address, uint16_t port);
    RConnSocketClient(msg_socket_t* server, msg_endpoint_t* ep, const char* address, uint16_t port);
    ~RConnSocketClient();

    int Open(const char* address, int port);
    void Close();

    int Send(msg_t* message);
    void OnConnectionClosed();
};

#endif // RCONNSOCKETCLIENT_H
/** @} */
