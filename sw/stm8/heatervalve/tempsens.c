/**
 * @addtogroup STM8_ADC
 *
 * @{
 * @file    tempsens.c
 * @brief   A module to interface STM8 AD converter.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@web.de>
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

#include "tempsens.h"

#include <stdio.h>

// include
#include "stm8l052c6.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#include "adc.h"
#include "debug.h"

// --- Definitions -------------------------------------------------------------

//! ADC channel of temperature sensor
#define TEMPSENS_ADC    24
//! Linear correction factor
#define TEMP_M          (-43)
//! Linear correction offset
#define TEMP_B          3135

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize the temperature sensor.
 */
void tempsens_initialize(void)
{
    // enable power for temperature sensor
    PD_DDR |= (1 << 4);
    PD_CR1 |= (1 << 4);
    PD_ODR &= ~(1 << 4);
}

/**
 * Start temperatur sensor conversion
 */
void tempsens_start_conversion(void)
{
    if (adc_running() == false) {
        PD_ODR |= (1 << 4); // switch sensor on
        adc_start(TEMPSENS_ADC);
    }
}

/**
 * Read the temperature value.
 */
int16_t tempsens_read(void)
{
    int16_t adcv, temp;
    adcv = adc_read();
    // convert ADC value into temperature
    // T = (adc - TEMP_B) * 10 / TEMP_M
    adcv = adcv - TEMP_B;
    // multiply by 10: v*10 = v*8 + v*2
    temp = adcv << 3;
    adcv = temp + (adcv << 1);
    // divide by TEMP_M
    adcv = adcv / TEMP_M;
    PD_ODR &= ~(1 << 4); // switch sensor off
    return adcv;
}

/**
 * Start temperature measument and output temperature
 */
void tempsens_background(void)
{
    uint16_t temp_loc;
    if (adc_conversion_ready(TEMPSENS_ADC)) {
        temp_loc = tempsens_read();
        printf("t %04X\n", temp_loc);
    }
}

/** @} */
