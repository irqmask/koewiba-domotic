/**
 * @addtogroup REMOTE_TEMPSENSE
 * @brief Remote temperature sensor module. Receives current temperature and
 *        desired temperature over uart, sends desired temperature over uart.
 *
 * @{
 * @file    remote_tempsense.h
 * @brief   Remote temperature sensor module. Receives current temperature and
 *          desired temperature over uart, sends desired temperature over uart.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
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

#ifndef _REMOTE_TEMPSENSE_H_
#define _REMOTE_TEMPSENSE_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void remts_initialize(void);

uint16_t remts_get_desired_temp();

uint16_t remts_get_current_temp();

void remts_set_desired_temp(uint16_t temp);

void remts_set_current_temp(uint16_t temp);


#endif // _REMOTE_TEMPSENSE_H_
/** @} */
