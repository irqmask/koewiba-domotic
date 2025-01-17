/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_configuration_module_config.cpp
 * @brief   This module contains the unittests for: ModuleConfig class..
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@gmx.de>
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
// libsystem
#include "systime.h"
// kwbconfiguration
#include "../kwbconfiguration/cfg_module_json.h"
#include "../kwbconfiguration/cfg_module_json.cpp"

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
TEST(kwbconfiguration_Module, fromString)
{
    std::string modcfg =
        //"{\"modules\":["
        "  {"
        "    \"nodeid\": \"0x104\","
        "    \"appid\": \"0x815\","
        "    \"name\": \"GustavGans\","
        "    \"description\": \"Das ist die Beschreibung zu Giesbert Gockel\""
        "  }";
        //"]}";

    ModuleJson m;
    ASSERT_NO_THROW(m.fromJson(modcfg));

    ASSERT_EQ(m.nodeId, 0x104);
    ASSERT_EQ(m.appId, 0x815);
    ASSERT_STREQ(m.name.c_str(), "GustavGans");
    ASSERT_STREQ(m.description.c_str(), "Das ist die Beschreibung zu Giesbert Gockel");


}

// test node id in dec
// test node id in hex
// test app id in dec
// test app id in hex
// test missing description (no error)
// test missing node id (error expected)
// test missing app id (error expected)
// test missing name (error expected)

/** @} */
