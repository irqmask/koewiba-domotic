/**
 * @addtogroup MOTOR
 * @brief This module contains functions to control the motor relays.
 *
 * @{
 * @file    motor.c
 * @brief   This module contains functions to control the motor relays.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "appconfig.h"
#include "clock.h"
#include "motor.h"
#include "sleepmode.h"

// --- Definitions -------------------------------------------------------------

#define MOTOR_UPDOWN_PIN	PC0
#define MOTOR_OFFON_PIN		PC1

//! Delay time in milliseconds between switching on/off and up/down relay
#define MOTOR_SWITCH_DELAY  50

// --- Type definitions --------------------------------------------------------

typedef enum {
    idle,
    start_moving_up,
    moving_up,
    start_moving_down,
    moving_down,
    stopping
} motor_state_t;

// --- Local variables ---------------------------------------------------------

static motor_state_t g_motor_state = idle;
static motor_state_t g_last_motor_state = idle;
static clock_timer_t g_motor_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize motor module. Configure output pins.
 */
void motor_initialize       (void)
{
    DDRC |= ((1<<MOTOR_UPDOWN_PIN) | (1<<MOTOR_OFFON_PIN));
    PORTC &= ~((1<<MOTOR_UPDOWN_PIN) | (1<<MOTOR_OFFON_PIN));
    g_motor_state = idle;
    g_last_motor_state = idle - 1;
}

void motor_up               (void)
{

    if (g_motor_state == idle) {
        g_motor_state = start_moving_up;
        PORTC |= (1<<MOTOR_UPDOWN_PIN);
        clk_timer_start(&g_motor_timer, CLOCK_MS_2_TICKS(MOTOR_SWITCH_DELAY));
        sleep_prevent(APP_eSLEEPMASK_MOTOR, 1);
    } else {
        motor_stop();
    }
}

void motor_down             (void)
{
    if (g_motor_state == idle) {
        g_motor_state = start_moving_down;
        PORTC &= ~(1<<MOTOR_UPDOWN_PIN);
        clk_timer_start(&g_motor_timer, CLOCK_MS_2_TICKS(MOTOR_SWITCH_DELAY));
        sleep_prevent(APP_eSLEEPMASK_MOTOR, 1);
    } else {
        motor_stop();
    }
}

void motor_stop             (void)
{
    if (g_motor_state != idle) {
        g_motor_state = stopping;
        PORTC &= ~(1<<MOTOR_OFFON_PIN);
        clk_timer_start(&g_motor_timer, CLOCK_MS_2_TICKS(MOTOR_SWITCH_DELAY));
        sleep_prevent(APP_eSLEEPMASK_MOTOR, 0);
    }
}

bool motor_is_running       (void)
{
    return (g_motor_state != idle);
}

void motor_background       (void)
{
    switch (g_motor_state) {
    case idle:
        break;

    case start_moving_up:
        if (clk_timer_is_elapsed(&g_motor_timer)) {
            g_motor_state = moving_up;
            PORTC |= (1<<MOTOR_OFFON_PIN);
        }
        break;

    case moving_up:
        break;

    case start_moving_down:
        if (clk_timer_is_elapsed(&g_motor_timer)) {
            g_motor_state = moving_down;
            PORTC |= (1<<MOTOR_OFFON_PIN);
        }
        break;

    case moving_down:
        break;

    case stopping:
        if (clk_timer_is_elapsed(&g_motor_timer)) {
            g_motor_state = idle;
            PORTC &= ~(1<<MOTOR_UPDOWN_PIN);
        }
        break;

    default:
        break;
    }
}
/** @} */
