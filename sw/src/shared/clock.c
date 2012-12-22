/**
 * @addtogroup CLOCK
 * @brief System clock.
 *
 * @{
 * @file    clock.c
 * @brief   System clock.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include "prjtypes.h"
#include "clock.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

volatile sClkTimer_t* g_asRunningTimers[CLOCK_NUM_TIMER];

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
        if (g_asRunningTimers[ii] == NULL) {
            g_asRunningTimers[ii] = psTimer;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Timer interrupt. Increase clock and iterate through running timers list.
 */
ISR(TIMER1_COMPA_vect)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii] != NULL) {
            if (g_asRunningTimers[ii]->uTicks <= 1) {
                // time elapsed, de-register timer from list
                g_asRunningTimers[ii]->uTicks = 0;
                g_asRunningTimers[ii] = NULL;
            } else {
                g_asRunningTimers[ii]->uTicks--;
            }
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
        g_asRunningTimers[ii] = NULL;
    }

    // initialize timer interrupt every 10th second (fOc = 10)
    //                     fClk
    // fIrq = 2 * fOC  = --------- - 1
    //                    N*fOC
    TCCR1B |= ((0<<CS12) | (1<<CS11) | (1<<CS10)); // set prescaler to 1/64 (N=64)
    TCCR1B |= (1<<WGM12);                          // CTC mode (clear timer on compare match)
    OCR1AH = (F_CPU/(64*CLOCK_TICKS_PER_SECOND) - 1) >> 8;
    OCR1AL = (F_CPU/(64*CLOCK_TICKS_PER_SECOND) - 1) & 0x00FF;
    TIMSK1 |= (1<<OCIE1A);                          // enable output compare interrupt
}

/**
 * Start a new count-down timer.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 * @param[in] uTime
 * Time in milliseconds. Real timer resolution is defined by CLOCK_TICKS_PER_SECOND.
 *
 * @returns TRUE, if timer has been started, otherwise FALSE.
 */
BOOL CLK_bTimerStart (sClkTimer_t* psTimer, uint16_t uTime)
{
    uint32_t tick;

    // check if timer is still running
    if (psTimer->uTicks != 0) {
        return FALSE;
    }

    // calculate ticks (from milliseconds)
    tick = (uTime * CLOCK_TICKS_PER_SECOND);
    tick /= 1000;
    if (tick == 0) tick = 1;

    psTimer->uTicks = (uint16_t)tick;
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
BOOL CLK_bTimerIsElapsed    (sClkTimer_t* psTimer)
{
    if (psTimer->uTicks == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/** @} */
