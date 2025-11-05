/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    uiconsole.h
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
#pragma once

// --- Include section ---------------------------------------------------------

#include <climits>
#include <cstdint>
#include <iostream>

// include
#include "prjconf.h"
#include "prjtypes.h"
#include "application.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Console user interface base class.
 */
class UIConsole
{
public:
    /**
     * Constructor
     * @param[in]   app         Reference to belonging application
     */
    UIConsole(Application &app);

    /**
     * Main loop. Will run while current UI screen is active
     */
    void run();

protected:
    /**
     * Display the screen
     */
    virtual void display();

    /**
     * Query input from user
     */
    virtual void queryInput();

    /**
     * Act on menu choice
     */
    virtual void onMenuChoice();

    /**
     * Query a character as user input.
     *
     * @param[in]   query_text   Text to display for this query.
     *
     * @return User input as character.
     */
    virtual char queryChar(const std::string &query_text);

    /**
     * Query an unsigned 32bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as unsigned 32bit integer.
     */
    virtual uint32_t queryU32(const std::string &query_text, uint32_t lowest_allowed = 0,
                              uint32_t highest_allowed = 0xFFFFFFFF);

    /**
     * Query an unsigned 16bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as unsigned 16bit integer.
     */
    virtual uint16_t queryU16(const std::string &query_text, uint16_t lowest_allowed = 0,
                              uint16_t highest_allowed = 0xFFFF);

    /**
     * Query an unsigned 8bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as unsigned 8bit integer.
     */
    virtual uint8_t queryU8(const std::string &query_text, uint8_t lowest_allowed = 0, uint8_t highest_allowed = 0xFF);

    /**
     * Query a signed 32bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as signed 32bit integer.
     */
    virtual int32_t queryI32(const std::string &query_text, int32_t lowest_allowed = INT_MIN,
                             int32_t highest_allowed = INT_MAX);

    /**
     * Query a signed 16bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as signed 16bit integer.
     */
    virtual int16_t queryI16(const std::string &query_text, int16_t lowest_allowed = SHRT_MIN,
                             int16_t highest_allowed = SHRT_MAX);

    /**
     * Query a signed 8bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as signed 8bit integer.
     */
    virtual int8_t queryI8(const std::string &query_text, int8_t lowest_allowed = CHAR_MIN,
                           int8_t highest_allowed = CHAR_MAX);

    //! Selected menu item by the user.
    char        last_choice;
    //! Flag if menu shall be left and return to upper menu.
    bool        leave_menu;
    //! Reference to belonging application
    Application &app;
};

/** @} */
