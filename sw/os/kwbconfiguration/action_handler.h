/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    action_handler.h
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
#pragma once

// --- Include section ---------------------------------------------------------

#include <functional>
#include <mutex>
#include <vector>

#include "prjconf.h"

// include
#include "prjtypes.h"

// libkwb
#include "connection.h"
#include "message.h"

#include "cmd_handler.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Message broker class. Used to sort incoming messages to pending requests that
 * have been made before.
 */
class ActionHandler : public CommandHandler
{
public:
    /**
     * Default constructor
     */
    ActionHandler();

    /**
     * Handler for incoming message. Will distribute the messges to registered
     * listeners.
     *
     * @param[in]   message     Incoming message
     * @param[in]   reference   Reference to connection which received the
     *                          message.
     */
    void handleIncomingMessage(const msg_t &message, void *reference);
};

/** @} */
