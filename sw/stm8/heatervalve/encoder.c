/**
 * @addtogroup ENCODER
 *
 * @{
 * @file    encoder.c
 * @brief   A module to interface the heatervalve's encoder.
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

#include "encoder.h"

// include
#include "stm8l052c6.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

#define ENC_DDR     PC_DDR
#define ENC_INPUT   PC_IDR
#define ENC_CR1     PC_CR1
#define ENC_A       PIN_0
#define ENC_B       PIN_1
#define ENC_MASK    (ENC_A | ENC_B)

#define ENC_STATE_00 0
#define ENC_STATE_01 ENC_A
#define ENC_STATE_10 ENC_B
#define ENC_STATE_11 ENC_A | ENC_B

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint16_t g_encval;
static uint16_t g_minval;
static uint16_t g_maxval;
static uint8_t g_old_input_state;
static uint8_t g_input_state;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void increase(void)
{
    if (g_encval < g_maxval)
        g_encval++;
}

static void decrease(void)
{
    if (g_encval > g_minval)
        g_encval--;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize Encoder hardware and data.
 * @param[in]   minval      Lowest allowed value
 * @param[in]   maxval      Highest allowed value
 */
void enc_initialize(uint16_t minval, uint16_t maxval)
{
    ENC_DDR &= ~ENC_MASK;   // configure as input
    ENC_CR1 |= ENC_MASK;    // activate internal pull-up resistor
    g_encval = minval;

    enc_set_minmax(minval, maxval);

    g_old_input_state = ENC_INPUT & ENC_MASK;
    g_input_state = g_old_input_state;
}

/**
 * Set minimum and maximum value.
 * @param[in]   minval      Lowest allowed value
 * @param[in]   maxval      Highest allowed value
 */
void enc_set_minmax(uint16_t minval, uint16_t maxval)
{
    if (minval < maxval) {
        g_minval = minval;
        g_maxval = maxval;
    }
    else {
        g_maxval = minval;
        g_minval = maxval;
    }
}

/**
 * Called ideally by timer interrupt. Reads encoder input and
 * increments / decrements encoder value accordingly.
 */
void enc_on_irq(void)
{
    g_input_state = ENC_INPUT & ENC_MASK;

    if ( g_input_state == g_old_input_state )
        return;

    switch ( g_old_input_state ) {
    case ENC_STATE_00:
        if ( g_input_state == ENC_STATE_01 )
            increase();
        else if ( g_input_state == ENC_STATE_10 )
            decrease();
        break;
    case ENC_STATE_10:
        // don't count this step because it resides in mechanical unstable
        //state
        // if (new == 0b00) g_uEncValue--;
        // else if (new == 0b10) g_uEncValue++;
        break;
    case ENC_STATE_11:
        if ( g_input_state == ENC_STATE_10 )
            increase();
        else if ( g_input_state == ENC_STATE_01 )
            decrease();
        break;
    case ENC_STATE_01:
        // don't count this step because it resides in mechanical unstable
        //state
        // if (new == 0b11) g_uEncValue--;
        // else if (new == 0b00) g_uEncValue++;
        break;
    }
    g_old_input_state = g_input_state;
}

/**
 * Read current encoder value
 */
uint16_t enc_read()
{
    return g_encval;
}

/** @} */
