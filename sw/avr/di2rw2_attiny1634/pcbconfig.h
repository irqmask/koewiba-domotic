/**
 * @addtogroup DI2RW2_ATTINY1634
 * @addtogroup PCBCONFIG
 * @brief PCB configuration of the "di2rw2_attiny1634" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "di2rw2_attiny1634" application.
 *
 * @author  Robert Mueller
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

// bus-module_attiny1634 specific pin assignments
// ----------------------------------------------------------------------------

#define BUS_PCBCONFIG 		1
#define BUS_DDR_ENASND0     DDRA
#define BUS_PORT_ENASND0    PORTA
#define BUS_ENASND0         PA6


#define SPI_PCBCONFIG  1
//! Defines the DDR of a port which contains the MOSI signal.
#define SPI_DDR_MOSI   DDRB
//! Defines the DDR of a port which contains the MISO signal.
#define SPI_DDR_MISO   DDRB
//! Defines the DDR of a port which contains the SCK signal.
#define SPI_DDR_SCK    DDRC
//! Defines the DDR of a port which contains the standard slave select signal.
#define SPI_DDR_SS     DDRC
//! Defines the PORT which contains the standard slave select signal.
#define SPI_PORT_SS    PORTC
//! Defines the MOSI pin.
#define SPI_MOSI       PB2
//! Defines the MISO pin.
#define SPI_MISO       PB1
//! Defines the SCK pin.
#define SPI_SCK        PC1
//! Defines the SS (slave select) pin.
#define SPI_SS         PC0 // same as EEPROM!


// Application specific pin assignments
// ----------------------------------------------------------------------------

#define SPI_PCBCONFIG       1
#define SPI_DDR_MOSI        DDRB
#define SPI_DDR_MISO        DDRB
#define SPI_DDR_SCK         DDRC
#define SPI_DDR_SS          DDRC
#define SPI_PORT_SS         PORTC
#define SPI_MOSI            PB2
#define SPI_MISO            PB1
#define SPI_SCK             PC1
#define SPI_SS              PC0 // same as EEPROM!

#define EEPROM_PCB_CONFIG   1
#define __25LC256__
#define EEP_CS_PORT         PORTC
#define EEP_CS_DDR          DDRC
#define EEP_CS              PC0

// switch inputs
#define INPUT_PCBCONFIG     1
#define INPUT_NUM_PINS      2
#define INPUT_PORT_0        PUEA // Pull-Up Enable register
#define INPUT_PIN_0         PINA
#define INPUT_DDR_0         DDRA
#define INPUT_0             PA0
#define INPUT_PORT_1        PUEA // Pull-Up Enable register
#define INPUT_PIN_1         PINA
#define INPUT_DDR_1         DDRA
#define INPUT_1             PA1

#define KEY0_WAKEUP         PCINT0 // PA0
#define KEY0_WAKEUP_REG     PCMSK0
#define KEY1_WAKEUP         PCINT1 // PA1
#define KEY1_WAKEUP_REG     PCMSK0

#define RELAY_PCBCONFIG     1
#define RELAY_NUM_PINS      2
#define RELAY0_DDR			DDRA
#define RELAY0_PORT			PORTA
#define RELAY0_PIN			PINA
#define RELAY0				PA2
#define RELAY1_DDR			DDRA
#define RELAY1_PORT			PORTA
#define RELAY1_PIN			PINA
#define RELAY1				PA3

#define OUT_PCBCONFIG     	1
#define OUT_NUM_PINS      	2
#define OUT0_DDR			DDRA
#define OUT0_PORT			PORTA
#define OUT0_PIN			PINA
#define OUT0				PA4
#define OUT1_DDR			DDRA
#define OUT1_PORT			PORTA
#define OUT1_PIN			PINA
#define OUT1				PA5



// debug LED outputs
#define LED_PCBCONFIG       1
#define LED_STATUS          PA4         //!< Yellow status LED
#define LED_ERROR           PA5         //!< Red error LED
#define LED_STATUS_DDR      DDRA
#define LED_ERROR_DDR       DDRA
#define LED_STATUS_PORT     PORTA
#define LED_ERROR_PORT      PORTA

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
