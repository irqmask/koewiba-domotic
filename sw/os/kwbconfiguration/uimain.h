/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    uimain.h
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
#pragma once

// --- Include section ---------------------------------------------------------

#include <iostream>

// include
#include "prjconf.h"
#include "prjtypes.h"
#include "application.h"
#include "uiconsole.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * User interface: Main menu
 */
class UIMain: public UIConsole
{
public:
    /**
     * Constructor
     * @param[in]   app         Reference to belonging application
     */
    UIMain(Application &app);

protected:
    virtual void display() override;
    virtual void onMenuChoice() override;

private:
    void selectModule();

};

/** @} */
