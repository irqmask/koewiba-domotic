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
#include <sstream>

// include 
#include "kwb_defines.h"
// libsystem
#include "sysserial.h"

#include "exceptions.h"
#include "log.h"

#include "connection_serial.h"

// --- Local functions ---------------------------------------------------------

// will be called when a complete message has been
// received and is passed to the common OnIncomingMessage() handler for all
// type of connections.
static void incomingMessageHdl(const msg_t & message, void* reference, void* arg)
{
    ConnectionSerial* sercon = (ConnectionSerial*)arg;
    sercon->onIncomingMessage(message);
}

// will be called from when a connection is closed.
static void closeConnectionHdl(const std::string & uri, void* reference, void* arg)
{
    ConnectionSerial* sercon = (ConnectionSerial*)arg;
    sercon->onConnectionClosed();
}

// --- Class member functions --------------------------------------------------

//----------------------------------------------------------------------------
ConnectionSerial::ConnectionSerial(ioloop_t* io, std::string uri, bool configure)
    : Connection(io, uri)
    , baudrate(57600)
{
    open(configure);
}

//----------------------------------------------------------------------------
ConnectionSerial::~ConnectionSerial()
{
    close();
}

//----------------------------------------------------------------------------
uint32_t ConnectionSerial::getBaudrate()
{
    return this->baudrate;
}

//----------------------------------------------------------------------------
void ConnectionSerial::open(bool configure)
{
    int rc;

    fd = sys_serial_open(uri.c_str());
    if (fd == INVALID_FD) {
        log_sys_error("SERIAL: error opening connection to device %s", uri.c_str());
        throw ConnectionFailed(LOC, "error opening connection to device %s", uri.c_str());
    }

    if (configure) {
        rc = sys_serial_set_params(fd,
                                   sys_serial_baudrate(baudrate),
                                   eSYS_SER_DB_8,
                                   eSYS_SER_P_NONE,
                                   eSYS_SER_SB_1,
                                   eSYS_SER_FC_HW);
        if (rc != eERR_NONE) {
            log_sys_error("SERIAL: error setting baudrate %d and serial parameters", baudrate);
            throw ConnectionFailed(LOC, "error setting baudrate %d and serial parameter", baudrate);
        }
    }
    sys_serial_flush(fd);

    if (ioloop != nullptr)
        ioloop_register_fd(ioloop, fd, eIOLOOP_EV_READ, ConnectionSerial::receiveCallback, this);
}

//----------------------------------------------------------------------------
void ConnectionSerial::close()
{
    if (fd != INVALID_FD) {
        if (ioloop != nullptr)
            ioloop_unregister_fd(ioloop, fd, eIOLOOP_EV_UNKNOWN);

        sys_serial_close (fd);
        fd = INVALID_FD;
        log_msg(KWB_LOG_STATUS, "SERIAL close connection to %s", this->getName().c_str());
    }
}

//----------------------------------------------------------------------------
void ConnectionSerial::send(const msg_t & message)
{
    int     rc = eERR_NONE;

    do {
        // still bytes left to write (from previous call)?
        if (outgoingLength > outgoingWritten) {
            rc = continueSending();
            if (rc == eRUNNING) {
                rc = eMSG_ERR_BUSY;
                break; // still not ready to send new message
            }
        }

        outgoingLength = formatSerialMessage(outgoingBuffer, sizeof(outgoingBuffer), message);
        if (outgoingLength == 0) {
            log_error("SERIAL error encoding serial message!");
            rc = eMSG_ERR_SIZE;
        } else {
            log_msg(LOG_VERBOSE2, "SERIAL S RAW %s", outgoingBuffer);
            outgoingWritten = sys_serial_send(fd,
                                              (void*)outgoingBuffer,
                                              outgoingLength);
            if (outgoingWritten < 0) {
                log_sys_error("SERIAL Send failed!");
                rc = eERR_SYSTEM;
            } else if (outgoingWritten < outgoingLength) {
                log_warning("SERIAL Not all bytes written");
                rc = eRUNNING;
            }

            if (rc == eRUNNING || rc == eERR_NONE) {
                msg_log("SERIAL S", &message);
            }
        }
    } while (false);
}

