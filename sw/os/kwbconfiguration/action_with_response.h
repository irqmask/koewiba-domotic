/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    action_with_response.h
 * @brief   Base-class of an action which waits for a response to be performed with a bus-module.
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

#include "prjconf.h"

// include
#include "prjtypes.h"

#include "action_request.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Action to send a request / command to a bus module. A response is not
 * expected in this case.
 */
class ActionWithResponse : public ActionRequest
{
public:
    /**
     * Constructor
     * @param[in]   conn        Reference to established connection to a
     *                          KWB bus os router
     * @param[in]   moduleAddr  (optional, default = 0) Module address to communicate with.
     */
    ActionWithResponse(std::shared_ptr<Connection> conn, uint16_t moduleAddr = 0);

    virtual void abort();

protected:
    friend class ActionHandler;
    virtual bool formMessage() override = 0;

    /**
     * @brief filterResponse
     * @param message
     * @return
     */
    virtual bool filterResponse(const msg_t &message) = 0;

    /**
     * Handle the incoming response
     *
     * @param[in]   message     Received message which is the response to a
     *                          previously sent command.
     * @param[in]   reference   Reference given by libkwb connections incoming
     *                          message handler.
     */
    virtual void processResponse(const msg_t &message, void *reference) = 0;

    //! Message, which has been received during this action.
    msg_t    receivedMessage;
};

/** @} */
