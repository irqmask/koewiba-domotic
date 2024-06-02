/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_register.hpp
 * @brief   Configuration: Holds properties of the registers of a module.
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

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

// include
#include "prjtypes.h"
#include "moddef_common.h"

// libkwb
#include "exceptions.h"
#include "log.h"

// --- Definitions -------------------------------------------------------------

#define ACCESS_READ 0x01;
#define ACCESS_WRITE 0x02;

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

enum class RegType : uint8_t
{
    eINVALID = 0,
    eU8 = 1,
    eU16,
    eU32,
    eI8,
    eI16,
    eI32
};


enum RegAccess
{
    eREG_ACCESS_READ = 1,
    eREG_ACCESS_WRITE = 2
};

enum class ValueFormat : uint8_t
{
    eINVALID,
    eBINARY,
    eDECIMAL,
    eOCTAL,
    eHEXADECIMAL
};

/// Definition of a module's register
class ModuleRegisterDef
{
public:
    ModuleRegisterDef()
        : index(0)
        , type(RegType::eINVALID)
        , accessMask(0)
        , format(ValueFormat::eDECIMAL)
    {
    }

    ModuleRegisterDef(uint16_t idx, const RegType &t, uint8_t access, const std::string &n, const ValueFormat &f)
        : index(idx)
        , type(t)
        , accessMask(access)
        , name(n)
        , format(f)
    {}

    uint16_t index;
    RegType type;
    uint8_t accessMask;
    std::string name;
    ValueFormat format = ValueFormat::eDECIMAL;

protected:
    std::string regAccessToString() const
    {
        switch (this->accessMask) {
        case eREG_ACCESS_READ:
            return "r";
        case (eREG_ACCESS_READ | eREG_ACCESS_WRITE):
            return "rw";
        default:
            return "";
        }
    }


};


/// Module's register its holding value
class ModuleRegister : public ModuleRegisterDef
{
public:
    ModuleRegister()
        : ModuleRegisterDef()
        , value(0)
    {
    }

    ModuleRegister(uint16_t idx, const RegType &t, uint8_t access, const std::string &n, const ValueFormat &f, int64_t v)
        : ModuleRegisterDef(idx, t, access, n, f)
        , value(v)
    {
    }

    int64_t value;

protected:
    //----------------------------------------------------------------------------
    template<typename T>
    std::string valueToBinaryString(T value) const
    {
        std::stringstream ss;
        while (value > 0) {
            ss << (value % 2);
            value /= 2;
        }
        std::string binary = ss.str();
        std::reverse(binary.begin(), binary.end());
        return binary;
    }

    //----------------------------------------------------------------------------
    template<typename T>
    std::string tvalueToString(T value, ValueFormat format) const
    {
        std::stringstream ss;
        switch (format)
        {
        case ValueFormat::eBINARY:
            ss << valueToBinaryString(value);
            break;
        case ValueFormat::eOCTAL:
            ss << std::oct << static_cast<int64_t>(value); // ugly hack to proper convert uint_8 to octal
            break;
        case ValueFormat::eDECIMAL:
            ss << std::to_string(value);
            break;
        case ValueFormat::eHEXADECIMAL:
            ss << std::hex << static_cast<int64_t>(value); // ugly hack to proper convert uint_8 to hexadecimal
            break;
        default:
            throw InvalidParameter(LOC, "unknown format");
        }
        return ss.str();
    }

    //----------------------------------------------------------------------------
    std::string valueToString() const
    {
        switch (type) {
        case RegType::eU8:
            return tvalueToString<uint8_t>(static_cast<uint8_t>(value), format);
        case RegType::eU16:
            return tvalueToString<uint16_t>(static_cast<uint16_t>(value), format);
        case RegType::eU32:
            return tvalueToString<uint32_t>(static_cast<uint32_t>(value), format);
        case RegType::eI8:
            return tvalueToString<int8_t>(static_cast<int8_t>(value), format);
        case RegType::eI16:
            return tvalueToString<int16_t>(static_cast<int16_t>(value), format);
        case RegType::eI32:
            return tvalueToString<int32_t>(static_cast<int32_t>(value), format);
        default:
            return std::string("NaN");
        }
    }

};

/** @} */
