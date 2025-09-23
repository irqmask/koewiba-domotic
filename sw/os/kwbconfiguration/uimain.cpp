/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    uimain.cpp
 * @brief   UI main menu.
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

#include "uimain.h"

#include <ctime>
#include <iostream>
#include <string>

#include "kwbconfiguration_version.h"
#include "uisinglemodule.h"
#include "uiallmodules.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

//! application name
const std::string g_strProgram = "kwbconfiguration";

// --- Global variables --------------------------------------------------------

extern bool g_end_application;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

UIMain::UIMain(Application &app) : UIConsole(app)
{
}

//----------------------------------------------------------------------------
void UIMain::display()
{
    std::cout << std::endl;
    std::cout << g_strProgram << " V" << PGM_VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "Main Menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << " (1) single module >" << std::endl;
    std::cout << " (2) all modules >" << std::endl;
    std::cout << " (3) print modules list" << std::endl;
    std::cout << " (x) leave application" << std::endl;
}

//----------------------------------------------------------------------------
void UIMain::onMenuChoice()
{
    switch (this->last_choice) {
    case '1': {
        selectModule();
        UISingleModule uiSingleModule(app);
        uiSingleModule.run();
        break;
    }
    case '2': {
        UIAllModules uiAllModules(app);
        uiAllModules.run();
        break;
    }
    case 'x': {
        leave_menu = true;
        g_end_application = true;
        break;
    }
    default:
        UIConsole::onMenuChoice();
        break;
    }
}

//----------------------------------------------------------------------------
void UIMain::selectModule()
{
    app.selectModule(queryU16("Please enter module ID: ", 0x0001, 0x7F7F));
}



/** @} */
