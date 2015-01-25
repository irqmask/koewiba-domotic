/**
 * @addtogroup CLOCK
 * @brief System clock.
 *
 * This module contains a clock for the current time (if configured in
 * appconfig.h) and timer e.g. for timeout handling.
 *
 * @{
 * @file    clock.c
 * @brief   System clock.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include "prjtypes.h"
#include "clock.h"
#include "ucontroller.h"

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
static BOOL register_timer(sClkTimer_t* psTimer)
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
 * Remove timer from list.
 */
static BOOL remove_timer(sClkTimer_t* psTimer)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii] = psTimer) {
            g_asRunningTimers[ii] == NULL;
            return TRUE;
        }
    }
    return FALSE;
}


/**
 * Timer interrupt. Increase clock and iterate through running timers list.
 */
ISR(INTERRUPT_TIMER0_COMPA)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii] == NULL) continue;
        if (--g_asRunningTimers[ii]->uTicks == 0) {
            g_asRunningTimers[ii] = NULL;
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
    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        g_asRunningTimers[ii] = NULL;
    }

    // initialize timer interrupt every 1/100th second (fOc = 50Hz)
    //                      F_CPU          7372800Hz
    // fIrq = 2 * fOC  = ------------ = --------------- = 100Hz
    //                    N * (OC+1)     1024 * (71+1)
    //
    REG_TIMER0_OCRA = 141; // 71
    // No output waveform generation, CTC mode,
    // select prescaler 1024 CS2..0 = 0b101
    REG_TIMER0_TCCRA = (1<<REGBIT_TIMER0_WGM1);
    REG_TIMER0_TCCRB = (1<<REGBIT_TIMER0_CS2 | 1<<REGBIT_TIMER0_CS0);

    // enable OutputCompareMatchA interrupt
    REG_TIMER0_IRQMSK |= (1<<REGBIT_TIMER0_OCIEA);
}

/**
 * Start/Stop Clock-Timer
 *
 * @param[in] start
 * boolean for starting/stopping the timer (TRUE = start)
 */
void clk_control(BOOL bStart)
{
    static uint8_t tccr1b = 0;

    if (bStart) {
        if (0==tccr1b) return;
        REG_TIMER0_TCCRB = tccr1b;
    } else {
        tccr1b = REG_TIMER0_TCCRB;
        REG_TIMER0_TCCRB &= ~((1<<REGBIT_TIMER0_CS2) | (1<<REGBIT_TIMER0_CS1) | (1<<REGBIT_TIMER0_CS0));
    }
}

/**
 * (Re)Start a count-down timer.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 * @param[in] uTime
 * Time in ticks. Convert from milliseconds to ticks with
 * CLOCK_MS_2_TICKS macro.
 *
 * @returns TRUE, if timer has been (re)started, otherwise FALSE.
 */
BOOL clk_timer_start(sClkTimer_t* psTimer, uint16_t uTicks)
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
 * Stop a count-down timer.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if timer has been successfully removed from runningtimer-list, otherwise FALSE.
 */
BOOL clk_timer_stop(sClkTimer_t* psTimer)
{
    // if timer could not be removed successfully
    if( !remove_timer(psTimer) ) return FALSE;
    // otherwise reset value
    psTimer->uTicks = 0;
    return TRUE;
}

/**
 * Check if timer elapsed.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if time is over, otherwise false.
 */
BOOL clk_timer_is_elapsed(sClkTimer_t* psTimer)
{
    if (psTimer->uTicks == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/** @} */
