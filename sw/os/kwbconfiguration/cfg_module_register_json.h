/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_register_json.h
 * @brief   Configuration: Register with JSON serializer/deserializer.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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

#include "cfg_module_register.hpp"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

NLOHMANN_JSON_SERIALIZE_ENUM( RegType, {
    {RegType::eINVALID, nullptr},
    {RegType::eU8, "u8"},
    {RegType::eU8, "U8"},
    {RegType::eU16, "u16"},
    {RegType::eU16, "U16"},
    {RegType::eU32, "u32"},
    {RegType::eU32, "U32"},
    {RegType::eI8, "i8"},
    {RegType::eI8, "I8"},
    {RegType::eI16, "i16"},
    {RegType::eI16, "I16"},
    {RegType::eI32, "i32"},
    {RegType::eI32, "I32"},
})

// map TaskState values to JSON as strings
NLOHMANN_JSON_SERIALIZE_ENUM( ValueFormat, {
    {ValueFormat::eINVALID, nullptr},
    {ValueFormat::eBINARY, "bin"},
    {ValueFormat::eOCTAL, "oct"},
    {ValueFormat::eDECIMAL, "dec"},
    {ValueFormat::eHEXADECIMAL, "hex"},
})

// --- Class definition --------------------------------------------------------

class ModuleRegisterJson : public ModuleRegister
{
public:
    ModuleRegisterJson();
    ModuleRegisterJson(uint16_t idx, const RegType &t, uint8_t access, const std::string &n, const ValueFormat &f, int64_t v);
    virtual ~ModuleRegisterJson();

    virtual void fromJson(const char *jsonString);
    virtual void fromJson(const std::string &jsonString);
    virtual void fromJson(const nlohmann::json &j);

    nlohmann::json toJson() const;
    std::string toJsonStr() const;

protected:
    uint16_t getRegIndexFromJson(const nlohmann::json &j);

    RegType getRegTypeFromJson(const nlohmann::json &j);

    uint8_t getRegAccessFromJson(const nlohmann::json &j);
    std::string regAccessToJson();

    std::string getRegNameFromJson(const nlohmann::json &j);

    ValueFormat getValueFormatFromJson(const nlohmann::json &j);

    template <typename T>
    T getRegValueFromJson(const nlohmann::json &j);


};

/** @} */
