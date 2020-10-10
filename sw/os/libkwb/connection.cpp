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
    , segmentAddress(0)
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
void Connection::setIncomingHandler(incom_func_t func, void* arg)
{
    extOnIncommingMsg = func;
    extOnIncommingMsgArg = arg;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::clearIncomingHandler()
{
    extOnIncommingMsg = nullptr;
    extOnIncommingMsgArg = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::onIncomingMessage(const msg_t & message)
{
    if (this->extOnIncommingMsg != nullptr) {
        this->extOnIncommingMsg(message, this, this->extOnIncommingMsgArg);
    }
}

////////////////////////////////////////////////////////////////////////////////
void Connection::setConnectionHandler(conn_func_t func, void* arg)
{
    this->extOnConnectionClosed = func;
    this->extOnConnectionClosedArg = arg;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::clearConnectionHandler()
{
    this->extOnConnectionClosed = nullptr;
    this->extOnConnectionClosedArg = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::onConnectionClosed()
{
    if (this->extOnConnectionClosed != nullptr) {
        this->extOnConnectionClosed(uri, this, this->extOnConnectionClosedArg);
    }
    // do nothing else at this point. the class may be deleted right now.
}

////////////////////////////////////////////////////////////////////////////////
void Connection::setSegmentAddress(uint16_t segment_address)
{
    segmentAddress = segment_address & BUS_SEGBRDCSTMASK;
}

////////////////////////////////////////////////////////////////////////////////
uint16_t Connection::getSegmentAddress()
{
    return segmentAddress;
}

////////////////////////////////////////////////////////////////////////////////
bool Connection::addressIsInConnectionsSegment(uint16_t node_address)
{
    if ((segmentAddress == 0) ||
        (node_address == 0) ||
        ((node_address & BUS_SEGBRDCSTMASK) == segmentAddress)) return true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Connection::send(const msg_t & message)
{
}

/** @} */
