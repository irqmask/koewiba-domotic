/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    MsgEndpoint.cpp
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
 
// --- Include section ---------------------------------------------------------

#include "prjconf.h"

// include
#include "prjtypes.h"

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

#include "MsgEndpoint.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

MsgEndpoint::MsgEndpoint(ioloop_t& ioloop, uint16_t own_node_id=0) : ioloop(ioloop),
                                                                     ownNodeId(own_node_id)
{
    message_distribution_list.clear();
}

void MsgEndpoint::registerForIncommingMessages(msg_handler_t& func)
{
    this->message_distribution_list.push_back(func);
}

void MsgEndpoint::handleIncommingMessage(msg_t& message)
{
   for (auto receiver : this->message_distribution_list) {
        receiver(message);
   }
}

void MsgEndpoint::setOwnNodeId(uint16_t nodeId)
{
    ownNodeId = nodeId;
}
    
uint16_t MsgEndpoint::getOwnNodeId()
{
    return ownNodeId;
}

/** @} */
