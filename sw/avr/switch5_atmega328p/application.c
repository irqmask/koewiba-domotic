/**
 * @addtogroup SWITCH5_ATMEGA328
 * @addtogroup APPLICATION
 * @brief Application specific code of "switch5_atmega328" project.
 *
 * Contains application specific initialization, command-interpreter, 
 * register code and background loop.
 * 
 * @{
 * @file    application.c
 * @brief   Application specific code of "switch5_atmega328" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2020  christian <irqmask@web.de>
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

#include "prjtypes.h"

#include <avr/io.h>

#include "alarmclock.h"
#include "datetime.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t g_seconds_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static uint8_t inp(void)
{
    return (PINC & 0x1F);
}

static void outp(uint8_t val)
{
    uint8_t old = PORTD;
    old &= 0x03;
    val <<= 3; // PD3..7
    val &= 0xFC;

    PORTD = old | val;
}


// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Application specific initializations.
 * 
 * Executed if common code initialization passed.
 * @note Global interrupts are still switched off! Will be switched on in global
 * main after this initialization code.
 */
void app_init (void) 
{
    // PORTC 0..4 input keys
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4));
    // input with pull-ups
    PORTC |= ((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4));

    PCMSK1 |= ((1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12));
    // PORTD 3..7 output
    DDRD |= ((1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
    PORTD &= ~((1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));

    //register_set_u16(MOD_eReg_ModuleID, 0x20);
    dt_initialize();

    timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
}

/**
 * Application specific command interpreter code.
 * 
 * Executed if a not-common command is received.
 */
void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    switch (msg[0]) {
    //TODO insert application specific command interpreter here!
    default:
        break;
    }
}

/**
 * Application specific background code.
 * 
 * Executed once per main loop cycle.
 */
void app_background (void)
{
    //TODO insert application specific background routines here!
    if (timer_is_elapsed(&g_seconds_timer)) {
        timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
        dt_tick_second();
    }

    //outp(inp());
}
/** @} */
