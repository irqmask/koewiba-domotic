/**
 * @addtogroup HEATERVALVE
 * API for heatervalve sub-module
 *
 * @{
 * @file    heatervalve.h
 * @brief   API for heatervalve sub-module
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

#ifndef _HEATERVALVE_H_
#define _HEATERVALVE_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void hv_initialize(void);

void hv_reset(bool on);

void hv_background(void);

#endif // _HEATERVALVE_H_
/** @} */
