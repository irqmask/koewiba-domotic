/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    MsgEndpointSerial.h
 * @brief   Endpoint interface to send and receive messages. 
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@web.de>
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

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

// include
#include "prjtypes.h"
// os/shared
#include "ioloop.h"
#include "message_serial.h"

#include "MsgEndpoint.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class MsgEndpointSerial : public MsgEndpoint {
public:
    MsgEndpointSerial(ioloop_t& ioloop, uint16_t ownNodeId=0, std::string deviceName="", uint32_t baudrate=57600);
    ~MsgEndpointSerial();
    
    virtual bool connect();
    virtual bool disconnect();
    virtual bool sendMessage(msg_t &message);
    
    void setDevice(std::string device_name);
    std::string getDevice();
    
    void setBaudrate(uint32_t baudrate);
    uint32_t getBaudrate();
    
protected:
    std::string     deviceName;
    uint32_t        baudrate;
    
    msg_serial_t    msgSerialHandle;
};

/** @} */
