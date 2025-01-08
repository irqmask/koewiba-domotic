/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    numberformat.hpp
 * @brief   Convert numers to string and vice versa
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
#pragma once

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>

#include "exceptions.h"

/**
 * Supported value format representations
 */
enum class ValueFormat : uint8_t
{
    eUNSPECIFIED,
    eBINARY,
    eDECIMAL,
    eOCTAL,
    eHEXADECIMAL
};

/**
 * Convert string into number
 * @param[in]   s       String to convert
 * @param[in]   format  Expected value format, use eUNSPECIFIED to automatically convert.
 * @param[in]   isSignedFormat  The number shall be interpreted as signed number
 * @return Value of type T
 * @throws InvalidParameter if string cannot be converted into number in desired format.
 */
template <typename T>
T str2number(const std::string &s, const ValueFormat &format, bool isSignedFormat)
{
    T v = 0;
    try {
        int base = 10;
        switch (format)
        {
        case ValueFormat::eBINARY:
            base = 2;
            break;
        case ValueFormat::eOCTAL:
            base = 8;
            break;
        case ValueFormat::eDECIMAL:
            base = 10;
            break;
        case ValueFormat::eHEXADECIMAL:
            base = 16;
            break;
        default:
            base = 0;
            break;
        }

        if (isSignedFormat)
            v = stol(s, nullptr, base); // signed type conversion
        else
            v = stoul(s, nullptr, base); // unsigned type conversion
    }
    catch (std::invalid_argument &e) {
        throw InvalidParameter(LOC, "Value %s cannot be converted to number! %s", s.c_str(), e.what());
    }
    catch (std::out_of_range &e) {
        throw InvalidParameter(LOC, "Value %s cannot be converted to number! Out of range!", s.c_str(), e.what());
    }
    return v;
}

//----------------------------------------------------------------------------
template<typename T>
static std::string valueToBinaryString(T value)
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

/**
 * Convert number into string according to value format.
 * @tparam[in]  value   Value to convert into string
 * @param[in]   format  Value representation to convert into.
 * @returns Value as string
 * @throws InvalidParameter if given value format is not supported.
 */
template<typename T>
std::string number2str(T value, const ValueFormat &format)
{
    std::stringstream numberAsStream;
    switch (format)
    {
    case ValueFormat::eBINARY:
        numberAsStream << valueToBinaryString(value);
        break;
    case ValueFormat::eOCTAL:
        numberAsStream << std::oct << static_cast<int64_t>(value); // ugly hack to proper convert uint_8 to octal
        break;
    case ValueFormat::eDECIMAL:
        numberAsStream << std::to_string(value);
        break;
    case ValueFormat::eHEXADECIMAL:
        numberAsStream << std::hex << static_cast<int64_t>(value); // ugly hack to proper convert uint_8 to hexadecimal
        break;
    default:
        throw InvalidParameter(LOC, "unknown format");
    }
    return numberAsStream.str();
}


/** @} */
