/**
 * @addtogroup CONTROL_TEMP
 *
 * @{
 * @file    control_temp.c
 * @brief   Module to control the heatervalve depending on current and desired
 *          temperature.
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

#include "control_temp.h"

#include <stdint.h>
#include <stdio.h>
#include "STM8L052C6.h"

#include "motor.h"
#include "remote_tempsense.h"
#include "timer.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint32_t g_start;
static uint8_t g_second_count;

static uint16_t g_kp = 256;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void background_10s(void)
{
    uint16_t des_temp_c, cur_temp_c;
    int16_t temp_diff;
    uint32_t valvepos;
    int16_t diff_valvepos;

    des_temp_c = remts_get_desired_temp();
    cur_temp_c = remts_get_current_temp();
    temp_diff = des_temp_c - cur_temp_c;
    printf("# dt %4d ct %4d d %d\n", des_temp_c - 27315, cur_temp_c - 27315, temp_diff);
    if (temp_diff < 0) {
        valvepos = 0;
    }
    else {
        // max diff
        valvepos = temp_diff;
        valvepos *= g_kp;
        valvepos >>= 8;

        if (valvepos > 1000) {
            valvepos = 1000;
        }
    }
    printf("# vp %d\n", (uint16_t)valvepos);
    // valvepos is between 0 and 1000
    diff_valvepos = valvepos - motor_get_valve_pos();
    if (diff_valvepos < -100 || diff_valvepos > 100 || g_second_count > 60) {
        if (motor_start_move_pos(valvepos)) {
            g_second_count = 0;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void ctrl_temp_initialize(void)
{
    g_start = timer_get_millis();
}


void ctrl_temp_background(void)
{
    uint32_t elapsed = timer_get_millis() - g_start;
    if (elapsed > 10000) {
        g_start += 10000;
        background_10s();
    }
}

/** @} */
