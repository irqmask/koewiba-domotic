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

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <ctime>
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

//! application name
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

//----------------------------------------------------------------------------
void UIConsole::run()
{
    while (!this->leave_menu) {
        display();
        queryInput();
        onMenuChoice();
    }
}

//----------------------------------------------------------------------------
void UIConsole::display()
{
}

//----------------------------------------------------------------------------
uint32_t UIConsole::queryU32(std::string query_text, uint32_t lowest_allowed = 0, uint32_t highest_allowed = UINT_MAX)
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

        if (!input_is_valid) {
            std::cout << "invalid value entered!" << std::endl;
        }

    } while (!input_is_valid);
    return value;
}

//----------------------------------------------------------------------------
uint16_t UIConsole::queryU16(std::string query_text, uint16_t lowest_allowed = 0, uint16_t highest_allowed = USHRT_MAX)
{
    return (uint16_t)queryU32(query_text, lowest_allowed, highest_allowed);
}

//----------------------------------------------------------------------------
uint8_t UIConsole::queryU8(std::string query_text, uint8_t lowest_allowed = 0, uint8_t highest_allowed = UCHAR_MAX)
{
    return (uint8_t)queryU32(query_text, lowest_allowed, highest_allowed);
}

//----------------------------------------------------------------------------
int32_t UIConsole::queryI32(std::string query_text, int32_t lowest_allowed = INT_MIN, int32_t highest_allowed = INT_MAX)
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

        if (!input_is_valid) {
            std::cout << "invalid value entered!" << std::endl;
        }

    } while (!input_is_valid);
    return value;
}

//----------------------------------------------------------------------------
int16_t UIConsole::queryI16(std::string query_text, int16_t lowest_allowed = SHRT_MIN,
                            int16_t highest_allowed = SHRT_MAX)
{
    return (int16_t)queryI32(query_text, lowest_allowed, highest_allowed);
}

//----------------------------------------------------------------------------
int8_t UIConsole::queryI8(std::string query_text, int8_t lowest_allowed = CHAR_MIN, int8_t highest_allowed = CHAR_MAX)
{
    return (int8_t)queryI32(query_text, lowest_allowed, highest_allowed);
}

//----------------------------------------------------------------------------
char UIConsole::queryChar(std::string query_text)
{
    char c;

    std::cout << query_text;
    std::cin >> c;
    return c;
}

//----------------------------------------------------------------------------
void UIConsole::queryInput()
{
    last_choice = queryChar("Please make a choice: ");
}

//----------------------------------------------------------------------------
void UIConsole::onMenuChoice()
{
    switch (this->last_choice) {
    case 'x':
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

//----------------------------------------------------------------------------
void UIMain::display()
{
    std::cout << std::endl;
    std::cout << g_strProgram << " V" << PGM_VERSION << std::endl;
    std::cout << std::endl;
    std::cout << "Main Menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << " (1) detect modules" << std::endl;
    std::cout << " (2) select module" << std::endl;
    //std::cout << " (3) update firmware" << std::endl;
    std::cout << " (4) read register" << std::endl;
    std::cout << " (5) write register" << std::endl;
    //std::cout << " (6) backup all registers" << std::endl;
    //std::cout << " (7) restore all registers" << std::endl;
    std::cout << " (t) set time (according to system time)" << std::endl;
    std::cout << " (z) read time" << std::endl;
    std::cout << " (x) leave application" << std::endl;
}

//----------------------------------------------------------------------------
void UIMain::onMenuChoice()
{
    switch (this->last_choice) {
    case '1':
        detectModules();
        break;
    case '2':
        selectModule();
        break;
    case '4':
        readRegister();
        break;
    case '5':
        writeRegister();
        break;
    case 't':
        setTime();
        break;
    case 'z':
        getTime();
        break;
    default:
        UIConsole::onMenuChoice();
        break;
    }
}

//----------------------------------------------------------------------------
void UIMain::detectModules()
{
    app.detectModules();
    auto modules = app.getDetectedModules();
    if (modules.size() == 0) {
        std::cout << "No modules detected!" << std::endl;
        return;
    }

    std::cout << "Node ID | Ctrl ID             | Board  |     | App ID | App Version " << std::endl;
    std::cout << "        |                     | ID     | Rev |        |             " << std::endl;
    std::cout << "--------+---------------------+--------+-----+--------+-------------" << std::endl;
    for (ActionQueryModules::Module m : modules) {
        printf("0x%04X  | 0x%02X 0x%02X 0x%02X 0x%02X | 0x%04X | %3d | 0x%04X | %d.%d.%d",
               m.nodeId,
               m.version.controller_id[0], m.version.controller_id[1],
               m.version.controller_id[2], m.version.controller_id[3],
               m.version.board_id, m.version.board_rev,
               m.version.app_id,
               m.version.version[0], m.version.version[1], m.version.version[2]);
    }
    std::cout << std::endl;
}

//----------------------------------------------------------------------------
void UIMain::selectModule()
{
    app.selectModule(queryU16("Please enter module ID: ", 0x0001, 0x7F7F));
}

//----------------------------------------------------------------------------
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
    }
    else {
        std::cout << "reading failed!" << std::endl;
    }
}

