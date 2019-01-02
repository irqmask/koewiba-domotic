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
/*
 * Copyright (C) 2019  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _SLEEPMODE_H_
#define _SLEEPMODE_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"
#include "prjtypes.h"

#ifdef PRJCONF_UC_AVR
#include "ucontroller.h"
#include <avr/sleep.h>
#endif

// --- Definitions -------------------------------------------------------------

#ifdef PRJCONF_UC_AVR

#ifdef __AVR_ATtiny1634__
 #define sleep_pinchange_enable()    GIMSK |=  (1<<PCIE2)
 #define sleep_pinchange_disable()   GIMSK &= ~(1<<PCIE2)
#else
 #define sleep_pinchange_enable()    PCICR |=  ((1<<PCIE2) | (1<<PCIE1) | (1<<PCIE0))
 #define sleep_pinchange_disable()   PCICR &= ~((1<<PCIE2) | (1<<PCIE1) | (1<<PCIE0))
#endif
// future use
//#define sleep_pinchange2_enable()   PINCHGIR_REGISTER |=  PCIE_BITS
//#define sleep_pinchange2_disable()  PINCHGIR_REGISTER &= ~PCIE_BITS
#define sleep_set_mode(m)           set_sleep_mode(m)
#define sleep_delay_ms(t)           _delay_ms(t)
#else
#define sleep_pinchange_enable()
#define sleep_pinchange_disable()
#define sleep_set_mode(m)
#define sleep_activate()
#define sleep_delay_ms(t)
#define sleep_prevent(a,b)
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------


// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifdef PRJCONF_UC_AVR

void sleep_activate(void);

void sleep_prevent(uint8_t prevent_mask, uint8_t on);

#endif

#endif /* _SLEEPMODE_H_ */
/** @} */
