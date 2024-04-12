/**
 * @addtogroup SN74595
 * @brief This module contains functions to control a 74595 latch.
 *
 * This module contains functions to control a 74595 latch using SPI0 and
 * additional latch and output-enable pins.
 *
 * @{
 * @file    sn74595.h
 * @brief   This module contains functions to control a 74595 latch.
 *
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
#ifndef _SN74595_H_
#define _SN74595_H_

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

void sn74595_initialize     (void);

void sn74595_latch          (void);

void sn74595_OE_on          (void);

void sn74595_OE_off         (void);

void sn74595_set_byte       (uint8_t byte_idx, uint8_t data, uint8_t mask);

void sn74595_send           (void);

void sn74595_send_multiple  (uint8_t *data, uint8_t length);

#endif /* _SN74595_H_ */
/** @} */
