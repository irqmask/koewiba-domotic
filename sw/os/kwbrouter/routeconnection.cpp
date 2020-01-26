/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    routeconnection.cpp
 * @brief   Implementation of the base class for connections of the router.
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

#include <stdio.h>
#include <string.h>

#include "bus.h"
#include "log.h"

#include "routeconnection.h"

/**
 * Default constructor. Initializes data for methods in this base class.
 * This base-class by itself is useless, because it implements no usable
 * connection for sending and receiving messages.
 */
RouteConnection::RouteConnection() : extOnIncommingMsg((msg_incom_func_t)nullptr)
                                   , extOnIncommingMsgArg(nullptr)
                                   , extOnConnectionClosed(nullptr)
                                   , extOnConnectionClosedArg(nullptr)
                                   , ioloop((ioloop_t*)nullptr)
                                   , segmentAddress(0)
{
    memset(this->name, 0, sizeof(name));
    snprintf(name, sizeof(name) - 1, "RC");
}

/**
 * No need for cleanup in base-class
 */
RouteConnection::~RouteConnection()
{
}

/**
 * Get the name of the type of the route connection
 * @returns The name of the connection type.
 */
const char* RouteConnection::GetName()
{
    return this->name;
}

/**
 * Register "incomming message" callback.
 *
 * @param[in]   func        Function of type #msg_incom_func_t to be called.
 * @param[in]   arg         Optional argument which is passed to the callback.
 */
void RouteConnection::SetIncommingHandler(msg_incom_func_t func, void* arg)
{
    extOnIncommingMsg = func;
    extOnIncommingMsgArg = arg;
}

/**
 * Unregister "incomming message" callback. No function will be called when 
 * connection closes.
 */
void RouteConnection::ClearIncommingHandler()
{
    extOnIncommingMsg = (msg_incom_func_t)nullptr;
    extOnIncommingMsgArg = nullptr;
}

void RouteConnection::SetSegmentAddress(uint16_t segment_address)
{
    segmentAddress = segment_address & BUS_SEGBRDCSTMASK;
}

/**
 * Get the segment address.
 * @return segment address.
 */
uint16_t RouteConnection::GetSegmentAddress()
{
    return segmentAddress;
}

/**
 * Checks if given address is in the range of the connections segment.
 *
 * Currently used only for serial connections to check if address is in the
 * targeted bus-segment.
 * @param[in] node_address  Address to check. If segment address is set to 0,
 *                          every address is considered beeing in this segment.
 * @returns true if address is in the bus segment, otherwise false.
 */
bool RouteConnection::AddressIsInConnectionsSegment(uint16_t node_address)
{
    if ((segmentAddress == 0) ||
        (node_address == 0) ||
        ((node_address & BUS_SEGBRDCSTMASK) == segmentAddress)) return true;
    return false;
}

/**
 * Send the message over the established route-connection.
 *
 * @param[in]   message     Message to be sent.
 *
 * @returns eERR_NONE, if message was successfully sent, 
 *          eRUNNING, if message was not completely sent,
 *          otherwise error code of #gen_errors_t.
 */
int RouteConnection::Send(msg_t* message)
{
    log_msg(LOG_VERBOSE1, "%6s <-- message sent", this->GetName());
    msg_log("???SEND", *message);
    return 0;
}

/**
 * Conveys incomming message to be passed to the router.
 *
 * @param[in]   message     Incomming message.
 */
void RouteConnection::OnIncomingMessage(msg_t* message)
{
    log_msg(LOG_VERBOSE1, "%6s --> message received", this->GetName());
    msg_log("???RECV", *message);
    if (this->extOnIncommingMsg != nullptr) {
        this->extOnIncommingMsg(message, this, this->extOnIncommingMsgArg);
    }
}

/**
 * Register "OnClose" callback, which is called when the connection of
 * this route is closed.
 *
 * @param[in]   func        Function of type #msg_conn_func_t to be called.
 * @param[in]   arg         Optional argument which is passed to the callback.
 */
void RouteConnection::SetConnectionHandler(msg_conn_func_t func, void* arg)
{
    this->extOnConnectionClosed = func;
    this->extOnConnectionClosedArg = arg;
}

/**
 * Unregister "OnClose" callback. No function will be called when connection
 * closes.
 */
void RouteConnection::ClearConnectionHandler()
{
    this->extOnConnectionClosed = nullptr;
    this->extOnConnectionClosedArg = nullptr;
}

/**
 * Called by external "OnClose" event
 */
void RouteConnection::OnConnectionClosed()
{
    log_msg(LOG_STATUS, "%s Client connection closed", this->GetName());
    if (this->extOnConnectionClosed != nullptr) {
        this->extOnConnectionClosed("", 0, this, this->extOnConnectionClosedArg);
    }
    // do nothing else at this point. the class may be deleted right now.
}


/** @} */
