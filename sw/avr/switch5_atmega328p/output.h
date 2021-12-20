/**
 * @addtogroup RELAY_OUTPUT
 * @brief This module contains functions to control the relay output.
 * signals.
 *
 * @{
 * @file    output.h
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize output data and i/o
 */
void output_initialize(void);

void output_set(uint8_t idx, uint8_t v);

void output_toggle(uint8_t idx);

uint8_t output_get_value(uint8_t idx);

void output_set_threshold_off(uint8_t chn, uint8_t throff);

void output_set_threshold_on(uint8_t chn, uint8_t thron);

#endif // _OUTPUT_H_
/** @} */
