/**
 * @addtogroup STM8_ADC
 * @brief A module to interface STM8 AD converter.
 *
 * @{
 * @file    adc.h
 * @brief   A module to interface STM8 AD converter.
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

#ifndef _STM8_ADC_H_
#define _STM8_ADC_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

// --- Definitions -------------------------------------------------------------


// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void adc_initialize(void);

uint16_t adc_read(void);

#endif // _STM8_ADC_H_
/** @} */
