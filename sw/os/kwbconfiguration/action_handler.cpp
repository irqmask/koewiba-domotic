/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    action_handler.cpp
 * @brief   Broker which sorts incomming messages to running actions.
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

#include "prjconf.h"
#include "action_handler.h"

// include
#include "prjtypes.h"

#include "log.h"

#include "action_with_response.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

//----------------------------------------------------------------------------
ActionHandler::ActionHandler()
{
}

//----------------------------------------------------------------------------
void ActionHandler::handleIncomingMessage(const msg_t &message, void *reference)
{
    std::lock_guard<std::recursive_mutex> lock(mutex);

    for (auto action : this->runQueue) {
        auto awr = std::dynamic_pointer_cast<ActionWithResponse>(action);
        if (awr == nullptr) {
            continue;
        }
        if (awr->filterResponse(message)) {
            awr->processResponse(message, awr.get());
        }
    }
}

/** @} */
