/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_file.h
 * @brief   Configuration: Module with JSON serializer/deserializer.
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
#pragma once

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <string>
#include <nlohmann/json.hpp>

// include
#include "prjtypes.h"
#include "moddef_common.h"

#include "cfg_module_json.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class ModuleJson;

class ModuleFile
{
public:
    ModuleFile();
    ModuleFile(const std::string &filename);

    void fromFile(const std::string &filename);
    void fromString(const std::string &jsonString);
    void fromJson(const nlohmann::json &json);

    std::vector<ModuleJson> getModules()
    {
        return modules;
    };

protected:
    std::vector<ModuleJson> modules;

    std::vector<ModuleJson> loadFromFile(const std::string &filename);
};

/** @} */
