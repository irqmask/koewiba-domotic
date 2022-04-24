/**
 * @addtogroup MOTOR
 * @brief This module contains functions to control the motor relays.
 *
 * @{
 * @file    motor.c
 * @brief   This module contains functions to control the motor relays.
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

#include "appconfig.h"
#include "motor.h"
#include "sleepmode.h"
#include "timer.h"

// --- Definitions -------------------------------------------------------------

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#ifndef MOTOR_PCBCONFIG
 #define MOTOR_PCBCONFIG    1

 //! Number of motors supported by the application
 #define MOTOR_COUNT 1
 #define MOTOR_0_UPDOWN_PORT    PORTC
 #define MOTOR_0_UPDOWN_DDR     DDRC
 #define MOTOR_0_ONOFF_PORT     PORTC
 #define MOTOR_0_ONOFF_DDR      DDRC
 //! Pin to control motor's up/down relay
 #define MOTOR_0_UPDOWN_PIN     PC0
 //! Pin to control motor's off/on relay
 #define MOTOR_0_ONOFF_PIN      PC1
 //! Delay time in milliseconds between switching on/off and up/down relay
 #define MOTOR_SWITCH_DELAY     50
#endif
#if MOTOR_COUNT > 8
#error "BLIND_COUNT too high, maximum number of eight blinds supported!"
#endif

// --- Type definitions --------------------------------------------------------

typedef enum {
    idle,
    start_moving_up,
    moving_up,
    start_moving_down,
    moving_down,
    stopping
} motor_state_t;

//! Control state of the motors.
typedef struct {
    motor_mode_t    motor_mode;         //!< control mode of the motor.
    motor_state_t   motor_state;        //!< current state of the motor.
    motor_state_t   last_motor_state;   //!< last known state of the motor.
    timer_data_t    propagation_delay;  //!< timer to handle switch delay.
} motor_control_t;

// --- Local variables ---------------------------------------------------------

//! storage of the state of the motors.
static motor_control_t g_motor_control[MOTOR_COUNT];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern void motors_relay_initialize(void);
extern void motor_relay_onoff(uint8_t index, bool onoff);
extern void motor_relay_updown(uint8_t index, bool updown);

// --- Global functions --------------------------------------------------------

/**
 * Start moving the motor up.
 * @param[in]   index   Index of the motor (0...MOTOR_COUNT-1)
 * @note up/down relay is switched directly, on/off relay, when MOTOR_SWITCH_DELAY expires.
 */
void motor_up               (uint8_t index)
{
    if (index >= MOTOR_COUNT) return;
    if (g_motor_control[index].motor_state == idle) {
        g_motor_control[index].motor_state = start_moving_up;
        motor_relay_updown(index, true);
        timer_start(&g_motor_control[index].propagation_delay, TIMER_MS_2_TICKS(MOTOR_SWITCH_DELAY));
        sleep_prevent(APP_eSLEEPMASK_MOTOR, 1);
    } else {
        motor_stop(index);
    }
}

/**
 * Start moving the motor down.
 * @param[in]   index   Index of the motor (0...MOTOR_COUNT-1)
 * @note up/down relay is switched directly, on/off relay, when MOTOR_SWITCH_DELAY expires.
 */
void motor_down             (uint8_t index)
{
    if (index >= MOTOR_COUNT) return;
    if (g_motor_control[index].motor_state == idle) {
        g_motor_control[index].motor_state = start_moving_down;
        motor_relay_updown(index, false);
        timer_start(&g_motor_control[index].propagation_delay, TIMER_MS_2_TICKS(MOTOR_SWITCH_DELAY));
        sleep_prevent(APP_eSLEEPMASK_MOTOR, 1);
    } else {
        motor_stop(index);
    }
}

/**
 * Stops the motor immediately.
 * @param[in]   index   Index of the motor (0...MOTOR_COUNT-1)
 * @note on/off relay is switched directly, up/down relay, when MOTOR_SWITCH_DELAY expires.
 */
void motor_stop             (uint8_t index)
{
    if (index >= MOTOR_COUNT) return;
    if (g_motor_control[index].motor_state != idle) {
        g_motor_control[index].motor_state = stopping;
        motor_relay_onoff(index, false);
        timer_start(&g_motor_control[index].propagation_delay, TIMER_MS_2_TICKS(MOTOR_SWITCH_DELAY));
        sleep_prevent(APP_eSLEEPMASK_MOTOR, 0);
    }
}

/**
 * Checks if the given motor is in a running state.
 * @param[in]   index   Index of the motor (0...MOTOR_COUNT-1)
 * @returns true, if the motor is running up or down, otherwise false.
 */
bool motor_is_running       (uint8_t index)
{
    if (index >= MOTOR_COUNT) return false;
    return (g_motor_control[index].motor_state != idle);
}

/**
 * Set output switching mode for the motor.
 */
void motor_set_mode         (uint8_t index, motor_mode_t mode)
{
    if (index >= MOTOR_COUNT || mode >= eMOTOR_MODE_LAST) return;
    g_motor_control[index].motor_mode = mode;
}

/**
 * Initialize motor module. Configure output pins.
 */
void motors_initialize       (void)
{
    motors_relay_initialize();

    for (uint8_t index=0; index<MOTOR_COUNT; index++) {
        g_motor_control[index].motor_state = idle;
        g_motor_control[index].last_motor_state = idle - 1;
    }
}

/**
 * Background task which controls all motors.
 * @note This function need to be called periodically in the application.
 */
void motors_background       (void)
{
    for (uint8_t index=0; index < MOTOR_COUNT; index++) {
        switch (g_motor_control[index].motor_state) {
        case idle:
            break;

        case start_moving_up:
            if (timer_is_elapsed(&g_motor_control[index].propagation_delay)) {
                g_motor_control[index].motor_state = moving_up;
                motor_relay_onoff(index, true);
            }
            break;

        case moving_up:
            break;

        case start_moving_down:
            if (timer_is_elapsed(&g_motor_control[index].propagation_delay)) {
                g_motor_control[index].motor_state = moving_down;
                motor_relay_onoff(index, true);
            }
            break;

        case moving_down:
            break;

        case stopping:
            if (timer_is_elapsed(&g_motor_control[index].propagation_delay)) {
                g_motor_control[index].motor_state = idle;
                motor_relay_updown(index, false);
            }
            break;

        default:
            break;
        }
    }
}
/** @} */
