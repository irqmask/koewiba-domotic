/**
 * @addtogroup STM8_LCD
 *
 * @{
 * @file    lcd.c
 * @brief   A module to interface STM8 LCD display.
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

#include "motor.h"

#include <stdio.h>

// include
#include "stm8l052c6.h"

#include "adc.h"
#include "debug.h"
#include "timer.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

#define MOTOR_E7 (1 << 7)
#define MOTOR_E6 (1 << 6)
#define MOTOR_C4 (1 << 4)
#define MOTOR_C7 (1 << 7)
#define MOTOR_ADC 5


#define MOTOR_DUR_AWAY_FROM_HOME     5000   //!< duration in ms to drive away
                                            //!< from home during initialization
#define MOTOR_DUR_SEEK_HOME         30000   //!< duration in ms to seek towards
                                            //!< home during initialization

#define MOTOR_DUR_FULL_OPEN         29000   //!< duration to fully open valve

#define MOTOR_ADC_THD_BLK_LOW        1500   //!< Threshold to detect motor block
#define MOTOR_ADC_THD_BLK_HIGH       2036   //!< Threshold to detect motor block

// valve's opening time without mechanical load 29s.
// valve's closing time without mechanical load 29s.
// motor home is the complete open position of the valve.
// valve's pin is completely out, this devices pin is completely in.

// --- Type definitions --------------------------------------------------------


typedef enum
{
    MOTOR_IDLE,
    MOTOR_START_AWAY_FROM_HOME,
    MOTOR_MOVING_AWAY_FROM_HOME,
    MOTOR_START_SEEK_HOME,
    MOTOR_MOVING_TOWARDS_HOME,
    MOTOR_MOVING_TO_POS
} motor_states_t;

// --- Local variables ---------------------------------------------------------

static motor_states_t g_state;
static uint16_t g_timer;
static uint32_t g_start;
static uint16_t g_pos_desired;
static uint16_t g_pos;
static uint16_t g_last_reads[4];
static uint16_t g_last_read_sum;


// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static uint16_t filter_motor_val(uint16_t val)
{
    static uint8_t idx = 0;

    g_last_read_sum -= g_last_reads[idx];
    g_last_reads[idx] = val;
    g_last_read_sum +=  val;

    idx++;
    if (idx > 3) idx = 0;

    return g_last_read_sum >> 2; // div 8
}


static bool motor_block_detected(void)
{
    uint16_t mv;
    if (!adc_conversion_ready(MOTOR_ADC)) {
        return false;
    }
    mv = adc_read();
    mv = filter_motor_val(mv);
    printf("mot %d\n", mv);
    if (mv > MOTOR_ADC_THD_BLK_LOW && mv < MOTOR_ADC_THD_BLK_HIGH) {
        LOG_DEBUG("# BLOCKED!\n");
        return true;
    }
    return false;
}
// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize motor hardware.
 */
void motor_initialize(void)
{
    // 1. H-Bridge
    PE_DDR |= MOTOR_E7 | MOTOR_E6;
    PE_CR1 |= MOTOR_E7 | MOTOR_E6;
    PE_ODR |= MOTOR_E7 | MOTOR_E6;

    // 2. H-Bridge
    PC_DDR |= MOTOR_C4 | MOTOR_C7;
    PC_CR1 |= MOTOR_C4 | MOTOR_C7;
    PC_ODR |= MOTOR_C4 | MOTOR_C7;

    g_state = MOTOR_IDLE;
    g_timer = 0;
    g_pos = 0;
}


void motor_set(bool on, bool dir_close)
{
    if (on) {
        if (dir_close) {
            PC_ODR |= MOTOR_C4 | MOTOR_C7; // 0V
            PE_ODR &= ~MOTOR_E7 & ~MOTOR_E6; // 3.3V

        }
        else {
            PE_ODR |= MOTOR_E7 | MOTOR_E6; // 0V
            PC_ODR &= ~MOTOR_C4 & ~MOTOR_C7; // 3.3V
        }
    }
    else {
        PE_ODR |= MOTOR_E7 | MOTOR_E6; // 0V
        PC_ODR |= MOTOR_C4 | MOTOR_C7; // 0V
    }
}


uint16_t motor_get_valve_pos(void)
{
    return g_pos;
}


bool motor_start_homing(void)
{
    if (g_state != MOTOR_IDLE) {
        return false;
    }
    g_state = MOTOR_START_AWAY_FROM_HOME;
    g_pos = 0;
    g_pos_desired = 0;
    return true;
}


/**
 * start to move motor to position.
 * @param[in]   pos     0...1000, where 0 means valve is fully closed
 *                      1000 means valve is fully open.
 * @returns true, if action was started, otherwise false.
 */
bool motor_start_move_pos(uint16_t pos)
{
    uint16_t diff;
    uint32_t time = 0;

    if (g_state != MOTOR_IDLE) {
        return false;
    }

    g_pos_desired = pos;
    if (pos > g_pos) {
        // move towards open position
        diff = pos - g_pos;
        time = (uint32_t)diff * MOTOR_DUR_FULL_OPEN / 1000;
        motor_set(true, false);
    }
    else if (g_pos > pos) {
        // move towards close position
        diff = g_pos - pos;
        time = (uint32_t)diff * MOTOR_DUR_FULL_OPEN / 1000;
        motor_set(true, true);
    }
    g_start = timer_get_millis();
    g_timer = (uint16_t)time;
    g_state = MOTOR_MOVING_TO_POS;

    return true;
}


void motor_background(void)
{
    uint16_t elapsed;

    switch (g_state) {
    case MOTOR_START_AWAY_FROM_HOME:
        g_timer = MOTOR_DUR_AWAY_FROM_HOME;
        g_state = MOTOR_MOVING_AWAY_FROM_HOME;
        g_start = timer_get_millis();
        motor_set(true, true);
        break;

    case MOTOR_MOVING_AWAY_FROM_HOME:
        elapsed = timer_get_millis() - g_start;
        if (elapsed >= g_timer || (motor_block_detected() && elapsed > 500)) {
            motor_set(true, false);
            g_start = timer_get_millis();
            g_timer = MOTOR_DUR_SEEK_HOME;
            g_state = MOTOR_MOVING_TOWARDS_HOME;
        }
        else {
            adc_start(MOTOR_ADC);
        }
        break;

    case MOTOR_MOVING_TOWARDS_HOME:
        elapsed = timer_get_millis() - g_start;
        if (elapsed >= g_timer || (motor_block_detected() && elapsed > 500)) {
            motor_set(false, false);
            g_timer = 0;
            g_state = MOTOR_IDLE;
            g_pos = 1000; // valve is fully open now
            g_pos_desired = 1000;
            printf("mp %04x\n", g_pos);
        }
        else {
            adc_start(MOTOR_ADC);
        }
        break;

    case MOTOR_MOVING_TO_POS:
        elapsed = timer_get_millis() - g_start;
        if (elapsed >= g_timer) {
            motor_set(false, false);
            g_pos = g_pos_desired;
            g_timer = 0;
            g_state = MOTOR_IDLE;
            printf("mp %04x\n", g_pos);
        }
        break;

    default:
        break;
    }
}

/** @} */
