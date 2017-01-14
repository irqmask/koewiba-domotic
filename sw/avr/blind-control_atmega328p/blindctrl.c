/**
 * @addtogroup BLINDCTRL
 * @brief This module contains functions to control the blind.
 *
 * @{
 * @file    blindctrl.c
 * @brief   This module contains functions to control the blind.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "blindctrl.h"
#include "bus.h"
#include "clock.h"
#include "cmddef_common.h"
#include "motor.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum {
    idle,
    moving_up,
    moving_down,
    stopping
} blind_state_t;

// --- Local variables ---------------------------------------------------------

//! Saves the state of the blind control statemachine.
static blind_state_t g_blind_state = idle;
//! Current position in percent 0% = fully closed 100% = fully open.
static uint8_t g_current_position = 0;
//! Desired position in percent 0% = fully closed 100% = fully open.
static uint8_t g_position_setpoint = 0;
//! Delay time in timer ticks (1/100sec) until movement starts after applying
//! power.
static uint8_t g_reaction_delay = 0;
//! Duration in timer ticks (1/100sec) to fully open the blind.
static uint16_t g_duration_open = CLOCK_MS_2_TICKS(10000);
//! Duration in timer ticks (1/100sec) to fully close the blind.
static uint16_t g_duration_close = CLOCK_MS_2_TICKS(10000);
static clock_timer_t g_blindtimer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Calculate the movement time to move the blinds down, depending on the
 * position setpoint and the current position.
 *
 * @returns Movement duration in timer ticks.
 */
static uint16_t calc_motor_time_down (void)
{
    uint32_t down_duration;
    uint8_t  diff;

    if (g_position_setpoint == 0) {
        down_duration = g_duration_close;
    } else {
        diff = g_current_position - g_position_setpoint;
        down_duration = (g_duration_close * diff) / 100;
    }

    down_duration += g_reaction_delay;
    if (down_duration > 65535) down_duration = 65535;

    return (uint16_t)down_duration;
}

/**
 * Calculate the movement time to move the blinds up, depending on the
 * position setpoint and the current position.
 *
 * @returns Movement duration in timer ticks.
 */
static uint16_t calc_motor_time_up (void)
{
    uint32_t up_duration;
    uint8_t  diff;

    if (g_position_setpoint == 100) {
        up_duration = g_duration_open;
    } else {
        diff = g_position_setpoint - g_current_position;
        up_duration = (g_duration_open * diff) / 100;
    }

    up_duration += g_reaction_delay;
    if (up_duration > 65535) up_duration = 65535;

    return (uint16_t)up_duration;
}

static void send_current_position (sBus_t* bus)
{
    uint8_t msg[3];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_PositionCurrent;
    msg[2] = g_current_position;
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
}

static void send_position_setpoint (sBus_t* bus)
{
    uint8_t msg[3];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_PositionSetPoint;
    msg[2] = g_position_setpoint;
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize blind control data. Assuming blings are closed (0) in the beginning.
 */
void blind_initialize       (void)
{
    g_blind_state = idle;
    g_current_position = 0;
    g_position_setpoint = 0;
}

/**
 * Move the blind to the position.
 *
 * @param[in] position
 * Position to move to. 0 = completely closed. 100 = completely open.
 */
void blind_move_to_position (uint8_t new_position)
{
    if (new_position > 100) new_position = 100;
    if (g_blind_state == idle && new_position != g_current_position) {
        g_position_setpoint = new_position;

        if (new_position < g_current_position) {
            // move down
            clk_timer_start(&g_blindtimer, calc_motor_time_down());
            motor_down();
            g_blind_state = moving_down;
        } else if (new_position > g_current_position) {
            // move up
            clk_timer_start(&g_blindtimer, calc_motor_time_up());
            motor_up();
            g_blind_state = moving_up;
        }
    } else if (g_blind_state != stopping){
        blind_stop();
    }
}

/**
 * Stop currently moving blind and update current position.
 */
void blind_stop             (void)
{
    uint16_t elapsed_ticks, total_ticks;
    int32_t  temp;

    motor_stop();
    if (g_blind_state != idle && g_blind_state != stopping) {
        g_blind_state = stopping;

        // blind has been stopped, before finishing movement.
        // calculate new position of the blind, depending on the
        // elapsed movement time.
        elapsed_ticks = clk_timer_get_elapsed_ticks(&g_blindtimer);
        // remove reaction delay time, when blinds are not moving
        if (elapsed_ticks > g_reaction_delay) {
            elapsed_ticks -= g_reaction_delay;
        } else {
            elapsed_ticks = 0;
        }

        // calculate new current position
        if (g_position_setpoint > g_current_position) {
            total_ticks = g_duration_open;
            temp = 100 * elapsed_ticks / total_ticks;
            if (temp + g_current_position > 100) g_current_position = 100;
            else g_current_position += temp;
        } else {
            total_ticks = g_duration_close;
            temp = 100 * elapsed_ticks / total_ticks;
            if (temp > g_current_position) g_current_position = 0;
            else g_current_position -= temp;
        }

        g_position_setpoint = g_current_position;
    }
}

/**
 * Process blind control statemachine. Call this in the main loop.
 */
void blind_background       (sBus_t* bus)
{
    switch (g_blind_state) {
    case idle:
        break;

    case moving_up:
        // fallthrough to moving_down
    case moving_down:
        if (clk_timer_is_elapsed(&g_blindtimer)) {
            motor_stop();
            g_current_position = g_position_setpoint;
            g_blind_state = stopping;
        }
        break;

    case stopping:
        if (!motor_is_running()) {
            g_blind_state = idle;
            send_current_position(bus);
            send_position_setpoint(bus);
        }
        break;
    }
}

uint8_t blind_get_current_position  (void)
{
    return g_current_position;
}

uint8_t blind_get_position_setpoint (void)
{
    return g_position_setpoint;
}

uint8_t blind_get_reaction_delay    (void)
{
    return g_reaction_delay;
}

void blind_set_reaction_delay       (uint8_t reaction_delay)
{
    g_reaction_delay = reaction_delay;
}


uint16_t blind_get_duration_open    (void)
{
    return g_duration_open;
}

void blind_set_duration_open        (uint16_t duration)
{
    g_duration_open = duration;
}

uint16_t blind_get_duration_close   (void)
{
    return g_duration_close;
}

void blind_set_duration_close       (uint16_t duration)
{
    g_duration_close = duration;
}

/** @} */
