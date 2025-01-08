/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module_register.hpp
 * @brief   Configuration: Holds properties of the registers of a module.
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

#include <iostream>
#include <string>

// include
#include "prjtypes.h"
#include "moddef_common.h"

// libkwb
#include "exceptions.h"
#include "log.h"
#include "numberformat.hpp"

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
    std::string valueToString() const
    {
        switch (type) {
        case RegType::eU8:
            return number2str<uint8_t>(static_cast<uint8_t>(value), format);
        case RegType::eU16:
            return number2str<uint16_t>(static_cast<uint16_t>(value), format);
        case RegType::eU32:
            return number2str<uint32_t>(static_cast<uint32_t>(value), format);
        case RegType::eI8:
            return number2str<int8_t>(static_cast<int8_t>(value), format);
        case RegType::eI16:
            return number2str<int16_t>(static_cast<int16_t>(value), format);
        case RegType::eI32:
            return number2str<int32_t>(static_cast<int32_t>(value), format);
        default:
            return std::string("NaN");
        }
    }

};

/** @} */
