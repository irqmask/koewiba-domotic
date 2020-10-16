/**
 * @addtogroup CLOCK
 * @{
 * @addtogroup CLOCK_OS
 * @{
 *
 * @brief System clock for linux based bus modules.
 * @file    timer_os.c
 * @brief   System clock for linux based bus modules.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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

#include "clock.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Timer runtime data structure
 * Stucture holding
 */
typedef struct clock {
    sys_time_t          starttime;
    clock_timer_t      *timer;
} local_clock_timer_t;

// --- Local variables ---------------------------------------------------------

//! List of running timers.
static local_clock_timer_t g_running_timers[CLOCK_NUM_TIMER];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Register timer in list.
 */
static bool register_timer(clock_timer_t *timer)
{
    uint8_t ii;

    for (ii = 0; ii < CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii].timer == NULL) {
            g_running_timers[ii].timer = timer;
            g_running_timers[ii].starttime = sys_time_get_usecs();
            return true;
        }
    }
    return false;
}

/**
 * Calculate elapsed ticks since start
 * @param[in]   starttime   Timestamp of beginning of time period to be measured
 * @return Ticks since starttime. On error 0.
 */
static uint16_t elapsed_ticks(sys_time_t starttime)
{
    sys_time_t diff, curr;

    curr = sys_time_get_usecs();
    if (curr < starttime) {
        return 0;
    }
    diff =  curr - starttime;
    diff /= 1000; // convert to milliseconds;
    diff /= (1000 / CLOCK_TICKS_PER_SECOND); // convert to ticks
    return (uint16_t) diff;
}

/**
 * Iterate through running timers list, check elapsed time.
 */
static void clean_up(void)
{
    uint8_t ii;

    for (ii = 0; ii < CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii].timer == NULL) {
            continue;
        }
        if (elapsed_ticks(g_running_timers[ii].starttime) >=
            g_running_timers[ii].timer->ticks) {
            g_running_timers[ii].timer->ticks = 0;
            g_running_timers[ii].timer = NULL;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize clock module. Reset data and start hardware timer.
 */
void clk_initialize(void)
{
    uint8_t ii;

    // clear running timers list
    for (ii = 0; ii < CLOCK_NUM_TIMER; ii++) {
        g_running_timers[ii].timer = NULL;
    }
}

/**
 * Start/Stop Clock-Timer
 *
 * @param[in]   start   Flag for starting/stopping the timer (true = start)
 */
void clk_control(bool start)
{
    // nothing to do in linux
}

/**
 * (Re)Start a count-down timer.
 *
 * @param[in]   timer   Pointer to timer structure.
 * @param[in]   ticks   Time in ticks. Convert from millisconds to ticks with
 * CLOCK_MS_2_TICKS macro.
 *
 * @returns true, if timer has been (re)started, otherwise false.
 */
bool clk_timer_start(clock_timer_t *timer, uint16_t ticks)
{
    // if timer is still running ...
    if (timer->ticks != 0) {
        timer->ticks = ticks; // ... restart timer
        return true;
    }
    timer->ticks = ticks;
    return register_timer(timer);
}

/**
 * Check if timer elapsed.
 *
 * @param[in]   timer   Pointer to timer structure.
 *
 * @returns true, if time is over, otherwise false.
 */
bool clk_timer_is_elapsed(clock_timer_t *timer)
{
    clean_up();
    if (timer->ticks == 0) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * Get next expiration in ticks from now on of all running timers
 *
 * @returns time difference in ticks from now.
 */
uint16_t clk_timers_next_expiration(void)
{
    uint16_t    diff = UINT16_MAX, new_diff, elapsed;
    uint8_t     ii;

    for (ii = 0; ii < CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii].timer == NULL) {
            continue;
        }

        elapsed = elapsed_ticks(g_running_timers[ii].starttime);
        if (elapsed > g_running_timers[ii].timer->ticks) {
            new_diff = 0;
        }
        else {
            new_diff = g_running_timers[ii].timer->ticks - elapsed;
        }
        if (new_diff < diff) {
            diff = new_diff;
        }
    }
    return diff;
}

/** @} */
/** @} */
