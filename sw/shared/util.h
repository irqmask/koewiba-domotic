/**
 * @addtogroup UTIL
 * @brief   Collection of commonly used functions.
 *
 * @{
 * @file    util.h
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
#ifndef _UTIL_H_
#define _UTIL_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

bool decode_hex8(const char* buf, uint8_t *value);

bool decode_hex16(const char* buf, uint16_t *value);

#endif // _UTIL_H_
/** @} */
