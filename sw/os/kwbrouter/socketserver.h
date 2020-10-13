/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    socketserver.h
 * @brief   Wrapper class to provide a server for UNIX sockets or TCP sockets 
 *          using the /ref MESSAGE_SOCKET module.
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
#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <list>

#include "connection_socket.h"
#include "router.h"

/**
 * Uses /refIOLOOP and /refMESSAGE_SOCKET to open and maintain a socket server
 * for Unix-Sockets and TCP-Sockets to which clients can connect to.
 * The socket server is not thread-safe and ment to be used in a single-threaded
 * mainloop.
 */
class SocketServer
{
public:
    SocketServer(ioloop_t* iol, Router* r);
    ~SocketServer();

    void open(const char* address, uint16_t port);
    void close();

    void onNewConnection(ConnectionSocket *connection);
    void onCloseConnection(ConnectionSocket *connection);

protected:
    SocketServer();
    static int acceptConnection(void *arg);
    void lostConnection(const std::string & uri, void *reference);

    std::list<ConnectionSocket*> clients;   ///< list of connected clients
    ioloop_t*                   ioloop;     ///< reference to ioloop
    Router*                     router;     ///< reference to router
    sys_fd_t                    fd;         ///< Server file descriptor
};

#endif // SOCKETSERVER_H
/** @} */
