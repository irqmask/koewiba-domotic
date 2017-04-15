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

#ifndef ROUTECONNECTION_H
#define ROUTECONNECTION_H

#include "ioloop.h"
#include "message.h"

class RouteConnection
{
protected:
    msg_incom_func_t    extOnIncommingMsg;
    void*               extOnIncommingMsgArg;
    msg_conn_func_t     extOnConnectionClosed;
    void*               extOnConnectionClosedArg;
    ioloop_t*           ioloop;
    char                name[32];

public:
    RouteConnection();
    ~RouteConnection();

    virtual const char* GetName();

    void SetIncommingHandler(msg_incom_func_t func, void* arg);
    void ClearIncommingHandler();
    void OnIncommingMessage(msg_t* message);

    void SetConnectionHandler(msg_conn_func_t func, void* arg);
    void ClearConnectionHandler();
    virtual void OnConnectionClosed();

    virtual int Send(msg_t* message);
};

#endif // ROUTECONNECTION_H
