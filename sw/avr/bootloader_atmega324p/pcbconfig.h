/**
 * @addtogroup BOOTLOADER_ATMEGA324_PCBCONFIG
 * @brief PCB configuration of the "bootloader_atmega324" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "bootloader_atmega324" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// bus-module_atmega328 specific pin assignments
// ----------------------------------------------------------------------------

#define SPI_PCBCONFIG       1
#define SPI_DDR_MOSI        DDRB
#define SPI_DDR_MISO        DDRB
#define SPI_DDR_SCK         DDRB
#define SPI_DDR_SS          DDRB
#define SPI_PORT_SS         PORTB
#define SPI_MOSI            PB5
#define SPI_MISO            PB6
#define SPI_SCK             PB7
#define SPI_SS              PB4 // same as EEPROM!

#define EEPROM_PCB_CONFIG   1
#define __25LC256__
#define EEP_CS_PORT         PORTB
#define EEP_CS_DDR          DDRB
#define EEP_CS              PB4

// Application specific pin assignments
// ----------------------------------------------------------------------------

#ifdef DEBUG_LED_OUTPUT
 #define DBG_0_PIN           PC0
 #define DBG_1_PIN           PC1
 #define DBG_2_PIN           PC2
 #define DBG_3_PIN           PC3
 #define DBG_4_PIN           PC4
 #define DBG_5_PIN           PC5
 #define DBG_PORT            PORTC
 #define DBG_DDR             DDRC
 #define DBG_ALL_MASK        (1<<DBG_0_PIN | 1<<DBG_1_PIN | 1<<DBG_2_PIN | 1<<DBG_3_PIN | 1<<DBG_4_PIN | 1<<DBG_5_PIN)
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
