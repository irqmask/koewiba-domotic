/**
 * @addtogroup ZAGWIRE
 * @brief Zagwire protocol decoder for TSIC temperature sensors.
 *
 * Supports pcbconfig.h for individual PCB settings of the Zagwire interface.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    zagwire.h
 * @brief   Zagwire protocol decoder for TSIC temperature sensors.
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
#ifndef _ZAGWIRE_H_
#define _ZAGWIRE_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "ucontroller.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection ZAGWIRE_PCBCONFIG
 * Configure ZAGWIRE Pinout.
 * @{
 */
#ifndef ZAGWIRE_PCBCONFIG
 #define ZAGWIRE_PCBCONFIG  1
 //! Defines the DDR of a port which contains the Zagwire enable signal.
 #define ZAGW_DDR_EN    DDRB
 //! Defines the DDR of a port which contains the Zagwire data signal.
 #define ZAGW_DDR_DATA  DDRB
 //! Defines the PORT which contains the Zagwire enable signal.
 #define ZAGW_PORT_EN   PORTB
 //! Defines the PORT which contains the Zagwire data signal.
 #define ZAGW_PORT_DATA PORTB
 //! Defines the input PORT which contains the Zagwire data signal.
 #define ZAGW_PIN_DATA  PINB
 //! Defines the Zagwire enable pin (output).
 #define ZAGW_EN        PB1
 //! Defines the Zagwire data pin (input).
 #define ZAGW_DATA      PB2
#endif // ZAGWIRE_PCBCONFIG
/** @} */

/**
 * @subsection ZAGWIRE_APPCONFIG
 * Configure ZAGWIRE protocol decoder. E.g. blocking or IRQ functions.
 * @{
 */
#ifndef ZAGWIRE_APPCONFIG
 #define ZAGWIRE_APPCONFIG 1
#endif // ZAGWIRE_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            zagw_initialize     (void);

void            zagw_enable         (bool enable);

bool            zagw_get_raw        (uint16_t* val);

uint16_t        zagw_get_temperature(uint16_t raw);


#endif // _ZAGWIRE_H_
/** @} */
