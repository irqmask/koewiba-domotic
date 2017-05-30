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

#include "routeconnection.h"

class Router
{
private:
    std::list<RouteConnection*> connections;

public:
    Router();
    ~Router();

    void AddConnection(RouteConnection* connection);
    void RemoveConnection(RouteConnection* connection);

    void DistributeMessage(msg_t* message, RouteConnection* sender);
};

#endif // ROUTER_H
