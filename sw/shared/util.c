/**
 * @addtogroup UTIL
 *
 * @{
 * @file    util.c
 * @brief   Collection of commonly used functions.
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

// --- Include section ---------------------------------------------------------

#include "util.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

bool decode_nibble(char c, uint8_t *nibble)
{
    if (c >= '0' && c <= '9') {
        *nibble = c - '0';
    }
    else if (c >= 'a' && c <= 'f') {
        * nibble = 10 + c - 'a';
    }
    else if (c >= 'A' && c <= 'F') {
        * nibble = 10 + c - 'A';
    }
    else {
        return false;
    }
    return true;
}

bool decode_hex8(const char* buf, uint8_t *value)
{
    uint8_t idx, val = 0, nibble;
    for (idx = 0; idx < 2; idx++) {
        if (decode_nibble(buf[idx], &nibble)) {
            val <<= 4;
            val |= nibble;
        }
        else {
            return false;
        }
    }
    *value = val;
    return true;
}

bool decode_hex16(const char* buf, uint16_t *value)
{
    uint16_t val = 0;
    uint8_t idx, nibble = 0;

    for (idx = 0; idx < 4; idx++) {
        if (decode_nibble(buf[idx], &nibble)) {
            val <<= 4;
            val |= nibble;
        }
        else {
            return false;
        }
    }
    *value = val;
    return true;
}

/** @} */
