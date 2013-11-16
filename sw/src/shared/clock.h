/**
 * @addtogroup CLOCK
 * @brief Public interface of system clock.
 *
 * TODO: Detailed description of module.
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
#define CLOCK_TICKS_PER_SECOND  100

//! Convert from milliseconds into ticks
#define CLOCK_MS_2_TICKS(ms)    (uint8_t)((uint32_t)ms*CLOCK_TICKS_PER_SECOND/1000)

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

void CLK_vInitialize        (void);

void CLK_vControl			(BOOL start);

BOOL CLK_bTimerStart        (sClkTimer_t* psTimer, uint16_t uTicks);

BOOL CLK_bTimerIsElapsed    (sClkTimer_t* psTimer);

#endif /* _CLOCK_H_ */
/** @} */
