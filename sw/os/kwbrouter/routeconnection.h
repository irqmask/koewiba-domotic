/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    routeconnection.h
 * @brief   This is the base class for connections of the router.
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
#ifndef ROUTECONNECTION_H
#define ROUTECONNECTION_H

#include "ioloop.h"
#include "message.h"

/**
 * Base class for connections to or from which messages are routed by the 
 * router. 
 * This class itself provides only common used functions for all 
 * route-connection types. Do not use it by itself!
 */
class RouteConnection
{
protected:
    msg_incom_func_t    extOnIncommingMsg;
    void*               extOnIncommingMsgArg;
    msg_conn_func_t     extOnConnectionClosed;
    void*               extOnConnectionClosedArg;
    ioloop_t*           ioloop;
    char                name[32];
    uint16_t            segmentAddress;     //!< segment address of connection

public:
    RouteConnection();
    ~RouteConnection();

    virtual const char* GetName();

    void SetIncommingHandler(msg_incom_func_t func, void* arg);
    void ClearIncommingHandler();
    virtual void OnIncomingMessage(msg_t* message);

    void SetConnectionHandler(msg_conn_func_t func, void* arg);
    void ClearConnectionHandler();
    virtual void OnConnectionClosed();

    void SetSegmentAddress(uint16_t segment_address);
    uint16_t GetSegmentAddress();

    virtual bool AddressIsInConnectionsSegment(uint16_t node_address);
    virtual int Send(msg_t* message);
};

#endif // ROUTECONNECTION_H
/** @} */
