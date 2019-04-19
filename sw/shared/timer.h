/**
 * @addtogroup TIMER
 * @brief Public interface of system timer.
 *
 * This module contains a clock for the current time (if configured in
 * appconfig.h) and timer e.g. for timeout handling.
 *
 * @{
 * @file    timer.h
 * @brief   System clock.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _TIMER_H_
#define _TIMER_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

#define TIMER_COUNT             8

//! Convert from milliseconds into ticks.
#define TIMER_MS_2_TICKS(ms)    (uint16_t)((uint32_t)((uint32_t)ms*CLOCK_TICKS_PER_SECOND/1000))

// --- Type definitions --------------------------------------------------------

//! Time-out timer runtime-data.
typedef struct _timer_data {
    uint16_t          tick_start_value; //!< start value of time count down.
    volatile uint16_t ticks;	//!< tick count down.
    volatile bool     active;   //!< timer is running.
} timer_data_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void timer_initialize       (void);

void timer_sleep            (bool shall_sleep);

bool timer_start            (timer_data_t* timer_instance, uint16_t ticks);

bool timer_stop             (timer_data_t* timer_instance);

bool timer_is_elapsed       (timer_data_t* timer_instance);

bool timer_is_running       (timer_data_t* timer_instance);

uint16_t timer_get_initial_start_ticks (timer_data_t* timer_instance);

uint16_t timer_get_elapsed_ticks (timer_data_t* timer_instance);

uint16_t timers_next_expiration (void);

#endif /* _TIMER_H_ */
/** @} */
