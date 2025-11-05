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

#include <array>
#include <chrono>
#include <iostream>
#include <list>

// include
#include "prjtypes.h"

// os/libkwb
#include "connection.h"

// os/kwbconfiguration
#include "action_handler.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

struct ModuleInfo {
    uint16_t nodeId;

    std::array<uint8_t, 4> controllerId;
    uint16_t boardId;
    uint8_t boardRev;
    uint16_t appId;
    uint8_t majorVersion;
    uint8_t minorVersion;
    uint8_t bugfixVersion;
    uint32_t versionHash;
};

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
    Application(std::shared_ptr<Connection> conn, std::shared_ptr<CommandHandler> cmdhandler);

    std::shared_ptr<Connection> getMsgEndpoint()
    {
        return msgEndpoint;
    };

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
     * @throws OperationFailed if reading a register value failed.
     */
    void readRegister(uint16_t moduleId, uint8_t registerId, int32_t &value);

    /**
     * Write into a register of a module.
     *
     * @param[in]   registerId  Id of the register to write.
     * @param[in]   value       Value to write into the register
     *
     * @throws OperationFailed if writing to the register failed.
     */
    void writeRegister(uint16_t moduleId, uint8_t registerId, int32_t value);

    /**
     * Verify a register value against a given value.
     *
     * @param[in]   registerId  Register Id to read the value from
     * @param[in]   value       Expected value
     * @param[out]  readValue   Value read from the register
     *
     * @throws OperationFailed if reading failed or read value does not match expected value.
     */
    void verifyRegister(uint16_t moduleId, uint8_t registerId, int value, int &readValue);

    /**
     * Read module information from modules registers.
     *
     * @param[in]   moduleId    Register Id to read the value from
     * @param[out]  mi          Structure containing the module information
     *
     * @throws OperationFailed if reading a register value failed.
     */
    void readModuleInfo(uint16_t moduleId, ModuleInfo &mi);

protected:
    static constexpr std::chrono::seconds defaultTimeout = std::chrono::seconds(2);

    //! Currently selected module
    uint16_t                        selectedModule;
    //! Reference to command handler
    std::shared_ptr<CommandHandler> handler;

    //std::shared_ptr<MsgBroker>      msgBroker;
    //! Reference to connection to the KWB sytsem
    std::shared_ptr<Connection>     msgEndpoint;

};

// -----------------------------------------------------------------------------



/** @} */
