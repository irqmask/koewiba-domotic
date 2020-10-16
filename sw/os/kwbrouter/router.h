/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    router.h
 * @brief   Router takes incomming message from a connection and routes it to
 *          all other connections.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
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
#ifndef ROUTER_H
#define ROUTER_H

#include <list>
#include <stddef.h>

#include "connection.h"

/**
 * The router takes incomming message from a connection and routes it to
 * all other connections.
 * The router is not thread-safe, it has to be used in a single threaded
 * loop.
 * The current design routes an incomming message from one connection to
 * all other connections.
 */
class Router
{
public:
    /**
     * ctor
     */
    Router();
    /**
     * Closes and deletes all remaining connections. (The closing happens
     * in the connection's destructor.
     */
    ~Router();

    /**
     * Adds a connection to/from which messages are routed.
     *
     * @param[in]   connection  Connection to be added.
     */
    void addConnection(Connection *connection);

    /**
     * Removes a connection from the routing list and unregisteres the router's
     * callback from the connection.
     *
     * @param[in]   connection  Connection to be removed.
     */
    void removeConnection(Connection *connection);

    /**
     * Distributes a received message from one connection to all other connection.
     * The message is not echoed back to the sender.
     */
    void distributeMessage(const msg_t &message, Connection *sender);

protected:
    /**
     * Callback to be called by incomming message handlers of all connections.
     *
     * @param[in]   message     Message to be distributed.
     * @param[in]   reference   Reference to sender of the message.
     * @todo replace typeless reference with connection type.
     */
    void onIncomingMessage(const msg_t &message, void *reference);

    /**
     * Log current list of connections.
     * @param[in]   current     Mark current connection(optional)
     */
    void listConnections(Connection *current);

    //! List of all established connections
    std::list<Connection *> connections;
};

#endif // ROUTER_H
/** @} */
