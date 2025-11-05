/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_register_json.cpp
 * @brief   Configuration: Implementation of Register with JSON serializer/deserializer.
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

#include "cfg_module_register_json.h"

#include <nlohmann/json.hpp>

// include
#include "prjtypes.h"
#include "cmddef_common.h"

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

// os/libkwb
#include "exceptions.h"
#include "log.h"
#include "numberformat.hpp"

#include "cfg_module_register.hpp"

using namespace nlohmann;

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------



// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

//----------------------------------------------------------------------------
ModuleRegisterJson::ModuleRegisterJson()
    : ModuleRegister()
{
}

//----------------------------------------------------------------------------
ModuleRegisterJson::ModuleRegisterJson(uint16_t index, const RegType &type, uint8_t accessMask, const std::string &name,
                                       const ValueFormat &format, int64_t value)
    : ModuleRegister(index, type, accessMask, name, format, value)
{
}

//----------------------------------------------------------------------------
ModuleRegisterJson::~ModuleRegisterJson()
{
}

//----------------------------------------------------------------------------
void ModuleRegisterJson::fromJson(const char *jsonString)
{
    fromJson(std::string(jsonString));
}

//----------------------------------------------------------------------------
void ModuleRegisterJson::fromJson(const std::string &jsonString)
{
    try {
        auto j = json::parse(jsonString);
        fromJson(j);
    }
    catch (json::parse_error &e) {
        throw InvalidParameter(LOC, "Cannot parse JSON string! %s", e.what());
    }
    catch (Exception &e) {
        throw InvalidParameter(LOC, "Cannot deserialize JSON into ModuleRegister! %s", e.what());
    }
    catch (std::exception &e) {
        throw InvalidParameter(LOC, "Cannot deserialize JSON into ModuleRegister! %s", e.what());
    }
}

//----------------------------------------------------------------------------
void ModuleRegisterJson::fromJson(const nlohmann::json &j)
{
    this->index = getRegIndexFromJson(j);
    this->type = getRegTypeFromJson(j);
    this->accessMask = getRegAccessFromJson(j);
    this->name = getRegNameFromJson(j);
    this->format = getValueFormatFromJson(j);

    if (j.contains("value")) {
        switch (this->type) {
        case RegType::eU8:
            this->value = getRegValueFromJson<uint8_t>(j);
            break;
        case RegType::eU16:
            this->value = getRegValueFromJson<uint16_t>(j);
            break;
        case RegType::eU32:
            this->value = getRegValueFromJson<uint32_t>(j);
            break;
        case RegType::eI8:
            this->value = getRegValueFromJson<int8_t>(j);
            break;
        case RegType::eI16:
            this->value = getRegValueFromJson<int16_t>(j);
            break;
        case RegType::eI32:
            this->value = getRegValueFromJson<int32_t>(j);
            break;
        default:
            this->value = 0;
        }
    }
    else {
        value = 0;
    }
}

//----------------------------------------------------------------------------
json ModuleRegisterJson::toJson() const
{
    if (type == RegType::eINVALID) {
        throw LogicError(LOC, "Cannot convert type RegType::eUNSPECIFIED to JSON");
    }
    if (format == ValueFormat::eUNSPECIFIED) {
        throw LogicError(LOC, "Cannot convert format ValueFormat::eUNSPECIFIED to JSON");
    }
    auto jr = json::object();
    jr["index"] = this->index;
    jr["type"] = this->type;
    jr["access"] = regAccessToString();
    jr["name"] = this->name;
    jr["format"] = this->format;

    jr["value"] = valueToString();
    return jr;
}

//----------------------------------------------------------------------------
std::string ModuleRegisterJson::toJsonStr() const
{
    std::stringstream sstr;
    sstr << toJson();
    return sstr.str();
}

