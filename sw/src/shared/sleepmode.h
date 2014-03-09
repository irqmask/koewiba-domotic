/**
 * @addtogroup SLEEPMODE
 * @brief Public interface of sleepmode control.
 *
 * Sleepmode control.
 *
 * @{
 * @file    sleepmode.h
 * @brief   Public interface of sleepmode control.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _SLEEPMODE_H_
#define _SLEEPMODE_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"
#include "prjtypes.h"
#include "ucontroller.h"

#ifdef PRJCONF_UC_AVR
#include <avr/sleep.h>
#endif

// --- Definitions -------------------------------------------------------------

#ifdef PRJCONF_UC_AVR
#define SLEEP_vPinChange2_Enable()  PCICR |=  (1<<PCIE2)
#define SLEEP_vPinChange2_Disable() PCICR &= ~(1<<PCIE2)
#define SLEEP_vSetMode(m)           set_sleep_mode(m)
#define SLEEP_vActivate()           sleep_mode()
#define SLEEP_vDelayMS(t)           _delay_ms(t)
#else
#define SLEEP_vPinChange2_Enable()
#define SLEEP_vPinChange2_Disable()
#define SLEEP_vSetMode(m)
#define SLEEP_vActivate()
#define SLEEP_vDelayMS(t)
#endif
// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _SLEEPMODE_H_ */
/** @} */
