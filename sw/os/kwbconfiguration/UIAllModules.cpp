/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    UIAllModules.cpp
 * @brief   UI sub-menu for actions on all modules.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
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

#include "UIAllModules.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

UIAllModules::UIAllModules(Application &app) : UIConsole(app)
{
}

//----------------------------------------------------------------------------
void UIAllModules::display()
{
    std::cout << std::endl;
    std::cout << "All modules Menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    //std::cout << " (1) ping all" << std::endl;
    std::cout << " (2) read all versions" << std::endl;
    std::cout << " (4) backup all registers" << std::endl;
    std::cout << " (5) restore all registers" << std::endl;
    std::cout << " (x) leave sub-menu" << std::endl;
}

//----------------------------------------------------------------------------
void UIAllModules::onMenuChoice()
{
    switch (this->last_choice) {
    case 'x':
        leave_menu = true;
        break;
    default:
        UIConsole::onMenuChoice();
        break;
    }
}
