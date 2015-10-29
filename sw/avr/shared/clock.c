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

volatile clock_timer_t* g_running_timers[CLOCK_NUM_TIMER];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Register timer in list.
 */
static BOOL register_timer(clock_timer_t* timer_instance)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii] == NULL) {
        	g_running_timers[ii] = timer_instance;
        	timer_instance->active = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Remove timer from list.
 */
static BOOL remove_timer(clock_timer_t* timer_instance)
{
    uint8_t ii;

    timer_instance->active = FALSE;
    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_running_timers[ii] == timer_instance) {
        	g_running_timers[ii] = NULL;
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
        if (g_running_timers[ii] == NULL) continue;
        if (--g_running_timers[ii]->ticks == 0) {
        	g_running_timers[ii] = NULL;
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
    	g_running_timers[ii] = NULL;
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
void clk_control(BOOL start)
{
    static uint8_t tccr1b = 0;

    if (start) {
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
BOOL clk_timer_start(clock_timer_t* timer_instance, uint16_t ticks)
{
    // if timer is still running ...
    if (timer_instance->ticks != 0) {
    	timer_instance->ticks = ticks; // ... restart timer
        return TRUE;
    }
    timer_instance->ticks = ticks;
    return register_timer(timer_instance);
}

/**
 * Stop a count-down timer.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if timer has been successfully removed from runningtimer-list, otherwise FALSE.
 */
BOOL clk_timer_stop(clock_timer_t* timer_instance)
{
    // if timer could not be removed successfully
    if( !remove_timer(timer_instance) ) return FALSE;
    // otherwise reset value
    timer_instance->ticks = 0;
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
BOOL clk_timer_is_elapsed(clock_timer_t* timer_instance)
{
    if (timer_instance->ticks == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}


/**
 * Check if timer is running.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if timer is running, otherwise false.
 */
BOOL clk_timer_is_running(clock_timer_t* timer_instance)
{
    return (timer_instance->active);
};

/** @} */