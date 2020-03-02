/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionRequest.cpp
 * @brief   Base-class of an action to be performed with a bus-module. 
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

#include <chrono>
#include <functional>
#include <thread>

#include "prjconf.h"

#include "ActionRequest.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

ActionRequest::ActionRequest(MsgEndpoint  &msgep, 
                             MsgBroker    &broker, 
                             uint16_t     nodeId) : Action(msgep, broker),
                                                    nodeId(nodeId),
                                                    messageToSend({0})
{
}

bool ActionRequest::start()
{
    if (formMessage()) {
        return msgEndpoint.sendMessage(messageToSend);
    }
    return false;
}

void ActionRequest::cancel()
{
}

bool ActionRequest::isFinished()
{
    return timeout_elapsed;
}

/** @} */
