/**
 * @addtogroup BLINDCONTROL
 * @addtogroup APPLICATION
 * @brief Application specific code of "blind-control_atmega328p" project.
 *
 * Contains application specific initialization, command-interpreter,
 * register code and background loop.
 *
 * @{
 * @file    application.c
 * @brief   Application specific code of "blind-control_atmega328p" project.
 *
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

#include "prjtypes.h"

#include <avr/io.h>

#include "alarmclock.h"
#include "blindctrl.h"
#include "bus.h"
#include "cmddef_common.h"
#include "datetime.h"
#include "input.h"
#include "motor.h"
#include "register.h"
#include "timer.h"

// --- Definitions -------------------------------------------------------------

#define APP_INPUT_UP        0   //!< Input key "up"
#define APP_INPUT_DOWN      1   //!< Input key "down"
#define APP_INPUT_WINDOW    2   //!< Input key "window"

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static bool g_window_state = false;
static bool g_last_window_state = true;
static timer_data_t g_seconds_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void send_window_state (sBus_t* bus)
{
    uint8_t msg[4];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_WindowState;      // number of bitfiels bytes
    msg[2] = (g_window_state==true) ? (1<<0) : 0;
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
}

// --- Module global functions -------------------------------------------------

extern void        app_register_load       (void);

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
    INPUT_PCMSK |= INPUT_PCMSK_VAL; // activate pin-change-interrupts for the inputs

    motors_initialize();
    blinds_initialize();
    dt_initialize();
    alarm_initialize();

    // load application parameters
    app_register_load();
    // initialize window statemachine
    g_window_state = false;
    g_last_window_state = !g_window_state;

    timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
}

/**
 * Application specific ISR for pin change interrupt.
 *
 * @param[in] pinchange_interruptflags  The value of PCIFR is for determining, 
 *                                      which register has to be checked for the
 *                                      source of interrupt.
 */
void app_on_pinchangeinterrupt(uint8_t pinchange_interruptflags)
{
    (void)pinchange_interruptflags;
    // nothing to do, here
}

/**
 * Application specific command interpreter code.
 *
 * Executed if a not-common command is received.
 */
void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    uint8_t blind_index = 0;

    switch (msg[0]) {
    case APP_eCmd_Stop:
        blind_index = msg[1];
        if (blind_index < BLIND_COUNT) blind_stop(blind_index);
        break;

    default:
        break;
    }
}

/**
 * Application specific background code.
 *
 * Executed once per main loop cycle.
 */
void app_background (sBus_t* bus)
{
    input_background();

    // check keys
    uint8_t keys = input_went_low();

    if (keys & (1<<APP_INPUT_UP)) {
        blind_move_to_position(0, 0);
    }
    if (keys & (1<<APP_INPUT_DOWN)) {
        blind_move_to_position(0, 100);
    }

    // check window position
    g_window_state = (input_state() & (1<<APP_INPUT_WINDOW)) != 0;
    if (g_last_window_state != g_window_state) {
        g_last_window_state = g_window_state;
        send_window_state(bus);
    }

    motors_background();
    blinds_background();

    if (timer_is_elapsed(&g_seconds_timer)) {
        timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
        dt_tick_second();
    }
}

/**
 *  Check every minute if one of the up/down alarms triggers.
 */
void app_on_minute(void)
{
    int8_t alarm_idx = -1;
    bool move_up = false;

    if (alarm_check(&alarm_idx)) {
        if (blind_is_moving(0) == false) {
            move_up = ((alarm_idx % 2) == 0);
            if (move_up) {
                blind_move_to_position(0, 0);
            } else {
                blind_move_to_position(0, 100);
            }
        }
    }
}

/** @} */
