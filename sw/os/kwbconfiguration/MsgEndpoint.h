/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    MsgEndpoint.h
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

#include <functional>
#include <vector>

#include "prjconf.h"

// include
#include "prjtypes.h"
// os/shared
#include "ioloop.h"
#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef std::function<void(msg_t&)> msg_handler_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class MsgEndpoint {
public:
    MsgEndpoint(ioloop_t& ioloop, uint16_t node_id);
    
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool sendMessage(msg_t& message) = 0;

    void registerForIncommingMessages(msg_handler_t& func);
    void handleIncommingMessage(msg_t& message);
   
    void setOwnNodeId(uint16_t nodeId);
    uint16_t getOwnNodeId();
    
protected:
    ioloop_t&                   ioloop;
    uint16_t                    ownNodeId;
    std::vector<msg_handler_t>  message_distribution_list;
    
};

/** @} */
