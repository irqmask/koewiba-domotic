/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    Action.cpp
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

#include "ActionWithResponse.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

ActionWithResponse::ActionWithResponse(Connection   &conn,
                                       MsgBroker    &broker, 
                                       uint16_t     nodeId)
    : ActionRequest(conn, broker, nodeId)
    , receivedMessage({0})
    , messageReceived(false)
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    msg_filter_t filterResponseFunc = std::bind(&ActionWithResponse::filterResponse, this, _1);
    incom_func_t handleResponseFunc = std::bind(&ActionWithResponse::handleResponse, this, _1, _2);
 
    msgBroker.registerForResponse(this, filterResponseFunc, handleResponseFunc);
}

void ActionWithResponse::cancel()
{
    msgBroker.unregisterForResponse(this);
}

bool ActionWithResponse::isFinished()
{
    return timeout_elapsed || messageReceived;
}

bool ActionWithResponse::waitForResponse()
{
    auto start = std::chrono::high_resolution_clock::now();
    
    while (!messageReceived) {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        std::this_thread::yield(); // better wait for an event
        if (elapsed > timeout) {
            timeout_elapsed = true;
            cancel(); 
            break;
        }
    }
    return messageReceived;
}

/** @} */
