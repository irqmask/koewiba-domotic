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

// --- Class member functions --------------------------------------------------

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
 * Callback to be called by incomming message handlers of all connections.
 *
 * @param[in]   message     Message to be distributed.
 * @param[in]   reference   Reference to sender of the message.
 * @todo replace typeless reference with connection type.
 */
void Router::onIncomingMessage(const msg_t & message, void* reference)
{
    Connection* sending_conn = static_cast<Connection*>(reference);
    this->distributeMessage(message, sending_conn);
}

/**
 * Adds a connection to/from which messages are routed.
 *
 * @param[in]   connection  Connection to be added.
 */
void Router::addConnection(Connection* connection)
{
    connections.push_back(connection);
    using std::placeholders::_1;
    using std::placeholders::_2;
    incom_func_t handleIncomingMessageFunc = std::bind(&Router::onIncomingMessage, this, _1, _2);
    connection->setIncomingHandler(handleIncomingMessageFunc);
    log_info("Add new connection:");
    listConnections(connection);
}

/**
 * Removes a connection from the routing list and unregisteres the router's 
 * callback from the connection.
 *
 * @param[in]   connection  Connection to be removed.
 */
void Router::removeConnection(Connection* connection)
{
    log_info("Remove Closed connection:");
    listConnections(connection);
    connection->clearIncomingHandler();
    connections.remove(connection);
}

/**
 * Distributes a received message from one connection to all other connection.
 * The message is not echoed back to the sender.
 */
void Router::distributeMessage(const msg_t & message, Connection* sender)
{
    for (auto conn : connections) {
        if (conn == sender) continue;
        if (!conn->addressIsInConnectionsSegment(message.receiver)) continue;
        log_info("ROUTE FROM %s NODE %04X VIA %s TO NODE %04X msg %s", sender->getName().c_str(), message.sender, conn->getName().c_str(), message.receiver, msg_to_string(&message, 16));
        conn->send(message);
    }
}

//----------------------------------------------------------------------------
void Router::listConnections(Connection *current)
{
    int32_t index = 0;
    for (auto conn : connections) {
        if (conn == current) {
            log_info("%02d: %15s (*)", index, conn->getName().c_str());
        } else {
            log_info("%02d: %15s", index, conn->getName().c_str());
        }
        index++;
    }
}

/** @} */
