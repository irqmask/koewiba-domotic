/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    application.cpp
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

// --- Include section ---------------------------------------------------------

#include "application.h"
#include "prjconf.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <safe_lib.h>
    #include <unistd.h>
#endif

// include
#include "prjtypes.h"
#include "moddef_common.h"

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

// os/libkwb
#include "exceptions.h"
#include "log.h"

#include "action_handler.h"
#include "action_read_register.h"
#include "action_write_register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

constexpr std::chrono::seconds Application::defaultTimeout;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

Application::Application(std::shared_ptr<Connection> conn,
                         std::shared_ptr<CommandHandler> cmdhandler)
    : selectedModule(0)
    , msgEndpoint(conn)
    , handler(cmdhandler)
{
}

//----------------------------------------------------------------------------
void Application::selectModule(uint16_t module_id)
{
    this->selectedModule = module_id;
}

//----------------------------------------------------------------------------
uint16_t Application::getSelectedModule()
{
    return this->selectedModule;
}

// obsolete repeating every action in application -> better move into dedicated actions

//----------------------------------------------------------------------------
void Application::readRegister(uint16_t moduleId, uint8_t registerId, int32_t &value)
{
    try {
        if (moduleId == 0) {
            moduleId = selectedModule;
        }
        auto cmd = handler->start<ActionReadRegister>(msgEndpoint, moduleId, registerId);
        handler->waitFinished(cmd->id(), this->defaultTimeout);
        value = cmd->getValue();
    }
    catch (Exception &e) {
        throw OperationFailed(LOC, "read register %d of node 0x%04x failed!\n%s", registerId, moduleId, e.what());
    }
}

//----------------------------------------------------------------------------
void Application::writeRegister(uint16_t moduleId, uint8_t registerId, int32_t value)
{
    try {
        if (moduleId == 0) {
            moduleId = selectedModule;
        }
        auto cmd = handler->start<ActionWriteRegister>(msgEndpoint, moduleId, registerId, value);
        handler->waitFinished(cmd->id(), this->defaultTimeout);
    }
    catch (Exception &e) {
        throw OperationFailed(LOC, "write register %d of node 0x%04x failed!\n%s", registerId, moduleId, e.what());
    }
}

//----------------------------------------------------------------------------
void Application::verifyRegister(uint16_t moduleId, uint8_t registerId, int value, int &readValue)
{
    try {
        if (moduleId == 0) {
            moduleId = selectedModule;
        }
        readRegister(moduleId, registerId, readValue);
        if (value != readValue) {
            throw OperationFailed(LOC, "read value %d does not match previous written value %d!", readValue, value);
        }
    }
    catch (Exception &e) {
        throw OperationFailed(LOC, "verify of register %d of node 0x%04x failed!\n%s", registerId, moduleId, e.what());
    }
}

//----------------------------------------------------------------------------
void Application::readModuleInfo(uint16_t moduleId, ModuleInfo &mi)
{
    if (moduleId == 0) {
        moduleId = selectedModule;
    }
    int32_t regval;
    readRegister(moduleId, MOD_eReg_BoardID, regval);
    mi.boardId = regval;
    readRegister(moduleId, MOD_eReg_BoardRev, regval);
    mi.boardRev = regval;
    readRegister(moduleId, MOD_eReg_AppID, regval);
    mi.appId = regval;
    readRegister(moduleId, MOD_eReg_AppVersionMajor, regval);
    mi.majorVersion = regval;
    readRegister(moduleId, MOD_eReg_AppVersionMinor, regval);
    mi.minorVersion = regval;
    readRegister(moduleId, MOD_eReg_AppVersionBugfix, regval);
    mi.bugfixVersion = regval;
    readRegister(moduleId, MOD_eReg_AppVersionHash, regval);
    mi.versionHash = regval;
    readRegister(moduleId, MOD_eReg_DeviceSignature0, regval);
    mi.controllerId[0] = regval;
    readRegister(moduleId, MOD_eReg_DeviceSignature1, regval);
    mi.controllerId[1] = regval;
    readRegister(moduleId, MOD_eReg_DeviceSignature2, regval);
    mi.controllerId[2] = regval;
    readRegister(moduleId, MOD_eReg_DeviceSignature3, regval);
    mi.controllerId[3] = regval;
}

/** @} */