//----------------------------------------------------------------------------
void ConnectionSerial::receive()
{
    char c;
    size_t received_bytes;
    bool complete;

    do {
        // reveive message
        do {
            received_bytes = sys_serial_recv(fd, &c, 1);
            // connection closed?
            if (received_bytes == 0) {
                onConnectionClosed();
                close();
                break;
            }

            // read one character, break if receive queue empty
            if (received_bytes != 1) {
                break;
            }
            // process character and check for a complete message
            complete = processReceiving(c);
        } while (!complete);

        // handle complete message
        if (complete) {
            msg_log("SERIAL R", &incomingMessage);
            onIncomingMessage(incomingMessage);
        }
    } while (0);
}

//----------------------------------------------------------------------------
int32_t ConnectionSerial::receiveCallback(void *context)
{
    reinterpret_cast<ConnectionSerial*>(context)->receive();
    return 0;
}

//----------------------------------------------------------------------------
void ConnectionSerial::convertByteToAscii (char* buffer, uint8_t* offset,
                                           uint8_t byte)
{
    uint8_t nibble;

    nibble = byte >> 4;
    if (nibble < 10) {
        buffer[(*offset)++] = nibble + '0';
    } else {
        buffer[(*offset)++] = nibble + 'A' - 10;
    }
    nibble = byte & 0x0F;
    if (nibble < 10) {
        buffer[(*offset)++] = nibble + '0';
    } else {
        buffer[(*offset)++] = nibble + 'A' - 10;
    }
}

//----------------------------------------------------------------------------
int ConnectionSerial::convertCharToNibble(char c, uint8_t* nibble)
{
    if (c >= '0' && c <= '9') {
        *nibble = (uint8_t)(c - '0');
    } else if (c >= 'a' && c <= 'f') {
        *nibble = (uint8_t)(c - ('a' - 10));
    } else if (c >= 'A' && c <= 'F') {
        *nibble = (uint8_t)(c - ('A' - 10));
    } else {
        return (uint8_t)-1;
    }
    return 0;
}

//----------------------------------------------------------------------------
size_t ConnectionSerial::formatSerialMessage(char* buffer, uint8_t buffersize, const msg_t & message)
{
    uint8_t ii, offset = 0;

    if (message.length > MAX_MSG_SIZE)  return 0;
    if (buffersize < (message.length*2 + 7)) return 0;
    memset(buffer, 0, buffersize);
    // sender
    convertByteToAscii(buffer, &offset, (message.sender & 0xFF00) >> 8);
    convertByteToAscii(buffer, &offset, message.sender & 0x00FF);
    // receiver
    convertByteToAscii(buffer, &offset, (message.receiver & 0xFF00) >> 8);
    convertByteToAscii(buffer, &offset, message.receiver & 0x00FF);
    // length
    convertByteToAscii(buffer, &offset, message.length);
    // message
    for (ii=0; ii<message.length; ii++) {
        convertByteToAscii(buffer, &offset, message.data[ii]);
    }
    buffer[offset++] = '\n';
    buffer[offset] = '\0';
    return offset;
}

//----------------------------------------------------------------------------
void ConnectionSerial::resetIncomingOnError()
{
    uint8_t ii;
    char strbuf2[1024], single_char[2];

    // log faulty message
    std::string errMsg = "SERIAL Incoming message corrupt: \"";
    single_char[1] = '\0';
    for (ii=0; ii<incomingNumReceived; ii++) {
        switch (incomingBuffer[ii]) {
        case '\n': errMsg += "<NL>"; break;
        case '\r': errMsg += "<CR>"; break;
        default:
            single_char[0] =  incomingBuffer[ii];
            errMsg += single_char;
            break;
        }
    }
    snprintf(strbuf2, sizeof(strbuf2)-1, "\" total length = %d, message length = %d\n", incomingNumReceived, incomingMessage.length);
    errMsg += strbuf2;
    log_error(errMsg.c_str());

    // reset incoming state
    incomingNumReceived = 0;
    incomingState = eSER_RECV_STATE_IDLE;
}

