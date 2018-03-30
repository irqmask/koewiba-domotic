/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    Application.cpp
 * @brief   Application frontend. 
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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

#include "Application.h"
#include "ActionReadRegister.h"
#include "ActionWriteRegister.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

Application::Application(MsgEndpoint&   msgep, 
                         MsgBroker&     broker, 
                         bool&          end) : end_application(end),
                                               msgEndpoint(msgep),
                                               msgBroker(broker),
                                               selected_module(0)
{
    this->detected_modules.clear();
}

void Application::detectModules()
{
    this->detected_modules.clear();
}

std::list<uint16_t> Application::getDetectedModules()
{
    return this->detected_modules;
}

void Application::selectModule(uint16_t module_id)
{
    this->selected_module = module_id;
}

uint16_t Application::getSelectedModule()
{
    return this->selected_module;
}

bool Application::readRegister(uint8_t registerId, int& value)
{
    bool rc = false;
    ActionReadRegister action_read_reg(msgEndpoint, msgBroker, selected_module, registerId);
    if ((rc = action_read_reg.start()) == true) {
        rc = action_read_reg.waitForResponse();
    }
    value = action_read_reg.getValue();
    return rc;
}

bool Application::writeRegister(uint8_t registerId, int value)
{
    bool rc = false;
    ActionWriteRegister action_write_reg(msgEndpoint, msgBroker, selected_module, registerId);
    action_write_reg.setValue(value);
    
    // hack, remove when register layout is known to the program
    if (value > 65535) action_write_reg.setRegisterFormat(eCMD_SET_REG_32BIT);
    else if (value > 255) action_write_reg.setRegisterFormat(eCMD_SET_REG_16BIT);
    else action_write_reg.setRegisterFormat(eCMD_SET_REG_8BIT);

    return action_write_reg.start();
}

bool Application::verifyRegister(uint8_t registerId, int value, int& readValue)
{
    if (!readRegister(registerId, readValue) || (value != readValue)) return false;
    return true;
}

int Application::getLastRegisterValue()
{
    return 0;
}

void Application::endApplication()
{
    std::cout << "ending..." << std::endl;
    end_application = true;
}

/** @} */