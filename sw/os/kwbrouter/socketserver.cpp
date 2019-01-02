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

#include "socketserver.h"

// --- Local functions ---------------------------------------------------------

// callback from /refMESSAGE_SOCKET socket-server when a new connection has been
// established.
static void new_connection(const char* address, uint16_t port, void* reference, void* arg)
{
    SocketServer* server = (SocketServer*)arg;
    server->OnNewConnection((msg_endpoint_t*)reference);
}

// callback from /refMESSAGE_SOCKET socket-server when a connection has been
// closed.
static void close_connection(const char* address, uint16_t port, void* reference, void* arg)
{
    SocketServer* server = (SocketServer*)arg;
    RConnSocketClient* client = (RConnSocketClient*)reference;
    server->OnCloseConnection(client);
}

// --- Class member functions --------------------------------------------------

/**
 * @todo use default paparmeters in full constructor instead of three constructors!
 */
SocketServer::SocketServer() : router((Router*)NULL),
                               ioloop(NULL)
{
    clients.clear();
    msg_s_init(&this->server);
}

/**
 * @todo use default paparmeters in full constructor instead of three constructors!
 */
SocketServer::SocketServer(ioloop_t* iol)
{
    SocketServer();
    this->ioloop = iol;
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
SocketServer::SocketServer(ioloop_t* iol, Router* r)
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
    this->Close();
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
int SocketServer::Open(const char* address, uint16_t port)
{
    int retval;
    msg_s_set_newconnection_handler(&this->server, new_connection, (void*)this);
    retval = msg_s_open_server(&this->server, this->ioloop, address, port);
    return retval;
}

/**
 * Closes the server. Before all remaining client connections will be closed.
 */
void SocketServer::Close()
{
    for(auto client : clients) {
        client->Close();
        if (this->router != NULL) {
            this->router->RemoveConnection(client);
        }
        delete(client);
    }
    clients.clear();
}

/**
 * Called when a new connection has been established.
 * @param[in]   endpoint    Pointer to endpoint structure which holds
 *                          information about the established connection.
 */
void SocketServer::OnNewConnection(msg_endpoint_t* endpoint)
{
    RConnSocketClient *newconn = new RConnSocketClient(&this->server, endpoint, this->server.address, this->server.port);
    newconn->SetConnectionHandler(close_connection, this);
    clients.push_back(newconn);
    if (this->router != NULL) {
        this->router->AddConnection(newconn);
    }
}

/**
 * Call this function, when a client connection closes, so the client 
 * can be removed from client list.
 */
void SocketServer::OnCloseConnection(RConnSocketClient* client)
{
    if (this->router != NULL) {
        this->router->RemoveConnection(client);
    }
    clients.remove(client);
    delete client;
}

/** @} */
