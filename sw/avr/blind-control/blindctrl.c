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
#include "cmddef_common.h"
#include "motor.h"
#include "timer.h"

// --- Definitions -------------------------------------------------------------

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#ifndef BLIND_APPCONFIG
 #define BLIND_APPCONFIG    1

 #define BLIND_COUNT         1
#endif
#if BLIND_COUNT > 8
#error "BLIND_COUNT too high, maximum number of eight blinds supported!"
#endif

// --- Type definitions --------------------------------------------------------

//! State which a blind can be in.
typedef enum {
    idle,           //!< The blind is idle.
    moving_up,      //!< The blind is moving up.
    moving_down,    //!< The blind is moving down.
    stopping        //!< The blind is stopping.
} blind_state_t;

//! Carries the operational data for each blind.
typedef struct {
    //! Saves the state of the blind control statemachine.
    blind_state_t   blind_state;
    //! Current position in percent 0% = open 100% = fully closed.
    uint8_t         current_position;
    //! Desired position in percent 0% = open 100% = fully closed.
    uint8_t         position_setpoint;
    //! Delay time in timer ticks (1/100sec) until movement starts after applying
    //! power.
    uint8_t         reaction_delay;
    //! Duration in timer ticks (1/100sec) to fully open the blind.
    uint16_t        duration_open;
    //! Duration in timer ticks (1/100sec) to fully close the blind.
    uint16_t        duration_close;
    //! Mode of how the blind / motor is controlled.
    uint8_t         mode;
    //! Timer to control movement of the blind.
    timer_data_t         blindtimer;
} blind_control_t;

// --- Local variables ---------------------------------------------------------

// local data which stores data of all blinds
static blind_control_t g_blind_control[BLIND_COUNT];
// indicator if blinds are currently moving.
static uint8_t g_blinds_active = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Calculate the movement time to move the blinds down, depending on the
 * position set-point and the current position.
 *
 * @returns Movement duration in timer ticks.
 */
static uint16_t calc_motor_time_down (uint8_t index)
{
    uint32_t down_duration;
    uint8_t  diff;

    if (g_blind_control[index].position_setpoint == 100) {
        down_duration = g_blind_control[index].duration_close;
    } else {
        diff = g_blind_control[index].position_setpoint - g_blind_control[index].current_position;
        down_duration = g_blind_control[index].duration_close;
        down_duration *= diff;
        down_duration /= 100;
    }

    down_duration += g_blind_control[index].reaction_delay;
    if (down_duration > 65535) down_duration = 65535;

    return (uint16_t)down_duration;
}

/**
 * Calculate the movement time to move the blinds up, depending on the
 * position set-point and the current position.
 *
 * @param[in]   index   Index of the blind.
 *
 * @returns Movement duration in timer ticks.
 */
static uint16_t calc_motor_time_up (uint8_t index)
{
    uint32_t up_duration;
    uint8_t  diff;

    if (g_blind_control[index].position_setpoint == 0) {
        up_duration = g_blind_control[index].duration_open;
    } else {
        diff = g_blind_control[index].current_position - g_blind_control[index].position_setpoint;
        up_duration = g_blind_control[index].duration_open;
        up_duration *= diff;
        up_duration /= 100;
    }

    up_duration += g_blind_control[index].reaction_delay;
    if (up_duration > 65535) up_duration = 65535;

    return (uint16_t)up_duration;
}

/**
 * Sends the current position of the blind as broadcast to the bus.
 *
 * @param[in]   index   Index of the blind.
 */
static void send_current_position (uint8_t index, sBus_t* bus)
{
    uint8_t msg[3];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_B0_PositionCurrent + APP_NUM_REGS_PER_BLIND * index;
    msg[2] = g_blind_control[index].current_position;
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
}

/**
 * Sends the current setpoint as broadcast to the bus.
 *
 * @param[in]   index   Index of the blind.
 */
