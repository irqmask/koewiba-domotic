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
#define sleep_pinchange2_enable()   PCICR |=  (1<<PCIE2)
#define sleep_pinchange2_disable()  PCICR &= ~(1<<PCIE2)
#define sleep_set_mode(m)           set_sleep_mode(m)
#define sleep_activate()            sleep_mode()
#define sleep_delay_ms(t)           _delay_ms(t)
#else
#define sleep_pinchange2_enable()
#define sleep_pinchange2_disable()
#define sleep_set_mode(m)
#define sleep_activate()
#define sleep_delay_ms(t)
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
