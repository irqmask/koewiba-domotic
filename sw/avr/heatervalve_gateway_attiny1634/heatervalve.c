/**
 * @addtogroup HEATERVALVE
 *
 * @{
 * @file    heatervalve.c
 * @brief   API for heatervalve sub-module.
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

#include "heatervalve.h"

#include <stdint.h>
#include <avr/io.h>

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif
#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

#include "prjtypes.h"
#include "uart.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize heatervalve sub-module
 */
void hv_initialize(void)
{
    DDRB &= ~(1<<PB3);
    PORTB &= ~(1<<PB3);

    hv_reset(true);
    uart_init_blk1(UART_BAUDRATE);
    hv_reset(false);
}

/**
 * Activate or deactivate heatervalve's reset line.
 * @param[in]   on  true, activate reset line, false, release reset line.
 */
void hv_reset(bool on)
{
    if (on) {
        DDRB |= (1<<PB3);
    }
    else {
        DDRB &= ~(1<<PB3);
    }
}

/**
 * Heatervalve background processing, e.g. listening for incoming messages from sub-module
 */
void hv_background(void)
{

}

/** @} */
