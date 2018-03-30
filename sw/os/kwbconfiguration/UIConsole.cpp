/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    UIConsole.cpp
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
 
// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <iostream>
#include <string>

#include <limits.h>


// include
#include "prjtypes.h"

#include "kwbconfiguration_version.h"
#include "UIConsole.h"


// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

const std::string g_strProgram = "kwbconfiguration";

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

UIConsole::UIConsole(Application &app) : app(app)
{
    this->leave_menu = false;
    this->last_choice = '\0';
}

void UIConsole::run()
{
    while (!this->leave_menu) {
        display();
        queryInput();
        onMenuChoice();
    }
}

void UIConsole::display()
{
}

uint32_t UIConsole::queryU32(std::string query_text, uint32_t lowest_allowed=0, uint32_t highest_allowed=UINT_MAX)
{
    bool input_is_valid = false;
    std::string user_input;
    uint32_t value = 0;
    
    do {
        try {
            std::cout << query_text;
            std::cin >> user_input;
            value = std::stoul(user_input, 0, 0);
            
            if (value >= lowest_allowed && value <= highest_allowed) {
                input_is_valid = true;
            }
        }
        catch (std::exception &e) { }

        if (!input_is_valid) std::cout << "invalid value entered!" << std::endl;
        
    } while (!input_is_valid);
    return value;
}

uint16_t UIConsole::queryU16(std::string query_text, uint16_t lowest_allowed=0, uint16_t highest_allowed=USHRT_MAX)
{
    return (uint16_t)queryU32(query_text, lowest_allowed, highest_allowed);
}

uint8_t UIConsole::queryU8(std::string query_text, uint8_t lowest_allowed=0, uint8_t highest_allowed=UCHAR_MAX)
{
    return (uint8_t)queryU32(query_text, lowest_allowed, highest_allowed);
}

int32_t UIConsole::queryI32(std::string query_text, int32_t lowest_allowed=INT_MIN, int32_t highest_allowed=INT_MAX)
{
    bool input_is_valid = false;
    std::string user_input;
    int32_t value = 0;
    
    do {
        try {
            std::cout << query_text;
            std::cin >> user_input;
            value = std::stol(user_input, 0, 0);
            
            if (value >= lowest_allowed && value <= highest_allowed) {
                input_is_valid = true;
            }
        }
        catch (std::exception &e) { }

        if (!input_is_valid) std::cout << "invalid value entered!" << std::endl;
        
    } while (!input_is_valid);
    return value;
}

int16_t UIConsole::queryI16(std::string query_text, int16_t lowest_allowed=SHRT_MIN, int16_t highest_allowed=SHRT_MAX)
{
    return (int16_t)queryI32(query_text, lowest_allowed, highest_allowed);
}

int8_t UIConsole::queryI8(std::string query_text, int8_t lowest_allowed=CHAR_MIN, int8_t highest_allowed=CHAR_MAX)
{
    return (int8_t)queryI32(query_text, lowest_allowed, highest_allowed);
}

char UIConsole::queryChar(std::string query_text)
{
    char c;
    
    std::cout << query_text;
    std::cin >> c;
    return c;
}

void UIConsole::queryInput()
{
    last_choice = queryChar("Please make a choice: ");
}

void UIConsole::onMenuChoice()
{
    switch (this->last_choice) {
    case 'x': 
        app.endApplication();
        this->leave_menu = true; 
        break;
    default:
        std::cout << "The option " << this->last_choice << " is invalid!" << std::endl;
        break;
    }
}

// -----------------------------------------------------------------------------

UIMain::UIMain(Application &app) : UIConsole(app)
{
}

void UIMain::display()
{
    std::cout << std::endl;
    std::cout << g_strProgram << " V" << PGM_VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "Main Menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    //std::cout << " (1) detect modules" << std::endl;
    std::cout << " (2) select module" << std::endl;
    //std::cout << " (3) update firmware" << std::endl;
    std::cout << " (4) read register" << std::endl;
    std::cout << " (5) write register" << std::endl;
    //std::cout << " (6) backup all registers" << std::endl;
    //std::cout << " (7) restore all registers" << std::endl;
    std::cout << " (x) leave application" << std::endl;
}

void UIMain::onMenuChoice()
{   
    switch (this->last_choice) {
    case '2':
        selectModule();
        break;
    case '4':
        readRegister();
        break;
    case '5':
        writeRegister();
        break;
        
    default:
        UIConsole::onMenuChoice();
        break;
    }
}

void UIMain::selectModule()
{
    app.selectModule(queryU16("Please enter module ID: ", 0x0001, 0x7F7F));
}

void UIMain::readRegister()
{
    uint16_t selected_module_id = app.getSelectedModule();
    uint8_t register_id = 0;
    int value = 0;
    
    if (selected_module_id == 0) {
        selectModule();
        selected_module_id = app.getSelectedModule();
        if (selected_module_id == 0) {
            std::cout << "no module selected!" << std::endl;
            return;
        }
    }
    std::cout << "Read register of module ID: 0x" << std::hex << selected_module_id << std::endl;
    register_id = queryU8("Please select register: ");
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    if (app.readRegister(register_id, value)) {
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " successfully read." << std::endl; 
    } else {   
        std::cout << "reading failed!" << std::endl; 
    }
}

void UIMain::writeRegister()
{
    uint16_t selected_module_id = app.getSelectedModule();
    uint8_t register_id = 0;
    int value = 0, read_back_value = 0;
    
    if (selected_module_id == 0) {
        selectModule();
        selected_module_id = app.getSelectedModule();
        if (selected_module_id == 0) {
            std::cout << "no module selected!" << std::endl;
            return;
        }
    }
    std::cout << "Write register of module ID: 0x" << std::hex <<selected_module_id << std::endl;
    register_id = queryU8("Please select register: ");
    value = queryU32("Please enter value: ");
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    if (app.writeRegister(register_id, value)) {
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " written." << std::endl; 
    } else {   
        std::cout << "writing failed!" << std::endl; 
    }
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    if (app.verifyRegister(register_id, value, read_back_value)) {
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " successfully verified." << std::endl; 
    } else {   
        std::cout << "verification failed: written " << value << " read " << read_back_value << "!" << std::endl; 
    }
}

// -----------------------------------------------------------------------------

UIReadRegister::UIReadRegister(Application &app) : UIConsole(app)
{
}

void UIReadRegister::display()
{
    std::cout << g_strProgram << " V" << PGM_VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "Read Register of module " << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    
    std::cout << " (l) list possible registers" << std::endl;
    std::cout << " (2) set register" << std::endl;
    std::cout << " (3) backup all registers" << std::endl;
    std::cout << " (4) restore all registers" << std::endl;
    std::cout << " (x) leave application" << std::endl;
}

void UIReadRegister::onMenuChoice()
{
    switch (this->last_choice) {
    default:
        UIConsole::onMenuChoice();
    break;
    }
}

/** @} */
