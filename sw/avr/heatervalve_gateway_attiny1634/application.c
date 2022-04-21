/**
 * @addtogroup HEATERVALVE_GATEWAY_ATTINY1634
 * @addtogroup APPLICATION
 * @brief Application specific code of "heatervalve_gateway_attiny1634" project.
 *
 * Contains application specific initialization, command-interpreter,
 * register code and background loop.
 *
 * @{
 * @file    application.c
 * @brief   Application specific code of "heatervalve_gateway_attiny1634" project.
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
// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include "prjtypes.h"
#include "timer.h"
#include "uart.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t g_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

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
    DDRB |= ((1<<PB1) | (1<<PB2));
    PORTB &= ~((1<<PB1) | (1<<PB2));
    PUEB &= ~((1<<PB1) | (1<<PB2));
        //uart_init_blk1(9600);
    //uart_put_string_blk1("Hello KWB world!\n");
    //TODO insert application specific initializations here!
    timer_start(&g_timer, TIMER_MS_2_TICKS(1000));
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

bool toggle = false;
/**
 * Application specific background code.
 *
 * Executed once per main loop cycle.
 */
void app_background (void)
{
    if (timer_is_elapsed(&g_timer)) {
        //uart_put_char_blk1('.');

        if (toggle) {
            toggle = false;
            PORTB |= (1<<PB1);
        } else {
            toggle = true;
            PORTB &= ~(1<<PB1);
        }
        timer_start(&g_timer, TIMER_MS_2_TICKS(100));
    }

    /*if (uart_is_rx_pending1()) {
        char c = uart_get_char_blk1();
        uart_put_char_blk1(c);
    }*/
    //TODO insert application specific background routines here!
}
/** @} */
