/**
 * @addtogroup LED2_ATMEGA324P
 * @brief PCB configuration of the "led2_atmega324p" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "led2_atmega324p" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
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

// bus-module_atmega324 specific pin assignments
// ----------------------------------------------------------------------------

#define BUS_PCBCONFIG       1
#define BUS_DDR_ENASND0     DDRD
#define BUS_PORT_ENASND0    PORTD
#define BUS_ENASND0         PD4

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

// Pin assignments of board keys and LEDs
#define LED_PCBCONFIG       1
#define LED_STATUS          PC3  //left yellow LED
#define LED_ERROR           PC4  //right yellow LED
#define BTN_TEST            PC5
#define BTN_EXP             PC6
#define LED_STATUS_DDR      DDRC
#define LED_ERROR_DDR       DDRC
#define LED_STATUS_PORT     PORTC
#define LED_ERROR_PORT      PORTC

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
