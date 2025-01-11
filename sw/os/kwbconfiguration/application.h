/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    application.h
 * @brief   Application frontend.
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

#include <iostream>
#include <list>

// include
#include "prjtypes.h"

// os/libkwb
#include "connection.h"
#include "msgbroker.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Application interface to UI of every action the application can perform.
 */
class Application
{
public:
    /**
     * @brief Application
     * @param[in]   conn            Reference to connection to kwbrouter or serial gateway
     * @param[in]   broker          Reference to message broker
     */
    Application(Connection &conn, MsgBroker &broker);

    MsgBroker &getMsgBroker() { return msgBroker; };

    Connection &getMsgEndpoint() { return msgEndpoint; };

    template <typename ActionType, typename T, typename...  Args>
    bool runAction(uint16_t nodeId, T &result, Args... args);

    /**
     * Select the module to work with.
     *
     * @param[in]   moduleAddr      The address of the module
     */
    void selectModule(uint16_t moduleAddr);

    /**
     * @return the address of the current selected module
     */
    uint16_t getSelectedModule();

    /**
     * Read from a register of a module.
     *
     * @param[in]   registerId  Id of the register to read.
     * @param[out]  value       The read value
     *
     * @return true, if the register has successfully been read, otherwise false.
     */
    bool readRegister(uint8_t registerId, int &value);

    /**
     * Write into a register of a module.
     *
     * @param[in]   registerId  Id of the register to write.
     * @param[in]   value       Value to write into the register
     *
     * @return true, if the value has successfully been written into the register,
     *         otherwise false.
     */
    bool writeRegister(uint8_t registerId, int value);

    /**
     * Verify a register value against a given value.
     *
     * @param[in]   registerId  Register Id to read the value from
     * @param[in]   value       Expected value
     * @param[out]  readValue   Value read from the register
     *
     * @return true, if the read value matches the expected value, otherwise false.
     */
    bool verifyRegister(uint8_t registerId, int value, int &readValue);

protected:
    //! Currently selected module
    uint16_t            selectedModule;
    //! Referenc to message broker
    MsgBroker           &msgBroker;
    //! Reference to connection to the KWB sytsem
    Connection          &msgEndpoint;
};

// -----------------------------------------------------------------------------



/** @} */
