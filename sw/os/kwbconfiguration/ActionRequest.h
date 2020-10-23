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

/**
 * Action to send a request / command to a bus module. A response is not
 * expected in this case.
 */
class ActionRequest : public Action
{
public:
    /**
     * Constructor
     * @param[in]   conn        Reference to established connection to a
     *                          KWB bus os router
     * @param[in]   broker      Reference to message broker.
     * @param[in]   moduleAddr  (optional, default=0) Module address to communicate with.
     */
    ActionRequest(Connection &conn, MsgBroker &broker, uint16_t moduleAddr = 0);

    bool start() override;
    virtual void cancel() override;
    virtual bool isFinished() override;

    /**
     * Set the address of the module to communicate with.
     * @param[in]   address     Module address.
     */
    void setModuleAddress(uint16_t address);

    /**
     * Get the currently set address of the recepient bus module.
     * @return module address
     */
    uint16_t getModuleAddress();

protected:
    /**
     * Form a message to be sent to the bus module.
     * @returns true, if the messge was successfully formed, otherwise false.
     */
    virtual bool formMessage() = 0;

    //! Address of module to perform action with.
    uint16_t moduleAddr;
    //! Message to be sent during this action.
    msg_t    messageToSend;
};

/** @} */
