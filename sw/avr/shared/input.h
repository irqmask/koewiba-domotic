/**
 * @addtogroup INPUT
 * @brief This module contains functions to read, debounce and evaluate input 
 * signals.
 *
 * @{
 * @file    input.h
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
#ifndef _INPUT_H_
#define _INPUT_H_

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
 * Initialize input ports.
 */
void input_initialize(void);

/**
 * Input background loop
 * This function performs actual input reading and debouncing.
 */
void input_background(void);

/**
 * Get current input state.
 * @returns current state of inputs
 */
uint8_t input_state(void);

/**
 * Get inputs where inputs went high.
 * @returns rising edges on inputs
 */
uint8_t input_went_high(void);

/**
 * Get inputs where inputs went low.
 * @returns falling edges on inputs
 */
uint8_t input_went_low(void);

/**
 * Get inputs where inputs has changed.
 * @returns toggled inputs
 */
uint8_t input_toggled(void);

#endif // _INPUT_H_
/** @} */
