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
     * @param[in]   broker      Reference to message broker.
     * @param[in]   moduleAddr  (optional, default = 0) Module address to communicate with.
     */
    ActionWithResponse(Connection &conn, MsgBroker &broker, uint16_t moduleAddr = 0);

    virtual void cancel() override;
    virtual bool isFinished() override;

    /**
     * Wait until the response has been received or timeour occurred.
     * @returns true if response has been received, otherwise false.
     */
    virtual bool waitForResponse();

protected:
    virtual bool formMessage() = 0;

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
    virtual void handleResponse(const msg_t &message, void *reference) = 0;

    //! Flag if message has been received.
    bool     messageReceived;
    //! Message, which has been received during this action.
    msg_t    receivedMessage;
};

/** @} */
