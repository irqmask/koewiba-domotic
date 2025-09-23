/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    connection_serial.h
 * @brief   Send and receive KWB messages over a serial connection.
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
#include "sysserial.h"

/**
 * This class implements sending and receiving KWB messages over a serial
 * connection.
 */
class ConnectionSerial : public Connection
{
public:
    /**
     * Maximum size of serial message:
     * [16bit receiver] + [8bit length] + [message data] + NL + 0
     * 4 + 2 + MAX_MSG_SIZE * 2 + 1 + 1
     */
    static constexpr size_t MAX_SERIAL_MSG_SIZE = (6 + MAX_MSG_SIZE * 2 + 2);

    /**
     * Open and initializes a serial connection.
     *
     * @param[in]   io          Pointer to ioloop.
     * @param[in]   uri         Devicename of serial device.
     * @param[in]   configure   Configure serial parameters e.g. baudrate (default true)
     */
    ConnectionSerial(ioloop_t *io, const std::string &uri, bool configure = true);

    /**
     * Closes active connection.
     * @note Does not call onConnectionClosed()!
     */
    ~ConnectionSerial();

    /**
     * @returns baudrate of established connection.
     */
    uint32_t getBaudrate();

    /**
     * Send a KWB messgae over the serial connection.
     * @param[in]   message     KWB message to be sent
     */
    virtual void send(const msg_t &message) override;

protected:
    /**
     * Open the connection to the serial device.
     * @param[in]   configure   (optional) Configure serial device (baudrate
     *                          etc.) The default value is true.
     */
    void open(bool configure = true);

    /**
     * Close the connection to the serial device.
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

    /**
     * convert a byte into two character hexadecimal ascii representation
     * @param[out]  buffer      Buffer to stor converted byte in.
     * @param[in]   offset      Offset in buffer where to start writing
     * @param[in]   byte        Byte to convert
     */
    void convertByteToAscii(char *buffer, uint8_t *offset, uint8_t byte);

    /**
     * Convert a character to a nibble (value 0..15)
     * @param[in]   c           Character to convert
     * @param[out]  nibble      Converted char as nibble
     * @return 0 if successful
     */
    int convertCharToNibble(char c, uint8_t *nibble);

    /**
     * Conver message to serial readable representation
     * @param[out]  buffer      Buffer which will contain formatted message
     * @param[in]   buffersize  Maximum buffer size
     * @param[in]   message     Message to convert to serial
     * @return size of serialized message.
     */
    size_t formatSerialMessage(char *buffer, uint8_t buffersize, const msg_t &message);

    /**
     * Reset incoming message state and buffer.
     */
    void resetIncomingOnError();

    /**
     * Process incoming characters to form a complete message.
     * @param[in]   newChar     New character of message.
     * @return true if a new message is complete.
     */
    bool processReceiving(char newChar);

    /**
     * Continue sending.
     * @returns eERR_NONE if outgoing message was completely sent, eRUNNING if
     *          message is still being sent or eERR_SYSTEM if sending failed.
     */
    int continueSending();

protected:
    //! States of receiving portions of a message
    enum receiveState {
        eSER_RECV_STATE_IDLE,
        eSER_RECV_STATE_SENDER,
        eSER_RECV_STATE_RECEIVER,
        eSER_RECV_STATE_LENGTH,
        eSER_RECV_STATE_DATA,
        eSER_RECV_STATE_NEWLINE
    };

    //! Baudrate of serial device
    uint32_t    baudrate;
    //! File descriptor of the serial connection
    sys_fd_t    fd;

    //! Buffer for incoming messages
    char        incomingBuffer[MAX_SERIAL_MSG_SIZE * 2];
    //! Complete received incoming message
    msg_t       incomingMessage;
    //! Incoming message state
    receiveState incomingState;
    //! Received bytes of currently incoming message
    uint8_t     incomingNumReceived;

    //! Buffer for outgoing messages
    char        outgoingBuffer[MAX_SERIAL_MSG_SIZE];
    //! Length in bytes of outgoing message
    size_t      outgoingLength;
    //! Number of bytes written of currently outgoing message.
    size_t      outgoingWritten;
};

/** @} */
