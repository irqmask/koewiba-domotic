/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    UIConsole.h
 * @brief   UI for console.
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

#include <iostream>

// include
#include "prjconf.h"
#include "prjtypes.h"
#include "Application.h"
#include "UIConsole.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * User interface: Main menu
 */
class UISingleModule : public UIConsole
{
public:
    /**
     * Constructor
     * @param[in]   app         Reference to belonging application
     */
    UISingleModule(Application &app);

protected:
    virtual void display() override;
    virtual void onMenuChoice() override;

private:
    void readVersion();
    void readRegister();
    void writeRegister();
    void backupModule();
    void restoreModule();
    void setTime();
    void getTime();
};

/** @} */
