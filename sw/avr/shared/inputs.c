/**
 * @addtogroup INPUTS
 * @brief This module contains functions to detect the state of inputs.
 *
 * Inputs have negative logic, due to pull-up resistor.
 * An input is activated, when the input is electrically low.
 * @{
 * @file    inputs.c
 * @brief   This module contains functions to detect the state of inputs.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2019  christian <irqmask@web.de>
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

#include <avr/io.h>

#include "timer.h"

#include "inputs.h"

// --- Definitions -------------------------------------------------------------

#define INPUT_DEBOUNCE_TIME     80
#define INPUT_TIMER_INTERVAL    (INPUT_DEBOUNCE_TIME / 8)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t     g_input_timer;
static uint8_t          g_debounce_array[NBR_OF_INPUTS];
static bool             g_last_state[NBR_OF_INPUTS];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize input module. Configure input pins.
 */
void input_initialize       (void)
{
    uint8_t n;

    INPUT_DDR &= ~((1<<INPUT_0_PIN) | (1<<INPUT_1_PIN) | (1<<INPUT_2_PIN));
    // activate internal pull-up resistors
    INPUT_PORT |= ((1<<INPUT_0_PIN) | (1<<INPUT_1_PIN) | (1<<INPUT_2_PIN));

    // activate pin-change-interrupts for the inputs
    INPUT_PCMSK |= INPUT_PCMSK_VAL;

    // clear debounce array
    for(n=0; n<NBR_OF_INPUTS; n++) {
        g_debounce_array[n] = 0xFF;
        g_last_state[n] = true;
    }
    timer_start(&g_input_timer, TIMER_MS_2_TICKS(INPUT_TIMER_INTERVAL));
}

void input_background       (void)
{
    uint8_t n, pin;

    if (timer_is_elapsed(&g_input_timer)) {
        timer_start(&g_input_timer, TIMER_MS_2_TICKS(INPUT_TIMER_INTERVAL));
        // this part is called every INPUT_TIMER_INTERVAL milliseconds.
        for(n=0; n<NBR_OF_INPUTS; n++) {
            switch (n)
            {
            case 0: pin = 1<<INPUT_0_PIN; break;
            case 1: pin = 1<<INPUT_1_PIN; break;
            case 2: pin = 1<<INPUT_2_PIN; break;
            default: pin = 0; break;
            }
            g_debounce_array[n] <<= 1;
            g_debounce_array[n] |= (INPUT_PIN & pin) ? 1:0;
        }
    }
}


bool input_active           (uint8_t input_nbr)
{
    // Inputs have negative logic, due to pull-up resistor.
    // An input is activated, when the input is electrically low.
    return (g_debounce_array[input_nbr] == 0xFF);
}

bool input_on_activation    (uint8_t input_nbr)
{
    bool temp;

    temp = input_active(input_nbr);
    if (g_last_state[input_nbr] != temp) {
        g_last_state[input_nbr] = temp;
        return temp;
    }
    return false;
}

/* bool input_on_deactivation  (uint8_t input_nbr)
{
    bool temp;

    temp = input_active(input_nbr);
    if (g_last_state[input_nbr] != temp) {
        g_last_state[input_nbr] = temp;
        return !temp;
    }
    return false;
}*/

/** @} */
