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
 * Copyright (C) 2021  christian <irqmask@web.de>
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
#include <avr/interrupt.h>

#include "alarmclock.h"
#include "datetime.h"
#include "input.h"
#include "led_debug.h"
#include "output.h"
#include "register.h"

#include "appconfig.h"
#include "pcbconfig.h"
#include "application.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

enum {
    MODE_INPUT_SWITCH_BIT = 0,
    MODE_LAST_BIT
} app_mode_bits_t;

// --- Local variables ---------------------------------------------------------

static timer_data_t g_seconds_timer;
static timer_data_t g_input_timer;

// --- Module global variables -------------------------------------------------

uint8_t app_chn_mode[APP_NUM_CHANNEL];

// --- Global variables --------------------------------------------------------

extern sBus_t g_bus;

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void app_send_state(uint8_t chn, uint8_t state)
{
    uint8_t reg_no = APP_eReg_Chn0_SwitchCurrent;
    reg_no += (chn * APP_REGS_PER_CHN);
    register_send_u8(&g_bus, BUS_BRDCSTADR, reg_no, state);
}


void app_toggle_output(uint8_t chn)
{
    output_toggle(chn);
    app_send_state(chn, output_get_value(chn));
}


void app_set_output(uint8_t chn, uint8_t value)
{
    output_set(chn, value);
    app_send_state(chn, output_get_value(chn));
}

void app_set_mode(uint8_t chn, uint8_t mode)
{
    if (chn >= APP_NUM_CHANNEL) {
        return;
    }
    app_chn_mode[chn] = mode;
}

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
    input_initialize();
    PCMSK1 |= ((1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12));

    output_initialize();

    // register_set_u16(MOD_eReg_ModuleID, 0x20);
    dt_initialize();

    app_register_load();

    timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
    timer_start(&g_input_timer, TIMER_MS_2_TICKS(20));

    LED_STATUS_DDR |= (1<<LED_STATUS);
    LED_ERROR_DDR |= (1<<LED_ERROR);
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
    if (timer_is_elapsed(&g_seconds_timer)) {
        timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
        dt_tick_second();
    }

    if (timer_is_elapsed(&g_input_timer)) {
        timer_start(&g_input_timer, TIMER_MS_2_TICKS(10));
        input_background();

        uint8_t it = input_toggled();
        uint8_t ife = input_went_low();

        for (uint8_t idx=0; idx<APP_NUM_CHANNEL; idx++) {
            if (app_chn_mode[idx] & (1 << MODE_INPUT_SWITCH_BIT)) {
                // input is a switch -> react on changing input
                if (it & (1 << idx)) {
                    app_toggle_output(idx);
                }
            }
            else {
                // input is toggling (e.g.a push-button) -> react on button release
                if (ife & (1 << idx)) {
                    app_toggle_output(idx);
                }
            }
        }
    }
}
/** @} */
