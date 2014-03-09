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
    uSysTime_t          uStarttime;
    sClkTimer_t*        psTimer;
} sLocalClkTimer_t;

// --- Local variables ---------------------------------------------------------

sLocalClkTimer_t g_asRunningTimers[CLOCK_NUM_TIMER];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Register timer in list.
 */
BOOL bRegisterTimer(sClkTimer_t* psTimer)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii].psTimer == NULL) {
            g_asRunningTimers[ii].psTimer = psTimer;
            g_asRunningTimers[ii].uStarttime = SYS_uGetTimeUSecs();
            return TRUE;
        }
    }
    return FALSE;
}

uint16_t uElapsedTicks(uSysTime_t uStarttime)
{
    uSysTime_t diff, curr;

    curr = SYS_uGetTimeUSecs();
    if (curr < uStarttime) return 0;
    diff =  curr - uStarttime;
    diff /= 1000; //convert to milliseconds;
    diff /= (1000 / CLOCK_TICKS_PER_SECOND); // convert to ticks
    return (uint16_t) diff;
}

/**
 * Iterate through running timers list, chck elapsed time.
 */
void vCleanUp(void)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii].psTimer == NULL) continue;
        if (uElapsedTicks(g_asRunningTimers[ii].uStarttime) >= 
            g_asRunningTimers[ii].psTimer->uTicks) {
            g_asRunningTimers[ii].psTimer->uTicks = 0;
            g_asRunningTimers[ii].psTimer = NULL;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize clock module. Reset data and start hardware timer.
 */
void CLK_vInitialize(void)
{
    uint8_t ii;

    // clear running timers list
    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        g_asRunningTimers[ii].psTimer = NULL;
    }
}

/**
 * Start/Stop Clock-Timer
 * 
 * @param[in] start
 * boolean for starting/stopping the timer (TRUE = start)
 */
void CLK_vControl(BOOL start)
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
BOOL CLK_bTimerStart(sClkTimer_t* psTimer, uint16_t uTicks)
{
    // if timer is still running ...
    if (psTimer->uTicks != 0) {
        psTimer->uTicks = uTicks; // ... restart timer
        return TRUE;
    }
    psTimer->uTicks = uTicks;
    return bRegisterTimer(psTimer);
}

/**
 * Check if timer elapsed.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if time is over, otherwise false.
 */
BOOL CLK_bTimerIsElapsed(sClkTimer_t* psTimer)
{
    vCleanUp();
    if (psTimer->uTicks == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/** @} */
