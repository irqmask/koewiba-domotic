/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    MsgEndpointSerial.cpp
 * @brief   Endpoint to send and receive messages over a serial line. 
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@gmx.de>
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
 
// --- Include section ---------------------------------------------------------

#include "prjconf.h"

// include
#include "prjtypes.h"

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

#include "MsgEndpointSerial.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

/**
 * Callback handler for incomming messages.
 */
static void incommingMessageHdl(msg_t* message, void* reference, void* arg)
{
    MsgEndpointSerial* msgep = (MsgEndpointSerial*)arg;
    msgep->handleIncommingMessage(*message);
}

MsgEndpointSerial::MsgEndpointSerial(ioloop_t&      ioloop, 
                                     uint16_t       ownNodeId,
                                     std::string    deviceName,
                                     uint32_t       baudrate) : MsgEndpoint(ioloop, ownNodeId),
                                                                deviceName(deviceName),
                                                                baudrate(baudrate)
{
    msg_ser_init(&this->msgSerialHandle);
    msg_ser_set_incomming_handler(&this->msgSerialHandle, incommingMessageHdl, this);
}

/**
 * Clean-up. Close connection, when object is deleted.
 */
MsgEndpointSerial::~MsgEndpointSerial()
{
    msg_ser_close(&this->msgSerialHandle);
}

/** 
 * Open serial connection.
 */
bool MsgEndpointSerial::connect()
{
    int rc = msg_ser_open(&this->msgSerialHandle, &this->ioloop, this->deviceName.data(), this->baudrate);
    if (rc != eERR_NONE) return false;
    return true;
}

/**
 * Close serial connection.
 */
bool MsgEndpointSerial::disconnect()
{
    msg_ser_close(&this->msgSerialHandle);
    return true;
}

bool MsgEndpointSerial::sendMessage(msg_t &message)
{
    int rc = msg_ser_send(&this->msgSerialHandle, &message);
    if (rc != eERR_NONE) return false;
    return true;
}

void MsgEndpointSerial::setDevice(std::string device_name)
{
    this->deviceName = device_name;
}

std::string MsgEndpointSerial::getDevice()
{
    return this->deviceName;
}
    
void MsgEndpointSerial::setBaudrate(uint32_t baudrate)
{
    this->baudrate = baudrate;
}

uint32_t MsgEndpointSerial::getBaudrate()
{
    return this->baudrate;
}

/** @} */
