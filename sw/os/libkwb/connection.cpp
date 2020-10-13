/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    connection.cpp
 * @brief   Implementation of the base class for connections of the kwb protocol.
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
#include <string>

#include "bus.h"
#include "log.h"

#include "connection.h"

////////////////////////////////////////////////////////////////////////////////
Connection::Connection(ioloop_t *io, std::string uri)
    : extOnIncommingMsg(nullptr)
    , extOnIncommingMsgArg(nullptr)
    , extOnConnectionClosed(nullptr)
    , extOnConnectionClosedArg(nullptr)
    , ioloop(io)
    , uri(uri)
    , segmentId(0)
{
}

////////////////////////////////////////////////////////////////////////////////
Connection::~Connection()
{
}

////////////////////////////////////////////////////////////////////////////////
const std::string Connection::getName()
{
    return this->uri;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::setIncomingHandler(incom_func_t func)
{
    extOnIncommingMsg = func;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::clearIncomingHandler()
{
    extOnIncommingMsg = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::onIncomingMessage(const msg_t & message)
{
    if (this->extOnIncommingMsg != nullptr) {
        this->extOnIncommingMsg(message, this);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Connection::setConnectionHandler(conn_func_t func)
{
    this->extOnConnectionClosed = func;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::clearConnectionHandler()
{
    this->extOnConnectionClosed = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::onConnectionClosed()
{
    if (this->extOnConnectionClosed != nullptr) {
        this->extOnConnectionClosed(uri, this);
    }
    // do nothing else at this point. the class may be deleted right now.
}

////////////////////////////////////////////////////////////////////////////////
void Connection::setOwnNodeId(uint16_t nodeId)
{
    this->ownNodeId = nodeId;
    this->segmentId = nodeId & BUS_SEGBRDCSTMASK;
}

////////////////////////////////////////////////////////////////////////////////
uint16_t Connection::getOwnNodeId()
{
    return this->ownNodeId;
}

////////////////////////////////////////////////////////////////////////////////
uint16_t Connection::getSegmentAddress()
{
    return this->segmentId;
}

////////////////////////////////////////////////////////////////////////////////
bool Connection::addressIsInConnectionsSegment(uint16_t node_address)
{
    if ((segmentId == 0) ||
        (node_address == 0) ||
        ((node_address & BUS_SEGBRDCSTMASK) == segmentId)) return true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::send(const msg_t & message)
{
}

/** @} */
