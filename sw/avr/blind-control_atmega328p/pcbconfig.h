/**
 * @addtogroup BLINDCONTROL
 * @addtogroup PCBCONFIG
 * @brief PCB configuration of the "blind-control_atmega328p" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "blind-control_atmega328p" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@web.de>
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

#define BUS_PCBCONFIG 1
#define BUS_DDR_ENASND0     DDRD
#define BUS_PORT_ENASND0    PORTD
#define BUS_ENASND0         PD2

#define SPI_PCBCONFIG       1
#define SPI_DDR_MOSI        DDRB
#define SPI_DDR_MISO        DDRB
#define SPI_DDR_SCK         DDRB
#define SPI_DDR_SS          DDRB
#define SPI_PORT_SS         PORTB
#define SPI_MOSI            PB3
#define SPI_MISO            PB4
#define SPI_SCK             PB5
#define SPI_SS              PB2 // same as EEPROM!

#define EEPROM_PCB_CONFIG   1
#define __25LC256__
#define EEP_CS_PORT         PORTB
#define EEP_CS_DDR          DDRB
#define EEP_CS              PB2

// Application specific pin assignments
// ----------------------------------------------------------------------------

// debug LED outputs
#define LED_PCBCONFIG        1
#define LED_STATUS           PD6    //!< Yellow status LED
#define LED_ERROR            PD7    //!< Red error LED
#define LED_STATUS_DDR       DDRD
#define LED_ERROR_DDR        DDRD
#define LED_STATUS_PORT      PORTD
#define LED_ERROR_PORT       PORTD

#define INPUTS_PCBCONFIG    1
#define INPUT_PORT          PORTC
#define INPUT_DDR           DDRC
#define INPUT_PIN           PINC
#define INPUT_0_PIN         PC4
#define INPUT_1_PIN         PC3
#define INPUT_2_PIN         PC2
#define INPUT_PCMSK         PCMSK1
#define INPUT_PCMSK_VAL     ((1<<PCINT12) | (1<<PCINT11) | (1<<PCINT10))
#define NBR_OF_INPUTS       3

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
