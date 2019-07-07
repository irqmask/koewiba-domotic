/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionRequest.h
 * @brief   Base-class of an action which sends a request / message to a bus-module. 
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

#include "Action.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class ActionRequest : public Action {
public:
    ActionRequest(MsgEndpoint &msgep, MsgBroker &broker, uint16_t nodeId=0);
    
    bool start();
    virtual void cancel();
    virtual bool isFinished();
        
    void setNodeID(uint16_t id);
    uint16_t getNodeID();
    
protected:
    virtual bool formMessage() = 0;
    
    uint16_t nodeId; //! ID of node to perform action with.
    msg_t    messageToSend; //!< Message to be sent during this action.
};

/** @} */
