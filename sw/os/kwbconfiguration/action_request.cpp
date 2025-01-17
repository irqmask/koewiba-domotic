/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    action_request.cpp
 * @brief   Base-class of an action to be performed with a bus-module.
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@web.de>
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

#include "action_request.h"

#include <chrono>
#include <functional>
#include <thread>

// include
#include "prjconf.h"
// libkwb
#include "exceptions.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

ActionRequest::ActionRequest(Connection   &conn,
                             MsgBroker    &broker,
                             uint16_t     moduleAddr)
    : Action(conn, broker)
    , moduleAddr(moduleAddr)
    , messageToSend({0})
{
}

//----------------------------------------------------------------------------
void ActionRequest::start()
{
    try {
        Action::start();
        formMessage();
        connection.send(messageToSend);
    }
    catch (Exception &e) {
        throw OperationFailed(LOC, "Cannot start action! Error occured:\n%s", e.what());
    }
}

//----------------------------------------------------------------------------
bool ActionRequest::isFinished()
{
    return timeoutOccurred;
}

//----------------------------------------------------------------------------
void ActionRequest::waitFinished()
{
    checkTimeout();
}

//----------------------------------------------------------------------------
void ActionRequest::cancel()
{
    // nothing to cancel here
}

/** @} */