static void send_position_setpoint (uint8_t index, sBus_t* bus)
{
    uint8_t msg[3];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_B0_PositionSetPoint + APP_NUM_REGS_PER_BLIND * index;
    msg[2] = g_blind_control[index].position_setpoint;
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Resets blind control state. Assuming blinds are closed 50% in the beginning.
 *
 * @param[in]   index   Index of the blind.
 */
void blind_reset            (uint8_t index)
{
    if (index >= BLIND_COUNT) return;
    g_blind_control[index].blind_state = idle;
    // current blind position is unknown. assume 50% so that driving in both
    // directions is still possible. 0% or 100% will apply each full up or down
    // time.
    g_blind_control[index].current_position = 50;
    g_blind_control[index].position_setpoint = 50;
}

/**
 * Move the blind to the position.
 *
 * @param[in]   index   Index of the blind.
 * @param[in]   position
 * Position to move to. 0 = completely open. 100 = completely closed.
 */
void blind_move_to_position (uint8_t index, uint8_t new_position)
{
    if (index >= BLIND_COUNT) return;
    if (new_position > 100) new_position = 100;
    if (g_blind_control[index].blind_state == idle && new_position != g_blind_control[index].current_position) {
        g_blind_control[index].position_setpoint = new_position;

        if (new_position > g_blind_control[index].current_position) {
            // move down
            timer_start(&g_blind_control[index].blindtimer, calc_motor_time_down(index));
            motor_down(index);
            g_blind_control[index].blind_state = moving_down;
            g_blinds_active |= (1<<index);
        } else if (new_position < g_blind_control[index].current_position) {
            // move up
            timer_start(&g_blind_control[index].blindtimer, calc_motor_time_up(index));
            motor_up(index);
            g_blind_control[index].blind_state = moving_up;
            g_blinds_active |= (1<<index);
        }
    } else if (g_blind_control[index].blind_state != stopping){
        blind_stop(index);
    }
}

/**
 * Stop currently moving blind and update current position.
 *
 * @param[in]   index   Index of the blind.
 */
void blind_stop             (uint8_t index)
{
    uint16_t elapsed_ticks, total_ticks;
    int32_t  temp;

    if (index >= BLIND_COUNT) return;
    motor_stop(index);
    if (g_blind_control[index].blind_state != idle && g_blind_control[index].blind_state != stopping) {
        g_blind_control[index].blind_state = stopping;

        // blind has been stopped, before finishing movement.
        // calculate new position of the blind, depending on the
        // elapsed movement time.
        elapsed_ticks = timer_get_elapsed_ticks(&g_blind_control[index].blindtimer);
        // remove reaction delay time, when blinds are not moving
        if (elapsed_ticks > g_blind_control[index].reaction_delay) {
            elapsed_ticks -= g_blind_control[index].reaction_delay;
        } else {
            elapsed_ticks = 0;
        }

        // calculate new current position
        if (g_blind_control[index].position_setpoint > g_blind_control[index].current_position) {
            total_ticks = g_blind_control[index].duration_close;
            temp = 100;
            temp *= elapsed_ticks;
            temp /= total_ticks;
            if (temp + (int32_t)g_blind_control[index].current_position > 100) g_blind_control[index].current_position = 100;
            else g_blind_control[index].current_position += temp;
        } else {
            total_ticks = g_blind_control[index].duration_open;
            temp = 100;
            temp *= elapsed_ticks;
            temp /= total_ticks;
            if (temp > (int32_t)g_blind_control[index].current_position) g_blind_control[index].current_position = 0;
            else g_blind_control[index].current_position -= temp;
        }

        g_blind_control[index].position_setpoint = g_blind_control[index].current_position;
    }
}

/**
 * Returns the current position of the blind.
 *
 * @param[in]   index   Index of the blind.
 *
 * @returns Current position.
 */
uint8_t blind_get_current_position  (uint8_t index)
{
    if (index >= BLIND_COUNT) return 0;
    return g_blind_control[index].current_position;
}

/**
 * Returns the current setpoint of the blind.
 *
 * @param[in]   index   Index of the blind.
 *
 * @returns Current setpoint.
 */
uint8_t blind_get_position_setpoint (uint8_t index)
{
    if (index >= BLIND_COUNT) return 0;
    return g_blind_control[index].position_setpoint;
}

/**
 * Returns the current set reaction delay of the blind.
 * The reaction delay is the time the blind-hardware needs to start moving when powered to move up/down.
 *
 * @param[in]   index   Index of the blind.
 *
 * @returns Reaction delay in 1/100 seconds
 */
uint8_t blind_get_reaction_delay    (uint8_t index)
{
    if (index >= BLIND_COUNT) return 0;
    return g_blind_control[index].reaction_delay;
}

/**
 * Sets the current reaction delay of the blind.
 * The reaction delay is the time the blind-hardware needs to start moving when powered to move up/down.
 *
 * @param[in]   index           Index of the blind.
 * @param[in]   reaction_delay  Reaction delay in 1/100 seconds
 */
void blind_set_reaction_delay       (uint8_t index, uint8_t reaction_delay)
{
    if (index >= BLIND_COUNT) return;
    g_blind_control[index].reaction_delay = reaction_delay;
}

/**
 * Get the duration which the blind needs for moving from completely closed to completely open.
 *
 * @param[in]   index           Index of the blind.
 *
 * @returns Duration in 1/100 seconds.
 */
uint16_t blind_get_duration_open    (uint8_t index)
{
    if (index >= BLIND_COUNT) return 0;
    return g_blind_control[index].duration_open;
}

/**
 * Set the duration which the blind needs for moving from completely closed to completely open.
 *
 * @param[in]   index           Index of the blind.
 * @param[in]   duration        Duration in 1/100 seconds.
 */
void blind_set_duration_open        (uint8_t index, uint16_t duration)
{
    if (index >= BLIND_COUNT) return;
    g_blind_control[index].duration_open = duration;
}

/**
 * Get the duration which the blind needs for moving from completely open to completely closed.
 *
 * @param[in]   index           Index of the blind.
 *
 * @returns Duration in 1/100 seconds.
 */
uint16_t blind_get_duration_close   (uint8_t index)
{
    if (index >= BLIND_COUNT) return 0;
    return g_blind_control[index].duration_close;
}

/**
 * Set the duration which the blind needs for moving from completely open to completely closed.
 *
 * @param[in]   index           Index of the blind.
 * @param[in]   duration        Duration in 1/100 seconds.
 */
void blind_set_duration_close       (uint8_t index, uint16_t duration)
{
    if (index >= BLIND_COUNT) return;
    g_blind_control[index].duration_close = duration;
}

/**
 * Set the mode, how the blind/motor is controlled.
 *
 * @param[in]   index           Index of the blind.
 * @param[in]   duration        Duration in 1/100 seconds.
 */
void blind_set_mode                 (uint8_t index, uint8_t mode)
{
    if (index >= BLIND_COUNT) return;
    g_blind_control[index].mode = mode;
}

/**
 * Initialize blind control data. Assuming blinds are closed (0) in the beginning.
 */
void blinds_initialize      (void)
{
    for (uint8_t index=0; index<BLIND_COUNT; index++) {
        g_blind_control[index].blind_state = idle;
        // current blind position is unknown. assume 50% so that driving in both
        // directions is still possible. 0% or 100% will apply each full up or down
        // time.
        g_blind_control[index].current_position = 50;
        g_blind_control[index].position_setpoint = 50;
        g_blind_control[index].reaction_delay = 0;
        g_blind_control[index].duration_open = TIMER_MS_2_TICKS(1000);
        g_blind_control[index].duration_close = TIMER_MS_2_TICKS(1000);
    }
    g_blinds_active = 0;
}

/**
 * Returns if blinds are moving.
 * @returns true if blinds are moving otherwise false.
 */
bool blinds_are_moving      (void)
{
    return (g_blinds_active != 0);
}

/**
 * Process blind control statemachine. Call this in the main loop.
 */
void blinds_background      (sBus_t* bus)
{
    for (uint8_t index=0; index<BLIND_COUNT; index++) {
        switch (g_blind_control[index].blind_state) {
        case idle:
            break;

        case moving_up:
            // fall-through to moving_down
        case moving_down:
            if (timer_is_elapsed(&g_blind_control[index].blindtimer)) {
                motor_stop(index);
                g_blind_control[index].current_position = g_blind_control[index].position_setpoint;
                g_blind_control[index].blind_state = stopping;
            }
            break;

        case stopping:
            if (!motor_is_running(index)) {
                g_blind_control[index].blind_state = idle;
                send_current_position(index, bus);
                send_position_setpoint(index, bus);
                g_blinds_active &= ~(1<<index);
            }
            break;
        }
    }
}
/** @} */
