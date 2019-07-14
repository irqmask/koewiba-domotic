/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionWithResponse.h
 * @brief   Base-class of an action which waits for a response to be performed with a bus-module. 
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

#include "ActionRequest.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class ActionWithResponse : public ActionRequest {
public:
    ActionWithResponse(MsgEndpoint &msgep, MsgBroker &broker, uint16_t nodeId=0);
    
    virtual void cancel();
    virtual bool waitForResponse();
    virtual bool isFinished();
    
protected:
    virtual bool formMessage() = 0;
    virtual bool filterResponse(msg_t& message) = 0;
    virtual void handleResponse(msg_t& message) = 0;
    
    void log_module_info();
    bool     messageReceived; //!< Flag if message has been received.
    msg_t    receivedMessage; //!< Message, which has been received during this action.
};

/** @} */
