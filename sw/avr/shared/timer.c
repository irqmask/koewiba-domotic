/**
 * @addtogroup TIMER
 * @brief System timer.
 *
 * This module contains a timer e.g. for timeout handling.
 *
 * @{
 * @file    timer.c
 * @brief   System timer.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include "prjtypes.h"
#include "timer.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

//! List of current running timers.
volatile timer_data_t* g_running_timers[TIMER_COUNT];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Register timer in list.
 */
static bool register_timer(timer_data_t* timer_instance)
{
    uint8_t ii;

    for (ii=0; ii<TIMER_COUNT; ii++) {
        if (g_running_timers[ii] == NULL) {
        	g_running_timers[ii] = timer_instance;
        	timer_instance->active = true;
            return true;
        }
    }
    return false;
}

/**
 * Remove timer from list.
 */
static bool remove_timer(timer_data_t* timer_instance)
{
    uint8_t ii;

    timer_instance->active = false;
    for (ii=0; ii<TIMER_COUNT; ii++) {
        if (g_running_timers[ii] == timer_instance) {
        	g_running_timers[ii] = NULL;
            return true;
        }
    }
    return false;
}


/**
 * Timer interrupt. Increase clock and iterate through running timers list.
 */
ISR(INTERRUPT_TIMER0_COMPA)
{
    uint8_t ii;

    for (ii=0; ii<TIMER_COUNT; ii++) {
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
void timer_initialize(void)
{
    uint8_t ii;

    // clear running timers list
    for (ii=0; ii<TIMER_COUNT; ii++) {
    	g_running_timers[ii] = NULL;
    }

    // initialize timer interrupt every 1/100th second (fOc = 50Hz)
    //                      F_CPU          7372800Hz
    // fIrq = 2 * fOC  = ------------ = --------------- = 100Hz
    //                    N * (OC+1)     1024 * (71+1)
    //
    REG_TIMER0_OCRA = 71;
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
 * boolean for starting/stopping the timer (true = start)
 */
void timer_control(bool start)
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
 * @param[in] timer_instance
 * Pointer to timer structure.
 * @param[in] ticks
 * Time in ticks. Convert from milliseconds to ticks with
 * CLOCK_MS_2_TICKS macro.
 *
 * @note If timer increases every 100th second, the maximum timer time is
 *       65535/100 = 655,35sec = 10:55,350.
 *
 * @returns true, if timer has been (re)started, otherwise FALSE.
 */
bool timer_start(timer_data_t* timer_instance, uint16_t ticks)
{
    // if timer is still running ...
    if (timer_instance->ticks != 0) {
    	timer_instance->tick_start_value = ticks;
        timer_instance->ticks = ticks; // ... restart timer
        return true;
    }
    timer_instance->tick_start_value = ticks;
    timer_instance->ticks = ticks;
    return register_timer(timer_instance);
}

/**
 * Stop a count-down timer.
 *
 * @param[in] timer_instance
 * Pointer to timer structure.
 *
 * @returns true, if timer has been successfully removed from running timer-list, otherwise false.
 */
bool timer_stop(timer_data_t* timer_instance)
{
    // if timer could not be removed successfully
    if( !remove_timer(timer_instance) ) return false;
    // otherwise reset value
    timer_instance->ticks = 0;
    return true;
}

/**
 * Check if timer elapsed.
 *
 * @param[in] timer_instance
 * Pointer to timer structure.
 *
 * @returns true, if time is over, otherwise false.
 */
bool timer_is_elapsed(timer_data_t* timer_instance)
{
    if (timer_instance->ticks == 0) {
        return true;
    } else {
        return false;
    }
}


/**
 * Check if timer is running.
 *
 * @param[in] timer_instance
 * Pointer to timer structure.
 *
 * @returns true, if timer is running, otherwise false.
 */
bool timer_is_running(timer_data_t* timer_instance)
{
    return (timer_instance->active);
}

/**
 * Check if timer is running.
 *
 * @param[in] timer_instance
 * Pointer to timer structure.
 *
 * @returns Returns Initial value with which the timer was started.
 */
uint16_t timer_get_initial_start_ticks(timer_data_t* timer_instance)
{
    return timer_instance->tick_start_value;
}

/**
 * Check if timer is running.
 *
 * @param[in] timer_instance
 * Pointer to timer structure.
 *
 * @returns Returns the elapsed timer ticks since timer start.
 */
uint16_t timer_get_elapsed_ticks(timer_data_t* timer_instance)
{
    return (timer_instance->tick_start_value - timer_instance->ticks);
}

/** @} */
