/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    router.cpp
 * @brief   The router takes incomming message from a connection and routes it to
 *          all other connections.
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

#include "log.h"
#include "message.h"

#include "router.h"

// --- Local functions ---------------------------------------------------------

/**
 * Callback to be called by incomming message handlers of all connections.
 *
 * @param[in]   message     Pointer to message to be distributed.
 * @param[in]   reference   Reference to sender of the message.
 * @param[in]   arg         Has to be a pointer to router class.
 * @todo pass message as reference instead of pointer to clarify its lifetime.
 * @todo replace typeless reference with connection type.
 */
void onIncommingMessage(msg_t* message, void* reference, void* arg)
{
    Router* router = (Router*)arg;
    RouteConnection* sender = (RouteConnection*)reference;
    router->DistributeMessage(message, sender);
}

// --- Class member functions --------------------------------------------------

/**
 * Currently nothing to do on creation.
 */
Router::Router()
{
    connections.clear();
}

/**
 * Closes and deletes all remaining connections. (The closing happens
 * in the connection's destructor.
 */
Router::~Router()
{
    connections.clear();
}

/**
 * Adds a connection to/from which messages are routed.
 *
 * @param[in]   connection  Connection to be added.
 */
void Router::AddConnection(RouteConnection* connection)
{
    connection->SetIncommingHandler(onIncommingMessage, this);
    connections.push_back(connection);
}

/**
 * Removes a connection from the routing list and unregisteres the router's 
 * callback from the connection.
 *
 * @param[in]   connection  Connection to be removed.
 */
void Router::RemoveConnection(RouteConnection* connection)
{
    connection->ClearIncommingHandler();
    connections.remove(connection);
}

/**
 * Distributes a received message from one connection to all other connection.
 * The message is not echoed back to the sender.
 */
void Router::DistributeMessage(msg_t* message, RouteConnection* sender)
{
    for (auto conn : connections) {
        if (conn == sender) continue;
        conn->Send(message);
    }
}
/** @} */