//----------------------------------------------------------------------------
uint16_t ModuleRegisterJson::getRegIndexFromJson(const json &j)
{

    if (!j.contains("index")) {
        throw InvalidParameter(LOC, "Missing parameter 'index'. expected entity 'index' beeing unsigned 16bit integer!");
    }
    if (!j["index"].is_number_unsigned()) {
        std::string kv = j;
        throw InvalidParameter(LOC,
                               "Invalid format of parameter 'index'. expected entity 'index' beeing unsigned 16bit integer but is %s!", kv.c_str());
    }

    uint64_t val = j.value<uint64_t>("index", 0);
    if (val > UINT16_MAX) {
        std::string kv = j;
        throw InvalidParameter(LOC,
                               "Invalid format of parameter 'index'. expected entity 'index' beeing unsigned 16bit integer but is %s!", kv.c_str());
    }

    return static_cast<uint16_t>(val);
}

//----------------------------------------------------------------------------
RegType ModuleRegisterJson::getRegTypeFromJson(const json &j)
{
    if (!j.contains("type")) {
        throw InvalidParameter(LOC,
                               "Missing parameter 'type'. expected entity 'type' beeing 'u8', 'u16', 'u32', 'i8', 'i16', 'i32' !");
    }
    RegType rt = j["type"].get<RegType>();
    if (rt == RegType::eINVALID) {
        std::string kv = j["type"];
        throw InvalidParameter(LOC,
                               "Invalid value '%s' for parameter 'type'. expected entity 'type' beeing 'u8', 'u16', 'u32', 'i8', 'i16', 'i32' !",
                               kv.c_str());
    }
    return rt;
}

//----------------------------------------------------------------------------
uint8_t ModuleRegisterJson::getRegAccessFromJson(const nlohmann::json &j)
{
    if (!j.contains("access")) {
        throw InvalidParameter(LOC, "Missing parameter 'access'. expected entity 'access' beeing 'r', 'rw' !");
    }
    std::string s = j["access"];
    if (s == "r") {
        return RegAccess::eREG_ACCESS_READ;
    }
    else if (s == "rw") {
        return RegAccess::eREG_ACCESS_READ | RegAccess::eREG_ACCESS_WRITE;
    }
    else {
        throw InvalidParameter(LOC, "Invalid parameter 'access' = %s. expected entity 'access' beeing 'r', 'rw' !", s.c_str());
    }
}

//----------------------------------------------------------------------------
std::string ModuleRegisterJson::getRegNameFromJson(const nlohmann::json &j)
{
    if (!j.contains("name")) {
        throw InvalidParameter(LOC, "Missing parameter 'name'!");
    }
    std::string s = j["name"];

    if (s.size() == 0) {
        throw InvalidParameter(LOC, "Empty value for parameter 'name' not allowed!");
    }

    return s;
}

//----------------------------------------------------------------------------
ValueFormat ModuleRegisterJson::getValueFormatFromJson(const nlohmann::json &j)
{
    if (!j.contains("format")) {
        return ValueFormat::eDECIMAL; // default
    }
    ValueFormat vf = j["format"].get<ValueFormat>();

    if (vf == ValueFormat::eUNSPECIFIED) {
        std::string kv = j["format"];
        throw InvalidParameter(LOC,
                               "Invalid value '%s' for parameter 'type'. expected entity 'type' beeing 'u8', 'u16', 'u32', 'i8', 'i16', 'i32' !",
                               kv.c_str());
    }
    return vf;
}

//----------------------------------------------------------------------------
template <typename T>
T ModuleRegisterJson::getRegValueFromJson(const nlohmann::json &j)
{
    if (!j.contains("value")) {
        throw InvalidParameter(LOC, "Missing parameter 'value'!");
    }
    T v = 0;
    if (j["value"].is_number()) {
        if (format != ValueFormat::eDECIMAL) {
            throw InvalidParameter(LOC,
                                   "Expected parameter 'value' needs to be a string for proper prefix for binary '0b', octal '0o' or hexadecimal '0x' values!");
        }
        v = j.value<T>("value", 0);
    }
    else if (j["value"].is_string()) {
        std::string s = j["value"];
        v = str2number<T>(s, format, (type == RegType::eI8) || (type == RegType::eI16) || (type == RegType::eI32));
    }
    else {
        std::string s = j["value"];
        throw InvalidParameter(LOC, "Value %s cannot be converted to number!", s.c_str());
    }

    return v;
}

/** @} */
