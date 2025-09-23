/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    connection.h
 * @brief   This is the base class for connections of the kwb protocol.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2020  christian <irqmask@gmx.de>
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
#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "ioloop.h"
#include "message.h"

class Connection;

//! Function pointer type for incoming messages
typedef std::function<void(const msg_t &message, void *reference)> incom_func_t;
//! Function pointer type for closing connections
typedef std::function<void(const std::string &uri, void *reference)> conn_func_t;

/**
 * Base class for connections to or from which messages are routed by the
 * router.
 * This class itself provides only common used functions for all
 * route-connection types. Do not use it by itself!
 */
class Connection
{
public:
    /**
     * Default constructor. Initializes data for methods in this base class.
     * This base-class by itself is useless, because it implements no usable
     * connection for sending and receiving messages.
     *
     * @param[in]   io      Pointer to existing IO-loop instance.
     * @param[in]   uri     Address or path to device of connection.
     */
    Connection(ioloop_t *io, const std::string &uri);

    /**
     * No need for cleanup in base-class
     */
    ~Connection();

    /**
     * @returns The name of the connection.
     */
    virtual const std::string getName();

    /**
     * Register "incoming message" callback.
     *
     * @param[in]   func        Function of type #msg_incom_func_t to be called.
     */
    void setIncomingHandler(incom_func_t &func);

    /**
     * Unregister "incomming message" callback. No function will be called when
     * connection closes.
     */
    void clearIncomingHandler();

    /**
     * Conveys incoming message to be passed to the router.
     *
     * @param[in]   message     Incoming message.
     */
    virtual void onIncomingMessage(const msg_t &message);

    /**
     * Register "OnClose" callback, which is called when the connection
     * is closed.
     *
     * @param[in]   func        Function of type #msg_conn_func_t to be called.
     */
    void setConnectionHandler(conn_func_t &func);

    /**
     * Unregister "OnClose" callback. No function will be called when connection
     * closes.
     */
    void clearConnectionHandler();

    /**
     * Called by external "OnClose" event
     */
    virtual void onConnectionClosed();

    /**
     * Set node id of corresponding bus-gateway
     * @param[in] nodeAddress   Node id to set.
     */
    void setOwnNodeId(uint16_t nodeAddress);

    /**
     * @returns Node address of corresponding bus-gateway
     */
    uint16_t getOwnNodeId();

    /**
     * Get the segment address.
     * @return segment address.
     */
    uint16_t getSegmentAddress();

    /**
     * Checks if given address is in the range of the connections segment.
     *
     * Currently used only for serial connections to check if address is in the
     * targeted bus-segment.
     * @param[in] nodeAddress   Address to check. If segment address is set to 0,
     *                          every address is considered beeing in this segment.
     * @returns true if address is in the bus segment, otherwise false.
     */
    virtual bool addressIsInConnectionsSegment(uint16_t nodeAddress);

    /**
     * Send the message over the established route-connection.
     *
     * @param[in]   message     Message to be sent.
     *
     * @throws OperationFailed, if message was not completely sent.
     */
    virtual void send(const msg_t &message);

protected:
    //! Callback which will be called for each incoming message.
    incom_func_t        extOnIncomingMsg;
    //! Optional callback argument when messages arrive.
    void               *extOnIncomingMsgArg;
    //! Callback which will be called for each closed connection.
    conn_func_t         extOnConnectionClosed;
    //! Optional callback argument when the connection is closed.
    void               *extOnConnectionClosedArg;
    //! IO-loop instance
    ioloop_t           *ioloop;
    //! Address or path to device of connection
    std::string         uri;
    //! Node-id represented by this connection
    uint16_t            ownNodeId;
    //! Segment address of connection
    uint16_t            segmentId;
};

/** @} */