//----------------------------------------------------------------------------
bool ConnectionSerial::processReceiving(char newChar)
{
    uint8_t digit, nibble, index;

    // first byte of new message received
    if (incomingState == eSER_RECV_STATE_IDLE) {
        incomingState = eSER_RECV_STATE_SENDER;
        incomingMessage.sender = 0;
        incomingMessage.receiver = 0;
        incomingMessage.length = 0;
    }

    incomingBuffer[incomingNumReceived++] = newChar;
    // below this line incoming_num_received is counting from 1!

    // decode received
    switch (incomingState) {
    case eSER_RECV_STATE_SENDER:
        digit = 4 - incomingNumReceived;
        if (convertCharToNibble(newChar, &nibble) != 0) {
            log_error("Error in state eSER_RECV_STATE_SENDER!");
            resetIncomingOnError();
            break;
        } else {
            incomingMessage.sender |= (nibble << (4*digit));
        }
        if (incomingNumReceived >= 4) {
            incomingState = eSER_RECV_STATE_RECEIVER;
        }
        break;

    case eSER_RECV_STATE_RECEIVER:
        digit = 8 - incomingNumReceived;
        if (convertCharToNibble(newChar, &nibble) != 0) {
            log_error("Error in state eSER_RECV_STATE_RECEIVER!");
            resetIncomingOnError();
            break;
        } else {
            incomingMessage.receiver |= (nibble << (4*digit));
        }
        if (incomingNumReceived >= 8) {
            incomingState = eSER_RECV_STATE_LENGTH;
        }
        break;

    case eSER_RECV_STATE_LENGTH:
        digit = 10 - incomingNumReceived;
        if (convertCharToNibble(newChar, &nibble) != 0) {
            log_error("Error in state eSER_RECV_STATE_LENGTH char %x!", newChar);
            resetIncomingOnError();
            break;
        } else {
            incomingMessage.length |= (nibble << (4*digit));
        }
        if (incomingNumReceived >= 10) {
            incomingState = eSER_RECV_STATE_DATA;
        }
        break;

    case eSER_RECV_STATE_DATA:
        digit = incomingNumReceived % 2; // digit sequence: 1010...
        index = (incomingNumReceived - 11) / 2;
        if (convertCharToNibble(newChar, &nibble) != 0) {
            log_error("Error in state eSER_RECV_STATE_DATA!");
            resetIncomingOnError();
            break;
        } else {
            if (digit == 1) incomingMessage.data[index] = 0;
            incomingMessage.data[index] |= (nibble << (4*digit));
        }
        if (index + 1 == incomingMessage.length && digit == 0) {
            incomingState = eSER_RECV_STATE_NEWLINE;
        } else if (index + 1 > incomingMessage.length){
            log_error("Error in state eSER_RECV_STATE_DATA!");
            resetIncomingOnError();
        }
        break;

    case eSER_RECV_STATE_NEWLINE:
        if (incomingNumReceived != (incomingMessage.length * 2 + 11) ||
            newChar != '\n') {
            log_error("Error in state eSER_RECV_STATE_NEWLINE!");
            resetIncomingOnError();
        } else {
            // message received
            if (incomingNumReceived) incomingNumReceived--;
            incomingBuffer[incomingNumReceived] = '\0';
            log_msg(LOG_VERBOSE2, "SERIAL R RAW %s", incomingBuffer);
            incomingNumReceived = 0;
            incomingState = eSER_RECV_STATE_IDLE;
            return true;
        }
        break;

    default:
        log_error("Wrong receive state: %d", incomingState);
        resetIncomingOnError();
        break;
    }
    return false;
}

//----------------------------------------------------------------------------
int ConnectionSerial::continueSending()
{
    int rc = eERR_NONE;
    size_t toSend = 0;
    ssize_t written = 0;

    if (outgoingWritten < outgoingLength) {
        toSend = outgoingLength - outgoingWritten;
        written = sys_serial_send(fd,
                                  &outgoingBuffer[outgoingWritten],
                                  toSend);
        if (written < 0) {
            log_sys_error("SERIAL Send failed!");
            rc = eERR_SYSTEM;
        } else {
            outgoingWritten += written;
            if (outgoingWritten < outgoingLength) {
                rc = eRUNNING;
            }
        }
    }
    return rc;
}

/** @} */
