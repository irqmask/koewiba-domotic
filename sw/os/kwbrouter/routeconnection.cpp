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

#include <string.h>
#include "log.h"

#include "routeconnection.h"


RouteConnection::RouteConnection() : extOnIncommingMsg((msg_incom_func_t)NULL),
                                     extOnIncommingMsgArg(NULL),
                                     ioloop((ioloop_t*)NULL)
{
    memset(this->name, 0, sizeof(name));
    snprintf(name, sizeof(name-1), "RC");
}

RouteConnection::~RouteConnection()
{
}

const char* RouteConnection::GetName()
{
    return this->name;
}

void RouteConnection::SetIncommingHandler(msg_incom_func_t func, void* arg)
{
    extOnIncommingMsg = func;
    extOnIncommingMsgArg = arg;
}

void RouteConnection::ClearIncommingHandler()
{
    extOnIncommingMsg = (msg_incom_func_t)NULL;
    extOnIncommingMsgArg = NULL;
}

int RouteConnection::Send(msg_t* message)
{
    log_msg(LOG_VERBOSE1, "%6s <-- message sent", this->GetName());
    msg_log(*message);
    return 0;
}

void RouteConnection::OnIncommingMessage(msg_t* message)
{
    log_msg(LOG_VERBOSE1, "%6s --> message received", this->GetName());
    msg_log(*message);
    if (this->extOnIncommingMsg != NULL) {
        this->extOnIncommingMsg(message, this, this->extOnIncommingMsgArg);
    }
}

void RouteConnection::SetConnectionHandler(msg_conn_func_t func, void* arg)
{
    this->extOnConnectionClosed = func;
    this->extOnConnectionClosedArg = arg;
}

void RouteConnection::ClearConnectionHandler()
{
    this->extOnConnectionClosed = NULL;
    this->extOnConnectionClosedArg = NULL;
}

void RouteConnection::OnConnectionClosed()
{
    log_msg(LOG_STATUS, "Client connection closed");
    if (this->extOnConnectionClosed != NULL) {
        this->extOnConnectionClosed("", 0, this, this->extOnConnectionClosedArg);
    }
    // do nothing else at this point. the class may be deleted right now.
}
