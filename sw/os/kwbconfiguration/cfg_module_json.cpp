/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_json.cpp
 * @brief   Configuration: Implementation of Module with JSON serializer/deserializer.
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

#include "cfg_module_json.h"

// include
#include "prjtypes.h"
#include "cmddef_common.h"

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

// os/libkwb
#include "exceptions.h"
#include "jsonhelper.hpp"
#include "log.h"
#include "numberformat.hpp"

using namespace nlohmann;

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

//----------------------------------------------------------------------------
ModuleJson::ModuleJson()
    : Module()
{
}

//----------------------------------------------------------------------------
void ModuleJson::fromJson(nlohmann::json &jm)
{
    this->nodeId = getNodeIdFromJson(jm);
    this->appId = getAppIdFromJson(jm);
    this->name = getNameFromJson(jm);
    this->description = getDescriptionFromJson(jm);
}

//----------------------------------------------------------------------------
void ModuleJson::fromJson(const char *jsonString)
{
    return fromJson(std::string(jsonString));
}

//----------------------------------------------------------------------------
void ModuleJson::fromJson(const std::string &jsonString)
{
    try {
        auto j = json::parse(jsonString);
        fromJson(j);
    }
    catch (json::parse_error &e) {
        throw InvalidParameter(LOC, "Cannot parse JSON string!\n%s", e.what());
    }
    catch (Exception &e) {
        throw InvalidParameter(LOC, "Cannot deserialize JSON into Module!\n%s", e.what());
    }
    catch (std::exception &e) {
        throw InvalidParameter(LOC, "Cannot deserialize JSON into Module!\n%s", e.what());
    }
}

//----------------------------------------------------------------------------
json ModuleJson::toJson() const
{
    auto jm = json::object();
    jm["nodeid"] = this->nodeId;
    jm["appid"] = this->appId;
    jm["name"] = this->name;
    jm["description"] = this->description;
    return jm;
}

//----------------------------------------------------------------------------
std::string ModuleJson::toJsonStr() const
{
    std::stringstream sstr;
    sstr << toJson();
    return sstr.str();
}

//----------------------------------------------------------------------------
uint16_t ModuleJson::getNodeIdFromJson(const nlohmann::json &jm)
{
    return json2number<uint16_t>(jm, "nodeid", ValueFormat::eUNSPECIFIED, false);
}

//----------------------------------------------------------------------------
uint16_t ModuleJson::getAppIdFromJson(const nlohmann::json &jm)
{
    return json2number<uint16_t>(jm, "appid", ValueFormat::eUNSPECIFIED, false);
}

//----------------------------------------------------------------------------
std::string ModuleJson::getNameFromJson(const nlohmann::json &jm)
{
    return json2string(jm, "name");
}

//----------------------------------------------------------------------------
std::string ModuleJson::getDescriptionFromJson(const nlohmann::json &jm)
{
    return json2string(jm, "description");
}

/** @} */
