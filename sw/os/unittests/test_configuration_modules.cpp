/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_configuration_modules.cpp
 * @brief   This module contains the unittests for: ModuleFile class.
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@gmx.de>
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

#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "prjconf.h"

// include
#include "prjtypes.h"
// kwbconfiguration
#include "../kwbconfiguration/cfg_module_file.h"
#include "../kwbconfiguration/cfg_module_file.cpp"

using namespace nlohmann;

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/// @test Test function to parse the register layout file
TEST(kwbconfiguration_Modules, OneFromString)
{
    std::string modcfg =
        "{\"modules\":["
        "  {"
        "    \"nodeid\": \"0x104\","
        "    \"appid\": \"0x815\","
        "    \"name\": \"GiesbertGockel\","
        "    \"description\": \"Das ist die Beschreibung zu Giesbert Gockel\""
        "  }"
        "]}";

    ModuleFile m;
    ASSERT_NO_THROW(m.fromString(modcfg));
    auto modules = m.getModules();

    ASSERT_EQ(modules.size(), 1);

    ASSERT_STREQ(modules[0].name.c_str(), "GiesbertGockel");
}


/// @test Test function to parse the register layout file
TEST(kwbconfiguration_Modules, ThreeFromString)
{
    std::string modcfg =
        "{\"modules\":["
        "  {"
        "    \"nodeid\": \"0x301\","
        "    \"appid\": 111,"
        "    \"name\": \"one\","
        "    \"description\": \"description one\""
        "  },"
        "  {"
        "    \"nodeid\": \"0x302\","
        "    \"appid\": 222,"
        "    \"name\": \"two\","
        "    \"description\": \"description two\""
        "  },"
        "  {"
        "    \"nodeid\": \"0x303\","
        "    \"appid\": 333,"
        "    \"name\": \"three\","
        "    \"description\": \"description three\""
        "  }"
        "]}";

    ModuleFile m;
    ASSERT_NO_THROW(m.fromString(modcfg));
    auto modules = m.getModules();

    ASSERT_EQ(modules.size(), 3);

    ASSERT_EQ(modules[0].appId, 111);
    ASSERT_STREQ(modules[0].name.c_str(), "one");
    ASSERT_EQ(modules[1].appId, 222);
    ASSERT_STREQ(modules[1].name.c_str(), "two");
    ASSERT_EQ(modules[2].appId, 333);
    ASSERT_STREQ(modules[2].name.c_str(), "three");
}

/** @} */
