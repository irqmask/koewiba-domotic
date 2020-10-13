/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    Application.h
 * @brief   Application frontend. 
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@web.de>
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

// libkwb


#include "connection.h"
#include "ActionQueryModules.h"
#include "MsgBroker.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class Application {
public:
    Application(Connection& conn, MsgBroker& broker, bool& endApplication);
    
    bool detectModules();
    std::vector<ActionQueryModules::Module> getDetectedModules();
    void selectModule(uint16_t nodeid);
    uint16_t getSelectedModule();
    
    bool readRegister(uint8_t registerId, int& value);
    bool writeRegister(uint8_t registerId, int value);
    bool verifyRegister(uint8_t registerId, int value, int& readValue);
    int getLastRegisterValue();
    
    void end();
    
protected:
    std::vector<ActionQueryModules::Module> detected_modules;
    uint16_t            selectedModule;
    MsgBroker           &msgBroker;
    Connection          &msgEndpoint;
    bool                &endApplication;
};

// -----------------------------------------------------------------------------



/** @} */
