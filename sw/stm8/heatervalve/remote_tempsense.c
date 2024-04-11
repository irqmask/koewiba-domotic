/**
 * @addtogroup REMOTE_TEMPSENSE
 *
 * @{
 * @file    remote_tempsense.c
 * @brief   Remote temperature sensor module. Receives current temperature and
 *          desired temperature over uart, sends desired temperature over uart.
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

// --- Include section ---------------------------------------------------------

#include "remote_tempsense.h"

#include <stdint.h>

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint16_t g_desired_temp;
static uint16_t g_current_temp;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize remote temperature sensor interface.
 */
void remts_initialize(void)
{
    g_desired_temp = 27315 + 1500; // 15°C
    g_current_temp = 27315 + 1500; // 15°C
}


/**
 * @returns desired temperature in 1/100Kelvin
 */
uint16_t remts_get_desired_temp()
{
    return g_desired_temp;
}


/**
 * @returns current temperature in 1/100Kelvin
 */
uint16_t remts_get_current_temp()
{
    return g_current_temp;
}


/**
 * set desired temperature
 * @param[in]   temp    Desired temperature in 1/100K;
 */
void remts_set_desired_temp(uint16_t temp)
{
    g_desired_temp = temp;
}


/**
 * set current temperature (measure elsewhere)
 * @param[in]   temp    Current temperature in 1/100K;
 */
void remts_set_current_temp(uint16_t temp)
{
    g_current_temp = temp;
}

/** @} */
