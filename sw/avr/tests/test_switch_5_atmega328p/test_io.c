/*
 * @file test_io.c
 * Test to check inputs and outputs.
 * Connect output 0...4 to input 0...4
 *
 * @author robert mueller
 */
/*
 * Copyright (C) 2019  robert <djrfm@gmx.de>
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
#include <avr/io.h>
#include "timer.h"
#include "bus.h"

#define CHASER_DELAY_MS 250

// --- Local variables ---------------------------------------------------------
static uint8_t g_test_is_ok = 1;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

static timer_data_t g_test_timer;

void output(uint8_t index, bool on)
{
    uint8_t pin = 0;

    switch (index) {
    case 0:
        pin = PD7;
        break;
    case 1:
        pin = PD6;
        break;
    case 2:
        pin = PD5;
        break;
    case 3:
        pin = PD4;
        break;
    case 4:
        pin = PD3;
        break;
    default:
        return;
    }
    if (on)
        PORTD |= (1 << pin);
    else
        PORTD &= ~(1 << pin);
}

bool input(uint8_t index)
{
    uint8_t pin = 0;

    switch (index) {
    case 0:
        pin = PINC0;
        break;
    case 1:
        pin = PINC1;
        break;
    case 2:
        pin = PINC2;
        break;
    case 3:
        pin = PINC3;
        break;
    case 4:
        pin = PINC4;
        break;
    default:
        return false;
    }
    return (0 == (PINC &= (1 << pin)));
}

void test_io_initialize(void)
{
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4));
    PORTC |= ((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4));
    DDRD |= ((1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
    PORTD &= ~((1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
    timer_start(&g_test_timer, TIMER_MS_2_TICKS(CHASER_DELAY_MS));
    g_test_is_ok = true;
}

uint8_t test_io_is_ok(void)
{
    return g_test_is_ok;
}

uint8_t test_io_run(void)
{
    static uint8_t relay = 0;
    static uint8_t step = 0;

    if (relay == 5) {
        relay++;
        return 0;
    }
    else if (relay > 5) {
        return 255;
    }

    if (timer_is_elapsed(&g_test_timer)) {
        timer_start(&g_test_timer, TIMER_MS_2_TICKS(CHASER_DELAY_MS));

        switch (step) {
        case 0:
            output(relay % 5, false);
            step = 1;
            break;

        case 1:
            // input must not be set, when relay is not active
            if (true == input(relay)) {
                g_test_is_ok = false;
            }
            else {
                // set relay active for next input check
                output(relay % 5, true);
                step = 2;
            }
            break;

        case 2:
            if (false == input(relay)) {
                g_test_is_ok = false;
            }
            output(relay % 5, false);
            step = 3;
            break;

        case 3:
            // input must not be set, when relay is not active
            if (true == input(relay)) {
                g_test_is_ok = false;
            }
            else {
                step = 0;
                relay++;
            }
            break;
        default:
            break;
        }
    }

    return 1;
}
