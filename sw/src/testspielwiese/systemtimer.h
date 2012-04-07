/**
 * @addtogroup TODO_MODULE_NAME_TODO
 * @brief TODO describe.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    systemtimer.h
 * @brief   Declaration of MIDI receive/send routines.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _SYSTEMTIMER_H__
#define _SYSTEMTIMER_H__

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

uint16_t    SYSTIME_GetTickCount    ();

void        SYSTIME_SetTime         (uint8_t        uHour,
                                     uint8_t        uMinute,
                                     uint8_t        uSecond);
void        SYSTIME_GetTime         (uint8_t*       puHour,
                                     uint8_t*       puMinute,
                                     uint8_t*       puSecond);

#endif /* _SYSTEMTIMER_H__ */
/** @} */
