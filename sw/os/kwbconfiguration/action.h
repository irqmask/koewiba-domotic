/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    Action.h
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
#pragma once

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <chrono>

// include
#include "prjtypes.h"
#include "msgbroker.h"
#include "connection.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Base-class for "actions". An action in this context is a command / response
 * sequence. The action is finished  when the command has been sent and a
 * response has been received.
 */
class Action
{
public:
    /**
     * Constructor
     * @param[in]   conn        Reference to established connection to a
     *                          KWB bus os router
     * @param[in]   broker      Reference to message broker.
     */
    Action(Connection &conn, MsgBroker &broker);

    /**
     * Start the action.
     * @throws OperationFailed if action cannot be started.
     */
    virtual void start();

    /**
     * @returns true if the action is finished, otherwise false.
     */
    virtual bool isFinished();

    /**
     * Advance the action until it is finished.
     * Call advance() until it returns false. Then the action is finished.
     * If the action throws an Exception it is automatically finished, too.
     *
     * @throws OperationFailed when a action failed during execution
     * @throws Timeout whe the action timed out.
     */
    virtual void waitFinished() = 0;

    /**
     * Cancel the running action.
     */
    virtual void cancel() = 0;

    /**
     * @returns true if the action has timed out, otherwise false.
     */
    virtual bool hasTimedOut();

protected:
    //! Measure time since action start and throw Timeout exception once the timeout duration exceeds.
    void checkTimeout();

    //! Reference to established connection.
    Connection                  &connection;
    //! Reference to message broker.
    MsgBroker                   &msgBroker;
    //! Duration until action times out.
    std::chrono::duration<int>  timeout;
    //! Start timepoint
    std::chrono::system_clock::time_point startTime;
    //! Flag if timeout occurred.
    bool                        timeoutOccurred;
    //! Flag if action is still running
    bool                        isRunning;
};

/** @} */
