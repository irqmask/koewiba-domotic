/**
 * @addtogroup ZAGWIRE
 * @brief Zagwire protocol decoder for TSIC temperature sensors.
 *
 * Contains functions to send and receive data over the SPI interface. Two
 * versions exist: A blocking version which halts the application until the data
 * is transmitted and a non-blocking version, which does the transmission in
 * background. In this case optional start and end-callbacks are called at the
 * beginning and at the end of the transmission.
 *
 * Only use the non-blocking option if the CPU frequency is at least times 10
 * faster than the SPI frequency. Otherwise the control overhead takes more CPU
 * time than the transmission itself.
 *
 * Supports pcbconfig.h for individual PCB settings of the SPI pinout.
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

//! Zagwire status bits.
typedef enum zagw_status {
    ZAGW_eNewValue = 0,
} ZAGW_eStatus;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            zagw_initialize    (void);

void            zagw_start_reception(void);

uint8_t         zagw_receive        (void);

uint16_t        zagw_get_bits       (void);

uint16_t        zagw_get_temperature(void);

#endif // _ZAGWIRE_H_
/** @} */
