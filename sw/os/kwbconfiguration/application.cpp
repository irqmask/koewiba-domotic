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

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

// os/libkwb
#include "exceptions.h"
#include "log.h"

#include "action_helper.hpp"
#include "action_read_register.h"
#include "action_write_register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

Application::Application(Connection     &conn,
                         MsgBroker      &broker)
    : msgEndpoint(conn)
    , msgBroker(broker)
    , selectedModule(0)
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
bool Application::readRegister(uint8_t registerId, int &value)
{
    try {
        ActionReadRegister action_read_reg(msgEndpoint, msgBroker, selectedModule, registerId);
        action_read_reg.start();
        action_read_reg.waitFinished();
        value = action_read_reg.getValue();
    }
    catch (Exception &e) {
        log_error("read register %d of node 0x%04x failed!\n%s\n", registerId, selectedModule, e.what());
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
bool Application::writeRegister(uint8_t registerId, int value)
{
    try {
        ActionWriteRegister action_write_reg(msgEndpoint, msgBroker, selectedModule, registerId);
        action_write_reg.setValue(value);

        // hack, remove when register layout is known to the program
        if (value > 65535) {
            action_write_reg.setRegisterFormat(eCMD_SET_REG_32BIT);
        }
        else if (value > 255) {
            action_write_reg.setRegisterFormat(eCMD_SET_REG_16BIT);
        }
        else {
            action_write_reg.setRegisterFormat(eCMD_SET_REG_8BIT);
        }

        action_write_reg.start();
        action_write_reg.waitFinished();
    }
    catch (Exception &e) {
        log_error("write register %d of node 0x%04x failed!\n%s\n", registerId, selectedModule, e.what());
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
bool Application::verifyRegister(uint8_t registerId, int value, int &readValue)
{
    try {
        readRegister(registerId, readValue);
        if (value != readValue) {
            throw OperationFailed(LOC, "read value %d does not match previous written value %d!", readValue, value);
        }
    }
    catch (Exception &e) {
        log_error("verify of register %d of node 0x%04x failed!\n%s\n", registerId, selectedModule, e.what());
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
template<typename ActionType, typename T, typename... Args>
bool Application::runAction(uint16_t nodeId, T &result, Args... args)
{
    try {
        runActionBlocking(this->msgEndpoint, this->msgBroker, nodeId, args...);
    }
    catch (Exception & e) {
        log_error("%s", e.what());
        return false;
    }
    return true;
}


/** @} */
