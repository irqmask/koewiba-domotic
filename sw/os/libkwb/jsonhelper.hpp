/**
 * @addtogroup LIBKWB
 *
 * @{
 * @file    jsonhelper.hpp
 * @brief   Convert from JSON object to number and string and vice versa.
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

#include <nlohmann/json.hpp>

#include "exceptions.h"
#include "numberformat.hpp"

/**
 * Convert value of JSON structure into integer number
 * @param[in]   jobject JSON object containing the key/value pair
 * @param[in]   key     Key of key/value pair
 * @param[in]   format  Expected value format, use eUNSPECIFIED to automatically convert.
 * @param[in]   isSignedFormat  The number shall be interpreted as signed number
 * @return Value of type T
 * @throws InvalidParameter if string cannot be converted into number in desired format.
 */
template <typename T>
T json2number(const nlohmann::json &jobject, const std::string &key, const ValueFormat &format, bool isSignedFormat)
{
    T value = 0;
    if (jobject.is_null() || !jobject.contains(key)) {
        throw InvalidParameter(LOC, "Key/value pair for key '%s' is missing!", key.c_str());
    }

    auto jo = jobject[key];
    if (jo.is_number_integer()) {
        value = jo;
    }
    else if (jo.is_string()) {
        value = str2number<T>(jo, ValueFormat::eUNSPECIFIED, false);
    }
    else {
        throw InvalidParameter(LOC, "'%s' is not a hexadecimal, octal (as string) or decimal number!", key.c_str());
    }
    return value;
}

/**
 * Convert JSON key/value pair to string.
 * @param[in]   jobject JSON object containing the key/value pair
 * @param[in]   key     Key of key/value pair
 * @returns value as string.
 * @throws InvalidParameter if value could not be parsed from given JSON object.
 */
std::string json2string(const nlohmann::json &jobject, const std::string &key)
{
    if (jobject.is_null()) {
        throw InvalidParameter(LOC, "Given JSON object is null!");
    }
    auto &jo = jobject[key];
    if (jo.is_null()) {
        throw InvalidParameter(LOC, "Key/value pair for key '%s' is missing!", key.c_str());
    }
    if (!jo.is_string()) {
        throw InvalidParameter(LOC, "Value of key '%s' is not a string!", key.c_str());
    }
    return jo;
}

/** @} */
