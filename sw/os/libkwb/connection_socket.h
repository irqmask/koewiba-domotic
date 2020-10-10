/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    connection_socket.h
 * @brief   Send and receive KWB messages over a socket connection.
 *
 * It connects to a socket server or is spawned by a
 * socket server.
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

#include "connection.h"

/**
 * This class implements sending and receiving KWB messages over a socket
 * connection.
 */
class ConnectionSocket : public Connection
{
public:
    /**
     * Initializes a socket connection which has been accepted by the socket server.
     *
     * @param[in]   ioloop      Pointer to ioloop.
     * @param[in]   address     Address and port of socket connection of TCP/IP
     *                          address, e.g. 127.0.0.1:1234.
     * @param[in]   fd          File descriptor of established connection (optional
     *                          for server usage)
     */
    ConnectionSocket(ioloop_t *io, std::string uri, sys_fd_t fd = INVALID_FD);

    /**
     * Closes active connection.
     * @note Does not call onConnectionClosed()!
     */
    ~ConnectionSocket();

    /**
     * Extract address and port from URI
     * URI is expected to be in the one of this format:
     * IPv4: AAA.AAA.AAA.AAA:PPPPP
     *
     * @param[in]   uri         URI string
     * @param[out]  address     Address part of URI
     * @param[out]  port        Port number as part of URI
     * @throws InvalidParameter, if address and/or port was not found in URI.
     * @note This is not a complete implementation of a URI parser as defined
     *       in RFC 3986
     */
    static void getAddressAndPort(std::string uri, std::string & address, uint16_t & port);

    /**
     * Send a KWB messgae over the socket connection.
     * @param[in]   message     KWB message to be sent
     */
    virtual void send(const msg_t & message) override;

protected:
    /**
     * Open the connection to the socket.
     */
    void open();

    /**
     * Close the connection to the socket.
     */
    void close();

    /**
     * Receive pending data. Call onIncomingMessage() for each completely
     * received message.
     */
    void receive();

    /**
     * Callback to be registered with ioloop which is called when data is
     * pending for this connection.
     * @param[in]   context     Reference to this object.
     * @return 0 (just to fulfill ioloop callback signature)
     */
    static int32_t receiveCallback(void *context);

protected:
    sys_fd_t    fd;     //!< File descriptor of socket connection
};

/** @} */
