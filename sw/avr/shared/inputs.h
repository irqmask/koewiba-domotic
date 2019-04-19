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

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection INPUTS_PCBCONFIG
 * Configure input Pinout.
 * @{
 */
#ifndef INPUTS_PCBCONFIG
 #define INPUTS_PCBCONFIG   1
 #define INPUT_PORT         PORTC
 #define INPUT_DDR          DDRC
 #define INPUT_PIN          PINC
 #define INPUT_0_PIN        PC0
 #define INPUT_1_PIN        PC1
 #define INPUT_2_PIN        PC2
 #define INPUT_PCMSK        PCMSK1
 #define INPUT_PCMSK_VAL    ((1<<PCINT8) | (1<<PCINT9) | (1<<PCINT10))
 #define NBR_OF_INPUTS      3
#endif // INPUTS_PCBCONFIG

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void input_initialize       (void);

void input_background       (void);

bool input_active           (uint8_t input_nbr);

bool input_on_activation    (uint8_t input_nbr);

#endif /* _INPUTS_H_ */
/** @} */
