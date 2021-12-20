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
    /**
     * Initialize the socket server.
     *
     * @param[in]   iol         Pointer to ioloop. Needed to connect a
     *                          /refMESSAGE_SOCKET server-socket to
     *                          /refIOLOOP background io loop.
     * @param[in]   r           Pointer to router, which will be informed about a
     *                          new connection to a client connected to the server.
     *                          The client will be added automatically to the routing list.
     */
    SocketServer(ioloop_t *iol, Router *r);

    /**
     * When a socket server is destroyed, close all remaining open connections
     * before.
     */
    ~SocketServer();

    /**
     * Opens a new listening socket for either unix sockets or tcp sockets. If the
     * port number is 0, a UNIX socket of name address will be opened otherwise a
     * TCP port.
     *
     * @param[in]   address     Name of unix socket or address of tcp socket.
     * @param[in]   port        Set to 0 for UNIX sockets, port number for TCP ports.
     *
     * @returns eERR_None, if successful otherwise error code of #gen_errors_t.
     */
    void open(const char *address, uint16_t port);

    /**
     * Closes the server. Before all remaining client connections will be closed.
     */
    void close();

    /**
     * Called when a new connection has been established.
     *
     * @param[in]   connection  Pointer to established connection.
     */
    void onNewConnection(ConnectionSocket *connection);

    /**
     * Call this function, when a client connection closes, so the client
     * can be removed from client list.
     *
     * @param[in]   connection  Pointer to established connection.
     */
    void onCloseConnection(ConnectionSocket *connection);

protected:
    /**
     * Default constructor.
     */
    SocketServer();

    /**
     * Callback function which will be called to accept incoming connections.
     *
     * @param[in]   reference   Callback reference to the server object.
     *
     * @return (unused)Always returs 0 to fulfill ioloops callback sibnature.
     */
    static int acceptConnection(void *reference);

    /**
     * Callback function which will be called when a connection is closed remotely.
     *
     * @param[in]   uri         URI of closed connection
     * @param[in]   reference   Callback reference to the server object.
     */
    void lostConnection(const std::string &uri, void *reference);

    std::list<ConnectionSocket *> clients;  ///< list of connected clients
    ioloop_t                   *ioloop;     ///< reference to ioloop
    Router                     *router;     ///< reference to router
    sys_fd_t                    fd;         ///< Server file descriptor
};

#endif // SOCKETSERVER_H
/** @} */
