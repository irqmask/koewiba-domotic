/**
 * @addtogroup DEBUG
 * @brief   Debug output via LEDs.
 *
 * @{
 * @file    led_debug.c
 * @brief   Debug output via LEDs.
 * @author  Robert Mueller
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
// --- Include section ---------------------------------------------------------
#include "led_debug.h"
#include "ucontroller.h"
#include <avr/interrupt.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------
#if 0
/**
 * Toggle the status LED.
 */
void dbg_toggle_status_LED (void)
{
	LED_STATUS_PORT ^= (1<<LED_STATUS);
}

void dbg_status_LED_on (void)
{
	LED_STATUS_PORT |= (1<<LED_STATUS);
}

void dbg_status_LED_off (void)
{
	LED_STATUS_PORT &= ~(1<<LED_STATUS);
}

void dbg_toggle_error_LED (void)
{
	LED_ERROR_PORT ^= (1<<LED_ERROR);
}

void dbg_error_LED_on (void)
{
	LED_ERROR_PORT |= (1<<LED_ERROR);
}

void dbg_error_LED_off (void)
{
	LED_ERROR_PORT &= ~(1<<LED_ERROR);
}
#endif
// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------
