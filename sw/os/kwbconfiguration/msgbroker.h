/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    msgbroker.h
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
#include <vector>

#include "prjconf.h"

// include
#include "prjtypes.h"

// libkwb
#include "connection.h"
#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! Callback type for filter functions.
typedef std::function<bool(const msg_t &)> msg_filter_t;

//! Hold callbacks and reference to filter incomin messages to convey it to
//! interested entities.
typedef struct {
    //! Reference to identify this entry
    void           *reference;
    //! Filter function
    msg_filter_t    msg_filter;
    //! Incoming message handler
    incom_func_t    msg_handler;
} msg_filter_data_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Message broker class. Used to sort incoming messages to pending requests that
 * have been made before.
 */
class MsgBroker
{
public:
    /**
     * Default constructor
     */
    MsgBroker();

    /**
     * Handler for incoming message. Will distribute the messges to registered
     * listeners.
     *
     * @param[in]   message     Incoming message
     * @param[in]   reference   Reference to connection which received the
     *                          message.
     */
    void handleIncomingMessage(const msg_t &message, void *reference);

    /**
     * @brief registerForResponse
     * @param[in]   reference   Pointer to object which registered for response
     * @param[in]   filter_func Filter function to select messages to be conveyed
     * @param[in]   handler_func Message haandler function to which the filtered
     *                           message is passed to.
     */
    void registerForResponse(void *reference, msg_filter_t &filter_func, incom_func_t &handler_func);

    /**
     * Un-register from listening to messages.
     *
     * @param[in]   reference   Pointer to object which registered for response
     */
    void unregisterForResponse(void *reference);

protected:
    //! List of registered response filter/handlers.
    std::vector<msg_filter_data_t> response_handlers;
};

/** @} */
