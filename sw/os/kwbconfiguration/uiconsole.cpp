/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    uiconsole.cpp
 * @brief   UI for console.
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

#include "uiconsole.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <string>

// include
#include "prjconf.h"
#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

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
uint32_t UIConsole::queryU32(std::string query_text, uint32_t lowest_allowed, uint32_t highest_allowed)
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
uint16_t UIConsole::queryU16(std::string query_text, uint16_t lowest_allowed, uint16_t highest_allowed)
{
    return (uint16_t)queryU32(query_text, lowest_allowed, highest_allowed);
}

//----------------------------------------------------------------------------
uint8_t UIConsole::queryU8(std::string query_text, uint8_t lowest_allowed, uint8_t highest_allowed)
{
    return (uint8_t)queryU32(query_text, lowest_allowed, highest_allowed);
}

//----------------------------------------------------------------------------
int32_t UIConsole::queryI32(std::string query_text, int32_t lowest_allowed, int32_t highest_allowed)
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
int16_t UIConsole::queryI16(std::string query_text, int16_t lowest_allowed, int16_t highest_allowed)
{
    return (int16_t)queryI32(query_text, lowest_allowed, highest_allowed);
}

//----------------------------------------------------------------------------
int8_t UIConsole::queryI8(std::string query_text, int8_t lowest_allowed, int8_t highest_allowed)
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

/** @} */
