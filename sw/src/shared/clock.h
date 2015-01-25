/**
 * @addtogroup CLOCK
 * @brief Public interface of system clock.
 *
 * This module contains a clock for the current time (if configured in
 * appconfig.h) and timer e.g. for timeout handling.
 *
 * @{
 * @file    clock.h
 * @brief   System clock.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _CLOCK_H_
#define _CLOCK_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define CLOCK_NUM_TIMER         4

//! Convert from milliseconds into ticks
#define CLOCK_MS_2_TICKS(ms)    (uint16_t)((uint32_t)ms*CLOCK_TICKS_PER_SECOND/1000)

// --- Type definitions --------------------------------------------------------

//! Time-out timer runtime-data.
typedef struct clktimer {
    volatile uint16_t uTicks;  //!< tick count down.
} sClkTimer_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void clk_initialize        (void);

void clk_control           (BOOL start);

BOOL clk_timer_start       (sClkTimer_t* psTimer, uint16_t uTicks);

BOOL clk_timer_stop        (sClkTimer_t* psTimer);

BOOL clk_timer_is_elapsed  (sClkTimer_t* psTimer);

uint16_t clk_timers_next_expiration (void);

#endif /* _CLOCK_H_ */
/** @} */
