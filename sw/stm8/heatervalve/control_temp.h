/**
 * @addtogroup CONTROL_TEMP
 * @brief Module to control the heatervalve depending on current and desired
 *        temperature.
 *
 * @{
 * @file    control_temp.h
 * @brief   Module to control the heatervalve depending on current and desired
 *          temperature.
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

#ifndef _CONTROL_TEMP_H_
#define _CONTROL_TEMP_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void ctrl_set_kp(uint16_t kp);

void ctrl_temp_initialize(void);

void ctrl_temp_background(void);

#endif // _CONTROL_TEMP_H_
/** @} */
