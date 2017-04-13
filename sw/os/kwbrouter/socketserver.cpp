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

#include "socketserver.h"

// --- Local functions ---------------------------------------------------------

void new_connection(const char* address, uint16_t port, void* reference, void* arg)
{
    SocketServer* server = (SocketServer*)arg;
    server->OnNewConnection((msg_endpoint_t*)reference);
}

void close_connection(const char* address, uint16_t port, void* reference, void* arg)
{
    SocketServer* server = (SocketServer*)arg;
    RConnSocketClient* client = (RConnSocketClient*)reference;
    server->OnCloseConnection(client);
}

// --- Class member functions --------------------------------------------------

SocketServer::SocketServer() : router((Router*)NULL),
                               ioloop(NULL)
{
    clients.clear();
    msg_s_init(&this->server);
}

SocketServer::SocketServer(ioloop_t* iol)
{
    SocketServer();
    this->ioloop = iol;
}

SocketServer::SocketServer(ioloop_t* iol, Router* r)
{
    SocketServer();
    this->ioloop = iol;
    this->router = r;
}

SocketServer::~SocketServer()
{
    this->Close();
}

int SocketServer::Open(const char* address, uint16_t port)
{
    int retval;
    msg_s_set_newconnection_handler(&this->server, new_connection, (void*)this);
    retval = msg_s_open_server(&this->server, this->ioloop, address, port);
    return retval;
}

void SocketServer::Close()
{
    RConnSocketClient* temp;

    for (std::list<RConnSocketClient*>::iterator it = clients.begin();
         it != clients.end();
         ++it) {
        temp = *it;
        temp->Close();
        if (this->router != NULL) {
            this->router->RemoveConnection(temp);
        }
        delete(temp);
    }
    clients.clear();
}

void SocketServer::OnNewConnection(msg_endpoint_t* endpoint)
{
    RConnSocketClient *newconn = new RConnSocketClient(&this->server, endpoint, this->server.address, this->server.port);
    newconn->SetConnectionHandler(close_connection, this);
    clients.push_back(newconn);
    if (this->router != NULL) {
        this->router->AddConnection(newconn);
    }
}

void SocketServer::OnCloseConnection(RConnSocketClient* client)
{
    if (this->router != NULL) {
        this->router->RemoveConnection(client);
    }
    clients.remove(client);
    delete client;
}