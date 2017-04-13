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

#include "log.h"
#include "message.h"

#include "router.h"

// --- Local functions ---------------------------------------------------------

void onIncommingMessage(msg_t* message, void* reference, void* arg)
{
    log_msg(LOG_STATUS, "Router received message");
    msg_log(*message);
    Router* router = (Router*)arg;
    RouteConnection* sender = (RouteConnection*)reference;
    router->DistributeMessage(message, sender);
}

// --- Class member functions --------------------------------------------------

Router::Router()
{
    connections.clear();
}

Router::~Router()
{
    connections.clear();
}

void Router::AddConnection(RouteConnection* connection)
{
    connection->SetIncommingHandler(onIncommingMessage, this);
    connections.push_back(connection);
}

void Router::RemoveConnection(RouteConnection* connection)
{
    connection->ClearIncommingHandler();
    connections.remove(connection);
}

void Router::DistributeMessage(msg_t* message, RouteConnection* sender)
{
    RouteConnection* temp;

    for (std::list<RouteConnection*>::iterator it = connections.begin();
         it != connections.end();
         ++it) {
        if (*it == sender) continue;
        temp = *it;
        temp->Send(message);
    }
}
