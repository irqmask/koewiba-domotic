/**
 * @addtogroup STM8_TIMER
 * @brief A module containing simple timing routines.
 *
 * @{
 * @file    timer.h
 * @brief   A module containing simple timing routines.
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

#ifndef _STM8_TIMER_H_
#define _STM8_TIMER_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void timer_initialize(void);

void timer_irq_handler(void);

uint32_t timer_get_millis(void);

void delay_ms(uint16_t ms);


#endif // _STM8_TIMER_H_
/** @} */
