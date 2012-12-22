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

#define CLOCK_NUM_TIMER        8
#define CLOCK_TICKS_PER_SECOND 10

// --- Type definitions --------------------------------------------------------

//! Time-out timer runtime-data.
typedef struct clktimer {
    uint8_t     uIndex;         //!< index in clock table
    volatile uint8_t uTicks;  //!< tick count down.
} sClkTimer_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void CLK_vInitialize        (void);

BOOL CLK_bTimerStart        (sClkTimer_t* psTimer, uint16_t uTime);

BOOL CLK_bTimerIsElapsed    (sClkTimer_t* psTimer);

#endif /* _CLOCK_H_ */
/** @} */
