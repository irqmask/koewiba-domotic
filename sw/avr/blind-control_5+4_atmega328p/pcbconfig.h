/**
 * @addtogroup BLINDCONTROL_5+4_ATMEGA328
 * @addtogroup PCBCONFIG
 * @brief PCB configuration of the "blindcontrol_5+4_atmega328p" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "blindcontrol_5+4_atmega328p" application.
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
#define LED_STATUS           PD7
#define LED_ERROR
#define LED_STATUS_DDR       DDRD
#define LED_ERROR_DDR
#define LED_STATUS_PORT      PORTD
#define LED_ERROR_PORT

#define MOTOR_PCBCONFIG    1

//! Number of motors supported by the application
#define MOTOR_COUNT 5

// motor relay pin assignment
//  µC | Signal | Description
//-----+--------+-------------------------------------------
// PC5 | Rel01  | Motor 0 OnOff
// PD5 | Rel02  | Motor 0 UpDown
// PC3 | Rel03  | Motor 1 OnOff
// PD3 | Rel04  | Motor 1 UpDown
// PC1 | Rel05  | Motor 2 OnOff
// PC4 | Rel06  | Motor 2 UpDown
// PC0 | Rel07  | Motor 3 OnOff
// PC2 | Rel08  | Motor 3 UpDown
// PD6 | Rel09  | Motor 4 OnOff
// PD4 | Rel10  | Motor 4 UpDown
#define MOTOR_0_ONOFF_PORT     PORTC
#define MOTOR_0_ONOFF_DDR      DDRC
#define MOTOR_0_ONOFF_PIN      PC5
#define MOTOR_0_UPDOWN_PORT    PORTD
#define MOTOR_0_UPDOWN_DDR     DDRD
#define MOTOR_0_UPDOWN_PIN     PD5

#define MOTOR_1_ONOFF_PORT     PORTC
#define MOTOR_1_ONOFF_DDR      DDRC
#define MOTOR_1_ONOFF_PIN      PC3
#define MOTOR_1_UPDOWN_PORT    PORTD
#define MOTOR_1_UPDOWN_DDR     DDRD
#define MOTOR_1_UPDOWN_PIN     PD3

#define MOTOR_2_ONOFF_PORT     PORTC
#define MOTOR_2_ONOFF_DDR      DDRC
#define MOTOR_2_ONOFF_PIN      PC1
#define MOTOR_2_UPDOWN_PORT    PORTC
#define MOTOR_2_UPDOWN_DDR     DDRC
#define MOTOR_2_UPDOWN_PIN     PC4

#define MOTOR_3_ONOFF_PORT     PORTC
#define MOTOR_3_ONOFF_DDR      DDRC
#define MOTOR_3_ONOFF_PIN      PC0
#define MOTOR_3_UPDOWN_PORT    PORTC
#define MOTOR_3_UPDOWN_DDR     DDRC
#define MOTOR_3_UPDOWN_PIN     PC2

#define MOTOR_4_ONOFF_PORT     PORTD
#define MOTOR_4_ONOFF_DDR      DDRD
#define MOTOR_4_ONOFF_PIN      PD6
#define MOTOR_4_UPDOWN_PORT    PORTD
#define MOTOR_4_UPDOWN_DDR     DDRD
#define MOTOR_4_UPDOWN_PIN     PD4

#define MOTOR_SWITCH_DELAY     80


// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
