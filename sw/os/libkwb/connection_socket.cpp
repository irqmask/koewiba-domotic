/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    connection_socket.cpp
 * @brief   Implementation of sending and receiveing KWB messages over a
 *          socket connection.
 *
 * This implements opening and closing as well as sending and receiving messages
 * over a socket connection. It connects to a socket server.
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

#include <cstring>

// include 
#include "kwb_defines.h"
// libsystem
#include "syssocket.h"

#include "exceptions.h"
#include "log.h"

#include "connection_socket.h"

// --- Local functions ---------------------------------------------------------

// will be called from /ref MESSAGE_SOCKET when a complete message has been
// received and is passed to the common OnIncomingMessage() handler for all
// type of connections.
static void incomingMessageHdl(const msg_t & message, void* reference, void* arg)
{
    ConnectionSocket* sockcon = (ConnectionSocket*)arg;
    sockcon->onIncomingMessage(message);
}

// will be called from /ref MESSAGE_SOCKET when a connection is closed. 
static void closeConnectionHdl(const std::string & uri, void* reference, void* arg)
{
    ConnectionSocket* sockcon = (ConnectionSocket*)arg;
    sockcon->onConnectionClosed();
}

// --- Class member functions --------------------------------------------------

//----------------------------------------------------------------------------
ConnectionSocket::ConnectionSocket(ioloop_t* io, std::string uri)
    : Connection(io, uri)
{
    open();
}

//----------------------------------------------------------------------------
ConnectionSocket::~ConnectionSocket()
{
    close();
}

//----------------------------------------------------------------------------
void ConnectionSocket::getAddressAndPort(std::string uri, std::string & address, uint16_t & port)
{
    size_t pos;
    if ((pos = uri.find_first_of(":")) == std::string::npos)
        throw InvalidParameter(LOC, "Invalid URI: %s!", uri.c_str());

    address = uri.substr(0, pos);

    std::string portstr = uri.substr(pos + 1, std::string::npos);
    uint32_t temp = 0;
    try {
        temp = std::stoul(portstr);
    }
    catch (std::exception & e) {
        throw InvalidParameter(LOC, "Invalid port number in URI %s", uri.c_str());
    }
    if (temp > UINT16_MAX)
        throw InvalidParameter(LOC, "Invalid port number in URI %s", uri.c_str());
    port = (uint16_t)temp;
}

//----------------------------------------------------------------------------
void ConnectionSocket::open()
{
    do {
        std::string address;
        uint16_t port = 0;
        getAddressAndPort(uri, address, port);
        if (port == 0) {
            fd = sys_socket_open_client_unix(address.c_str());
            if (fd <= INVALID_FD) {
                int err = errno;
                throw ConnectionFailed(LOC, "SOCKET Unable to connect client to unix socket server! address=%s syserror=%d %s",
                                       address.c_str(), err, strerror(err));
            }
        } else {
            fd = sys_socket_open_client_tcp(address.c_str(), port);
            if (fd <= INVALID_FD) {
                int err = errno;
                throw ConnectionFailed(LOC, "SOCKET Unable to connect client to tcp socket server! address=%s port=%d syserror=%d %s",
                                       address.c_str(), port, err, strerror(err));
            }
        }

        sys_socket_set_blocking(fd, false);
        if (ioloop != nullptr)
            ioloop_register_fd(ioloop, fd, eIOLOOP_EV_READ, ConnectionSocket::receiveCallback, this);

        log_msg(KWB_LOG_STATUS, "SOCKET open connection to %s", this->getName().c_str());
    } while (0);
}

//----------------------------------------------------------------------------
void ConnectionSocket::close()
{
    if (fd != INVALID_FD) {
        if (ioloop != nullptr)
            ioloop_unregister_fd(ioloop, fd, eIOLOOP_EV_UNKNOWN);

        sys_socket_close (fd);
        fd = INVALID_FD;
        log_msg(KWB_LOG_STATUS, "SOCKET close connection to %s", this->getName().c_str());
    }
}

//----------------------------------------------------------------------------
void ConnectionSocket::send(const msg_t & message)
{
    ssize_t sent = sys_socket_send(fd, &message, sizeof(msg_t));
    if (sent != sizeof(msg_t)) {
        log_sys_error("SOCKET Unable to send message!");
        throw OperationFailed(LOC, "Unable to send message!");
    }
}

//----------------------------------------------------------------------------
void ConnectionSocket::receive()
{
    msg_t message = {0};
    size_t received_bytes;

    do {
        received_bytes = sys_socket_recv(fd, &message, sizeof(message));
        if (received_bytes == 0) {
            onConnectionClosed();
            close();
            break;
        }

        if (received_bytes != sizeof(message)) {
            log_sys_error("SOCKET Invaid size %d of incoming message", received_bytes);
            break;
        }

        msg_log("SOCKET R", &message);
        onIncomingMessage(message);
    } while (0);
}

//----------------------------------------------------------------------------
int32_t ConnectionSocket::receiveCallback(void *context)
{
    reinterpret_cast<ConnectionSocket*>(context)->receive();
    return 0;
}
/** @} */
