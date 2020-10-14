/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    socketserver.cpp
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

#include <sstream>
#include <string>

// libkwb
#include "exceptions.h"
#include "log.h"

// libsystem
#include "syssocket.h"

#include "socketserver.h"

// --- Local functions ---------------------------------------------------------

// callback from socket-server when a connection has been closed.
static void close_connection(const char *address, uint16_t port, void *reference, void *arg)
{
    SocketServer *server = static_cast<SocketServer *>(arg);
    ConnectionSocket *client = static_cast<ConnectionSocket *>(reference);
    server->onCloseConnection(client);
}

// --- Class member functions --------------------------------------------------

/**
 * @todo use default paparmeters in full constructor instead of three constructors!
 */
SocketServer::SocketServer()
    : router(nullptr)
    , ioloop(nullptr)
    , fd(INVALID_FD)
{
    clients.clear();
}

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
SocketServer::SocketServer(ioloop_t *iol, Router *r)
{
    SocketServer();
    this->ioloop = iol;
    this->router = r;
}

/**
 * When a socket server is destroyed, close all remaining open connections
 * before.
 */
SocketServer::~SocketServer()
{
    this->close();
}

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
void SocketServer::open(const char *address, uint16_t port)
{
    int retval;

    if (port == 0) {
        this->fd = sys_socket_open_server_unix(address);
    }
    else {
        this->fd = sys_socket_open_server_tcp(port);
    }

    if (this->fd == INVALID_FD) {
        log_sys_error("SERVER Unable to open tcp socket server at port=%d", port);
        throw OperationFailed(LOC, "SERVER Unable to open tcp socket server at port=%d", port);
    }

    if (this->ioloop != nullptr) {
        ioloop_register_fd(this->ioloop, this->fd, eIOLOOP_EV_READ, SocketServer::acceptConnection, this);
    }
}

/**
 * Closes the server. Before all remaining client connections will be closed.
 */
void SocketServer::close()
{
    for (auto client : clients) {
        if (this->router != NULL) {
            this->router->removeConnection(client);
        }
        delete (client);
    }
    clients.clear();
}

/**
 * Called when a new connection has been established.
 * @param[in]   endpoint    Pointer to established connection.
 */
void SocketServer::onNewConnection(ConnectionSocket *connection)
{
    clients.push_back(connection);
    if (this->router != nullptr) {
        this->router->addConnection(connection);
    }
}

/**
 * Call this function, when a client connection closes, so the client
 * can be removed from client list.
 */
void SocketServer::onCloseConnection(ConnectionSocket *connection)
{
    if (this->router != nullptr) {
        this->router->removeConnection(connection);
    }
    clients.remove(connection);
    delete connection;
}

int SocketServer::acceptConnection(void *arg)
{
    SocketServer *server = static_cast<SocketServer *>(arg);
    char address[256];
    uint16_t port;
    sys_fd_t newFd;

    // get file descriptor of new client connection
    newFd = sys_socket_accept(server->fd, address, sizeof(address), &port);
    if (newFd <= INVALID_FD) {
        log_sys_error("SOCKET Server not accepting new endpoint");
        return 0;
    }

    std::stringstream uriss;
    uriss << address << ":" << port;

    // create new connection
    ConnectionSocket *conn = nullptr;
    try {
        conn = new ConnectionSocket(server->ioloop, uriss.str(), newFd);
    }
    catch (std::exception &e) {
        sys_socket_close(newFd);
        return 0;
    }

    using std::placeholders::_1;
    using std::placeholders::_2;
    conn_func_t lostConnectionFunc = std::bind(&SocketServer::lostConnection, server, _1, _2);
    conn->setConnectionHandler(lostConnectionFunc);
    server->onNewConnection(conn);
    return 0;
}

void SocketServer::lostConnection(const std::string &uri, void *reference)
{
    (uri);
    ConnectionSocket *conn = static_cast<ConnectionSocket *>(reference);
    this->onCloseConnection(conn);
}

/** @} */
