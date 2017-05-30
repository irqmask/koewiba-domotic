/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <list>

#include "message_socket.h"

#include "rconnsocketclient.h"
#include "router.h"

class SocketServer
{
private:
    std::list<RConnSocketClient*> clients;
    ioloop_t*                   ioloop;
    Router*                     router;
    msg_socket_t                server;

public:
    SocketServer();
    SocketServer(ioloop_t* iol);
    SocketServer(ioloop_t* iol, Router* r);
    ~SocketServer();

    int Open(const char* address, uint16_t port);
    void Close();

    void OnNewConnection(msg_endpoint_t* endpoint);
    void OnCloseConnection(RConnSocketClient* client);
};

#endif // SOCKETSERVER_H
