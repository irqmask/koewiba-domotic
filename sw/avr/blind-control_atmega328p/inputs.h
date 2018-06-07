/**
 * @addtogroup INPUTS
 * @brief Public interface to detect the state of inputs.
 *
 * This module contains functions to detect the state of inputs.
 *
 * @{
 * @file    inputs.h
 * @brief   This module contains functions to detect the state of inputs.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@web.de>
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

#ifndef _INPUTS_H_
#define _INPUTS_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void input_initialize       (void);

bool input_up               (void);

bool input_down             (void);

bool input_window_closed    (void);

void input_background       (void);

#endif /* _INPUTS_H_ */
/** @} */
