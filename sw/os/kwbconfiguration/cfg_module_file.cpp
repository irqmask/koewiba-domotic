/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_file.cpp
 * @brief   Configuration: Load module list from JSON file.
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

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <fstream>

#include "cfg_module_file.h"

// include
#include "prjtypes.h"

// os/libkwb
#include "exceptions.h"
#include "log.h"

using namespace nlohmann;

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

//----------------------------------------------------------------------------
ModuleFile::ModuleFile()
{
}

//----------------------------------------------------------------------------
ModuleFile::ModuleFile(const std::string &filename)
{
    fromFile(filename);
}

//----------------------------------------------------------------------------
void ModuleFile::fromFile(const std::string &filename)
{
    json all_modules;
    try {
        std::ifstream ifs(filename, std::ios::in);
        ifs >> all_modules;
    }
    catch (std::exception & e) {
        throw ResourceMissing(LOC, "Unable to read values file! %s\n%s", filename.c_str(), e.what());
    }
    fromJson(all_modules);
}

//----------------------------------------------------------------------------
void ModuleFile::fromString(const std::string &jsonString)
{
    json all_modules;
    try {
        all_modules = json::parse(jsonString);
    }
    catch (std::exception & e) {
        throw ResourceMissing(LOC, "Unable to parse JSON string!\n%s", e.what());
    }
    fromJson(all_modules);
}

//----------------------------------------------------------------------------
void ModuleFile::fromJson(const nlohmann::json &json)
{
    auto &ja = json["modules"];
    std::vector<ModuleJson> modules;

    if (ja.is_null() || !ja.is_array()) {
        throw InvalidParameter(LOC, "json object for modules wrong. expected outer entity beeing an array!");
    }

    try {
        for (auto jmod : ja) {
            ModuleJson m;
            m.fromJson(jmod);
            modules.emplace_back(m);
        }
    }
    catch(Exception &e) {
        throw OperationFailed(LOC, "modules json object parsing error %s", e.what());
    }
    this->modules = modules;
}

/** @} */
