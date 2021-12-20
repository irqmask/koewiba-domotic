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
    virtual char queryChar(std::string query_text);

    /**
     * Query an unsigned 32bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as unsigned 32bit integer.
     */
    virtual uint32_t queryU32(std::string query_text, uint32_t lowest_allowed, uint32_t highest_allowed);

    /**
     * Query an unsigned 16bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as unsigned 16bit integer.
     */
    virtual uint16_t queryU16(std::string query_text, uint16_t lowest_allowed, uint16_t highest_allowed);

    /**
     * Query an unsigned 8bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as unsigned 8bit integer.
     */
    virtual uint8_t queryU8(std::string query_text, uint8_t lowest_allowed, uint8_t highest_allowed);

    /**
     * Query a signed 32bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as signed 32bit integer.
     */
    virtual int32_t queryI32(std::string query_text, int32_t lowest_allowed, int32_t highest_allowed);

    /**
     * Query a signed 16bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as signed 16bit integer.
     */
    virtual int16_t queryI16(std::string query_text, int16_t lowest_allowed, int16_t highest_allowed);

    /**
     * Query a signed 8bit integer as user input.
     *
     * @param[in]   query_text      Text to display for this query.
     * @param[in]   lowest_allowed  Lowest accepted value.
     * @param[in]   highest_allowed Highest accepted value.
     *
     * @return User input as signed 8bit integer.
     */
    virtual int8_t queryI8(std::string query_text, int8_t lowest_allowed, int8_t highest_allowed);

    //! Selected menu item by the user.
    char        last_choice;
    //! Flag if menu shall be left and return to upper menu.
    bool        leave_menu;
    //! Reference to belonging application
    Application &app;
};

// -----------------------------------------------------------------------------

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
    void detectModules();
    void selectModule();
    void readRegister();
    void writeRegister();
    void setTime();
    void getTime();
};

/** @} */
