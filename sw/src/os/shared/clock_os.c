/**
 * @addtogroup CLOCK
 * @addtogroup CLOCK_LINUX
 * @brief System clock for linux based bus modules.
 *
 * @{
 * @file    clock_linux.c
 * @brief   System clock for linux based bus modules.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "clock.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct clock {
    sys_time_t          starttime;
    sClkTimer_t*        timer;
} local_clock_timer_t;

// --- Local variables ---------------------------------------------------------

static local_clock_timer_t g_running_timers[CLOCK_NUM_TIMER];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Register timer in list.
 */
static BOOL register_timer (sClkTimer_t* psTimer)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii].timer == NULL) {
            g_running_timers[ii].timer = psTimer;
            g_running_timers[ii].starttime = sys_time_get_usecs();
            return TRUE;
        }
    }
    return FALSE;
}

static uint16_t elapsed_ticks (sys_time_t starttime)
{
    sys_time_t diff, curr;

    curr = sys_time_get_usecs();
    if (curr < starttime) return 0;
    diff =  curr - starttime;
    diff /= 1000; // convert to milliseconds;
    diff /= (1000 / CLOCK_TICKS_PER_SECOND); // convert to ticks
    return (uint16_t) diff;
}

/**
 * Iterate through running timers list, check elapsed time.
 */
static void clean_up (void)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii].timer == NULL) continue;
        if (elapsed_ticks(g_running_timers[ii].starttime) >=
            g_running_timers[ii].timer->uTicks) {
            g_running_timers[ii].timer->uTicks = 0;
            g_running_timers[ii].timer = NULL;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize clock module. Reset data and start hardware timer.
 */
void clk_initialize (void)
{
    uint8_t ii;

    // clear running timers list
    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        g_running_timers[ii].timer = NULL;
    }
}

/**
 * Start/Stop Clock-Timer
 *
 * @param[in] start
 * boolean for starting/stopping the timer (TRUE = start)
 */
void clk_control (BOOL start)
{
    // nothing to do in linux
}

/**
 * (Re)Start a count-down timer.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 * @param[in] uTime
 * Time in ticks. Convert from millisconds to ticks with
 * CLOCK_MS_2_TICKS macro.
 *
 * @returns TRUE, if timer has been (re)started, otherwise FALSE.
 */
BOOL clk_timer_start (sClkTimer_t* psTimer, uint16_t uTicks)
{
    // if timer is still running ...
    if (psTimer->uTicks != 0) {
        psTimer->uTicks = uTicks; // ... restart timer
        return TRUE;
    }
    psTimer->uTicks = uTicks;
    return register_timer(psTimer);
}

/**
 * Check if timer elapsed.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if time is over, otherwise false.
 */
BOOL clk_timer_is_elapsed (sClkTimer_t* psTimer)
{
    clean_up();
    if (psTimer->uTicks == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Get next expiration in ticks from now on of all running timers
 *
 * @returns time difference in ticks from now.
 */
uint16_t clk_timers_next_expiration (void)
{
    uint16_t    diff = UINT16_MAX, new_diff, elapsed;
    uint8_t     ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii].timer == NULL) continue;

        elapsed = elapsed_ticks(g_running_timers[ii].starttime);
        if (elapsed > g_running_timers[ii].timer->uTicks) {
            new_diff = 0;
        } else {
            new_diff = g_running_timers[ii].timer->uTicks - elapsed;
        }
        if (new_diff < diff) diff = new_diff;
    }
    return diff;
}


/** @} */
