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
    motor_state_t motor_state;      //!< current state of the motor.
    motor_state_t last_motor_state; //!< last known state of the motor.
    timer_data_t       propagation_delay;//!< timer to handle switch delay.
} motor_control_t;

// --- Local variables ---------------------------------------------------------

//! storage of the state of the motors.
static motor_control_t g_motor_control[MOTOR_COUNT];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void motor_onoff_relay(uint8_t index, bool onoff)
{
    if (onoff == true) {
        switch (index) {
        case 0: MOTOR_0_ONOFF_PORT |= (1<<MOTOR_0_ONOFF_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_ONOFF_PORT |= (1<<MOTOR_1_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_ONOFF_PORT |= (1<<MOTOR_2_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_ONOFF_PORT |= (1<<MOTOR_3_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_ONOFF_PORT |= (1<<MOTOR_4_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_ONOFF_PORT |= (1<<MOTOR_5_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_ONOFF_PORT |= (1<<MOTOR_6_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_ONOFF_PORT |= (1<<MOTOR_7_ONOFF_PIN); break;
    #endif
        default:
            break;
        }
    } else {
        switch (index) {
        case 0: MOTOR_0_ONOFF_PORT &= ~(1<<MOTOR_0_ONOFF_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_ONOFF_PORT &= ~(1<<MOTOR_1_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_ONOFF_PORT &= ~(1<<MOTOR_2_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_ONOFF_PORT &= ~(1<<MOTOR_3_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_ONOFF_PORT &= ~(1<<MOTOR_4_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_ONOFF_PORT &= ~(1<<MOTOR_5_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_ONOFF_PORT &= ~(1<<MOTOR_6_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_ONOFF_PORT &= ~(1<<MOTOR_7_ONOFF_PIN); break;
    #endif
        default:
            break;
        }
    }
}

static void motor_updown_relay(uint8_t index, bool updown)
{
    if (updown == true) {
        switch (index) {
        case 0: MOTOR_0_UPDOWN_PORT |= (1<<MOTOR_0_UPDOWN_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_UPDOWN_PORT |= (1<<MOTOR_1_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_UPDOWN_PORT |= (1<<MOTOR_2_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_UPDOWN_PORT |= (1<<MOTOR_3_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_UPDOWN_PORT |= (1<<MOTOR_4_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_UPDOWN_PORT |= (1<<MOTOR_5_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_UPDOWN_PORT |= (1<<MOTOR_6_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_UPDOWN_PORT |= (1<<MOTOR_7_UPDOWN_PIN); break;
    #endif
        default:
            break;
        }
    } else {
        switch (index) {
        case 0: MOTOR_0_UPDOWN_PORT &= ~(1<<MOTOR_0_UPDOWN_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_UPDOWN_PORT &= ~(1<<MOTOR_1_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_UPDOWN_PORT &= ~(1<<MOTOR_2_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_UPDOWN_PORT &= ~(1<<MOTOR_3_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_UPDOWN_PORT &= ~(1<<MOTOR_4_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_UPDOWN_PORT &= ~(1<<MOTOR_5_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_UPDOWN_PORT &= ~(1<<MOTOR_6_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_UPDOWN_PORT &= ~(1<<MOTOR_7_UPDOWN_PIN); break;
    #endif
        default:
            break;
        }
    }
}

// --- Module global functions -------------------------------------------------

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
        motor_updown_relay(index, true);
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
        motor_updown_relay(index, false);
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
        motor_onoff_relay(index, false);
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
    for (uint8_t index=0; index<MOTOR_COUNT; index++) {
        switch (index) {
        case 0:
            MOTOR_0_ONOFF_DDR |= (1<<MOTOR_0_ONOFF_PIN);
            MOTOR_0_ONOFF_PORT &= ~(1<<MOTOR_0_ONOFF_PIN);
            MOTOR_0_UPDOWN_DDR |= (1<<MOTOR_0_UPDOWN_PIN);
            MOTOR_0_UPDOWN_PORT &= ~(1<<MOTOR_0_UPDOWN_PIN);
            break;
    #if MOTOR_COUNT > 1
        case 1:
            MOTOR_1_ONOFF_DDR |= (1<<MOTOR_1_ONOFF_PIN);
            MOTOR_1_ONOFF_PORT &= ~(1<<MOTOR_1_ONOFF_PIN);
            MOTOR_1_UPDOWN_DDR |= (1<<MOTOR_1_UPDOWN_PIN);
            MOTOR_1_UPDOWN_PORT &= ~(1<<MOTOR_1_UPDOWN_PIN);
           break;
    #endif
    #if MOTOR_COUNT > 2
        case 2:
            MOTOR_2_ONOFF_DDR |= (1<<MOTOR_2_ONOFF_PIN);
            MOTOR_2_ONOFF_PORT &= ~(1<<MOTOR_2_ONOFF_PIN);
            MOTOR_2_UPDOWN_DDR |= (1<<MOTOR_2_UPDOWN_PIN);
            MOTOR_2_UPDOWN_PORT &= ~(1<<MOTOR_2_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 3
        case 3:
            MOTOR_3_ONOFF_DDR |= (1<<MOTOR_3_ONOFF_PIN);
            MOTOR_3_ONOFF_PORT &= ~(1<<MOTOR_3_ONOFF_PIN);
            MOTOR_3_UPDOWN_DDR |= (1<<MOTOR_3_UPDOWN_PIN);
            MOTOR_3_UPDOWN_PORT &= ~(1<<MOTOR_3_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 4
        case 4:
            MOTOR_4_ONOFF_DDR |= (1<<MOTOR_4_ONOFF_PIN);
            MOTOR_4_ONOFF_PORT &= ~(1<<MOTOR_4_ONOFF_PIN);
            MOTOR_4_UPDOWN_DDR |= (1<<MOTOR_4_UPDOWN_PIN);
            MOTOR_4_UPDOWN_PORT &= ~(1<<MOTOR_4_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 5
        case 5:
            MOTOR_5_ONOFF_DDR |= (1<<MOTOR_5_ONOFF_PIN);
            MOTOR_5_ONOFF_PORT &= ~(1<<MOTOR_5_ONOFF_PIN);
            MOTOR_5_UPDOWN_DDR |= (1<<MOTOR_5_UPDOWN_PIN);
            MOTOR_5_UPDOWN_PORT &= ~(1<<MOTOR_5_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 6
        case 6:
            MOTOR_6_ONOFF_DDR |= (1<<MOTOR_6_ONOFF_PIN);
            MOTOR_6_ONOFF_PORT &= ~(1<<MOTOR_6_ONOFF_PIN);
            MOTOR_6_UPDOWN_DDR |= (1<<MOTOR_6_UPDOWN_PIN);
            MOTOR_6_UPDOWN_PORT &= ~(1<<MOTOR_6_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 7
        case 7:
            MOTOR_7_ONOFF_DDR |= (1<<MOTOR_7_ONOFF_PIN);
            MOTOR_7_ONOFF_PORT &= ~(1<<MOTOR_7_ONOFF_PIN);
            MOTOR_7_UPDOWN_DDR |= (1<<MOTOR_7_UPDOWN_PIN);
            MOTOR_7_UPDOWN_PORT &= ~(1<<MOTOR_7_UPDOWN_PIN);
            break;
    #endif
        default:
            break;
        }
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
                motor_onoff_relay(index, true);
            }
            break;

        case moving_up:
            break;

        case start_moving_down:
            if (timer_is_elapsed(&g_motor_control[index].propagation_delay)) {
                g_motor_control[index].motor_state = moving_down;
                motor_onoff_relay(index, true);
            }
            break;

        case moving_down:
            break;

        case stopping:
            if (timer_is_elapsed(&g_motor_control[index].propagation_delay)) {
                g_motor_control[index].motor_state = idle;
                motor_updown_relay(index, false);
            }
            break;

        default:
            break;
        }
    }
}
/** @} */