//----------------------------------------------------------------------------
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
    std::cout << "Write register of module ID: 0x" << std::hex << selected_module_id << std::endl;
    register_id = queryU8("Please select register: ");
    value = queryU32("Please enter value: ");
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    if (app.writeRegister(register_id, value)) {
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " written." << std::endl;
    }
    else {
        std::cout << "writing failed!" << std::endl;
    }
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    if (app.verifyRegister(register_id, value, read_back_value)) {
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " successfully verified." << std::endl;
    }
    else {
        std::cout << "verification failed: written " << value << " read " << read_back_value << "!" << std::endl;
    }
}

//----------------------------------------------------------------------------
void UIMain::setTime()
{
    uint16_t selected_module_id = app.getSelectedModule();
    int year = 0, month = 0, day = 0, dow = 0, hour = 0, minute = 0, second = 0;
    bool success = false;

    do {
        if (selected_module_id == 0) {
            selectModule();
            selected_module_id = app.getSelectedModule();
            if (selected_module_id == 0) {
                std::cout << "no module selected!" << std::endl;
                break;
            }
        }

        std::time_t t = std::time(0);

        tm *now = std::localtime(&t);
        year = now->tm_year + 1900;
        month = now->tm_mon + 1;
        day = now->tm_mday;
        dow = now->tm_wday;
        hour = now->tm_hour;
        minute = now->tm_min;
        second = now->tm_sec;

        if (!app.writeRegister(223, year)) {
            break;
        }
        if (!app.writeRegister(224, month)) {
            break;
        }
        if (!app.writeRegister(225, day)) {
            break;
        }
        if (!app.writeRegister(226, dow)) {
            break;
        }
        if (!app.writeRegister(227, hour)) {
            break;
        }
        if (!app.writeRegister(228, minute)) {
            break;
        }
        if (!app.writeRegister(229, second)) {
            break;
        }

        fprintf(stdout, "Set time of module ID: 0x%04X %04d-%02d-%02d dow %d %02d:%02d:%02d\n", selected_module_id, year, month,
                day, dow, hour, minute, second);
        success = true;
    } while (false);
    if (!success) {
        fprintf(stdout, "Get time of module ID: 0x%04X FAILED\n", selected_module_id);
    }
}

//----------------------------------------------------------------------------
void UIMain::getTime()
{
    uint16_t selected_module_id = app.getSelectedModule();
    int year = 0, month = 0, day = 0, dow = 0, hour = 0, minute = 0, second = 0;
    bool success = false;

    do {
        if (selected_module_id == 0) {
            selectModule();
            selected_module_id = app.getSelectedModule();
            if (selected_module_id == 0) {
                std::cout << "no module selected!" << std::endl;
                break;
            }
        }
        if (!app.readRegister(223, year)) {
            break;
        }
        if (!app.readRegister(224, month)) {
            break;
        }
        if (!app.readRegister(225, day)) {
            break;
        }
        if (!app.readRegister(226, dow)) {
            break;
        }
        if (!app.readRegister(227, hour)) {
            break;
        }
        if (!app.readRegister(228, minute)) {
            break;
        }
        if (!app.readRegister(229, second)) {
            break;
        }
        fprintf(stdout, "Get time of module ID: 0x%04X %04d-%02d-%02d dow %d %02d:%02d:%02d\n", selected_module_id, year, month,
                day, dow, hour, minute, second);
        success = true;
    } while (false);
    if (!success) {
        fprintf(stdout, "Get time of module ID: 0x%04X FAILED\n", selected_module_id);
    }
}

/** @} */
