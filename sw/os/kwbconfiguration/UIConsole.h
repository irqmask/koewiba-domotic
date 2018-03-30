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
#pragma once 

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <iostream>

// include
#include "prjtypes.h"
#include "Application.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class UIConsole {
public:
    UIConsole(Application &app);
    
    void run();
    
protected:
    virtual void display();
    virtual void queryInput();
    virtual void onMenuChoice();
    
    virtual char queryChar(std::string queryText);
    virtual uint32_t queryU32(std::string query_text, uint32_t lowest_allowed, uint32_t highest_allowed);
    virtual uint16_t queryU16(std::string query_text, uint16_t lowest_allowed, uint16_t highest_allowed);
    virtual uint8_t queryU8(std::string query_text, uint8_t lowest_allowed, uint8_t highest_allowed);
    virtual int32_t queryI32(std::string query_text, int32_t lowest_allowed, int32_t highest_allowed);
    virtual int16_t queryI16(std::string query_text, int16_t lowest_allowed, int16_t highest_allowed);
    virtual int8_t queryI8(std::string query_text, int8_t lowest_allowed, int8_t highest_allowed);


    
    char        last_choice;
    bool        leave_menu;
    
    Application &app;
};

// -----------------------------------------------------------------------------

class UIMain: public UIConsole {
public:
    UIMain(Application &app);
    
protected:
    virtual void display();
    virtual void onMenuChoice();
private:
    void selectModule();
    void readRegister();
    void writeRegister();
};

// -----------------------------------------------------------------------------

class UIReadRegister: public UIConsole {
public:
    UIReadRegister(Application &app);
 
protected:
    virtual void display();
    virtual void onMenuChoice();
};

/** @} */
