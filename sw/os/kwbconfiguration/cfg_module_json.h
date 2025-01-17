/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_json.h
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

#include <nlohmann/json.hpp>

// include
#include "prjtypes.h"
#include "moddef_common.h"

#include "cfg_module.hpp"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class ModuleJson : public Module
{
public:
    ModuleJson();

    virtual void fromJson(nlohmann::json &jm);
    virtual void fromJson(const char *jsonString);
    virtual void fromJson(const std::string &jsonString);

    nlohmann::json toJson() const;
    std::string toJsonStr() const;

protected:
    uint16_t getNodeIdFromJson(const nlohmann::json &jm);
    uint16_t getAppIdFromJson(const nlohmann::json &jm);
    std::string getNameFromJson(const nlohmann::json &jm);
    std::string getDescriptionFromJson(const nlohmann::json &jm);
};

/** @} */
