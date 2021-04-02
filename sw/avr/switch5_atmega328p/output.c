/**
 * @addtogroup RELAY_OUTPUT
 * @brief This module contains functions to control the relay output.
 * signals.
 *
 * @{
 * @file    output.c
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
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
#include <avr/interrupt.h>

#include "output.h"

// --- Definitions -------------------------------------------------------------

#ifdef HAS_PCBCONFIG_H
#include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
#include "appconfig.h"
#endif

#ifndef OUTPUT_PCBCONFIG
#define OUTPUT_PCBCONFIG     1

#define OUTPUT_NUM_PINS      8
#define OUTPUT_PORT_0        PORTD
#define OUTPUT_DDR_0         DDRD
#define OUTPUT_0             PD3
#define OUTPUT_PORT_1        PORTD
#define OUTPUT_DDR_1         DDRD
#define OUTPUT_1             PD4
#define OUTPUT_PORT_2        PORTD
#define OUTPUT_DDR_2         DDRD
#define OUTPUT_2             PD5
#define OUTPUT_PORT_3        PORTD
#define OUTPUT_DDR_3         DDRD
#define OUTPUT_3             PD6
#define OUTPUT_PORT_4        PORTD
#define OUTPUT_DDR_4         DDRD
#define OUTPUT_4             PD7

#endif

// --- Type definitions --------------------------------------------------------

typedef struct {
    uint8_t value;
    uint8_t threshold_on;
    uint8_t threshold_off;
} output_data_t;

// --- Local variables ---------------------------------------------------------

output_data_t g_data[OUTPUT_NUM_PINS];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void output_hw_set(uint8_t idx, bool state)
{
    if (idx > OUTPUT_NUM_PINS) return;

    cli();
    if (state) {
        switch (idx) {
#if OUTPUT_NUM_PINS > 0
        case 0:
            OUTPUT_PORT_0 |= (1<<OUTPUT_0);
            break;
#endif
#if OUTPUT_NUM_PINS > 1
        case 1:
            OUTPUT_PORT_1 |= (1<<OUTPUT_1);
            break;
#endif
#if OUTPUT_NUM_PINS > 2
        case 2:
            OUTPUT_PORT_2 |= (1<<OUTPUT_2);
            break;
#endif
#if OUTPUT_NUM_PINS > 3
        case 3:
            OUTPUT_PORT_3 |= (1<<OUTPUT_3);
            break;
#endif
#if OUTPUT_NUM_PINS > 4
        case 4:
            OUTPUT_PORT_4 |= (1<<OUTPUT_4);
            break;
#endif
#if OUTPUT_NUM_PINS > 5
        case 5:
            OUTPUT_PORT_5 |= (1<<OUTPUT_5);
            break;
#endif
#if OUTPUT_NUM_PINS > 6
        case 6:
            OUTPUT_PORT_6 |= (1<<OUTPUT_6);
            break;
#endif
#if OUTPUT_NUM_PINS > 7
        case 7:
            OUTPUT_PORT_7 |= (1<<OUTPUT_7);
            break;
#endif
        default:
            break;
        }
    }
    else { // state == false
        switch (idx) {
#if OUTPUT_NUM_PINS > 0
        case 0:
            OUTPUT_PORT_0 &= ~(1<<OUTPUT_0);
            break;
#endif
#if OUTPUT_NUM_PINS > 1
        case 1:
            OUTPUT_PORT_1 &= ~(1<<OUTPUT_1);
            break;
#endif
#if OUTPUT_NUM_PINS > 2
        case 2:
            OUTPUT_PORT_2 &= ~(1<<OUTPUT_2);
            break;
#endif
#if OUTPUT_NUM_PINS > 3
        case 3:
            OUTPUT_PORT_3 &= ~(1<<OUTPUT_3);
            break;
#endif
#if OUTPUT_NUM_PINS > 4
        case 4:
            OUTPUT_PORT_4 &= ~(1<<OUTPUT_4);
            break;
#endif
#if OUTPUT_NUM_PINS > 5
        case 5:
            OUTPUT_PORT_5 &= ~(1<<OUTPUT_5);
            break;
#endif
#if OUTPUT_NUM_PINS > 6
        case 6:
            OUTPUT_PORT_6 &= ~(1<<OUTPUT_6);
            break;
#endif
#if OUTPUT_NUM_PINS > 7
        case 7:
            OUTPUT_PORT_7 &= ~(1<<OUTPUT_7);
            break;
#endif
        default:
            break;
        }
    }
    sei();
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void output_initialize(void)
{
#if OUTPUT_NUM_PINS > 0
    OUTPUT_DDR_0 |= (1<<OUTPUT_0);
    OUTPUT_PORT_0 &= ~(1<<OUTPUT_0);
#endif
#if OUTPUT_NUM_PINS > 1
    OUTPUT_DDR_1 |= (1<<OUTPUT_1);
    OUTPUT_PORT_1 &= ~(1<<OUTPUT_1);
#endif
#if OUTPUT_NUM_PINS > 2
    OUTPUT_DDR_2 |= (1<<OUTPUT_2);
    OUTPUT_PORT_2 &= ~(1<<OUTPUT_2);
#endif
#if OUTPUT_NUM_PINS > 3
    OUTPUT_DDR_3 |= (1<<OUTPUT_3);
    OUTPUT_PORT_3 &= ~(1<<OUTPUT_3);
#endif
#if OUTPUT_NUM_PINS > 4
    OUTPUT_DDR_4 |= (1<<OUTPUT_4);
    OUTPUT_PORT_4 &= ~(1<<OUTPUT_4);
#endif
#if OUTPUT_NUM_PINS > 5
    OUTPUT_DDR_5 |= (1<<OUTPUT_5);
    OUTPUT_PORT_5 &= ~(1<<OUTPUT_5);
#endif
#if OUTPUT_NUM_PINS > 6
    OUTPUT_DDR_6 |= (1<<OUTPUT_6);
    OUTPUT_PORT_6 &= ~(1<<OUTPUT_6);
#endif
#if OUTPUT_NUM_PINS > 7
    OUTPUT_DDR_7 |= (1<<OUTPUT_7);
    OUTPUT_PORT_7 &= ~(1<<OUTPUT_7);
#endif

    for (uint8_t idx=0; idx<OUTPUT_NUM_PINS; idx++) {
        g_data[idx].value = 0;
        g_data[idx].threshold_on = 127;
        g_data[idx].threshold_off = 32;
    }
}

void output_set(uint8_t chn, uint8_t v)
{
    if (chn >= OUTPUT_NUM_PINS) {
        return;
    }

    if (v > g_data[chn].threshold_on) {
        g_data[chn].value = 255;
        output_hw_set(chn, true);
    }
    if (v < g_data[chn].threshold_off) {
        g_data[chn].value = 0;
        output_hw_set(chn, false);
    }
}

void output_toggle(uint8_t chn)
{
    if (chn >= OUTPUT_NUM_PINS) {
        return;
    }
    // toggle to off
    if (g_data[chn].value > g_data[chn].threshold_on) {
        g_data[chn].value = 0;
        output_hw_set(chn, false);
    }
    // toggle to on
    else if (g_data[chn].value < g_data[chn].threshold_off) {
        g_data[chn].value = 255;
        output_hw_set(chn, true);
    }
}

uint8_t output_get_value(uint8_t chn)
{
    if (chn >= OUTPUT_NUM_PINS) {
        return 0;
    }
    return g_data[chn].value;
}

void output_set_threshold_off(uint8_t chn, uint8_t throff)
{
    if (chn >= OUTPUT_NUM_PINS) {
        return;
    }
    g_data[chn].threshold_off = throff;
}

void output_set_threshold_on(uint8_t chn, uint8_t thron)
{
    if (chn >= OUTPUT_NUM_PINS) {
        return;
    }
    g_data[chn].threshold_on = thron;
}
