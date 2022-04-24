/**
 * @addtogroup TEST_MOTOR_SWITCH_5_ATMEGA328
 * @brief PCB configuration of the "test_motor_switch_5_atmega328" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "test_motor_switch_5_atmega328" application.
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

#define SN74595_PCBCONFIG   1
#undef LATCH_OE_ENABLED
#define LATCH_OE_DDR        DDRD
#define LATCH_OE_PORT       PORTD
#define LATCH_OE_PIN        PD7
#define LATCH_STB_DDR       DDRB
#define LATCH_STB_PORT      PORTB
#define LATCH_STB_PIN       PB0

#define MOTOR_PCBCONFIG     1
//! Number of motors supported by the application
#define MOTOR_COUNT         5
#define MOTOR_0_ONOFF       {0x00, 0x40}
#define MOTOR_0_UPDOWN      {0x00, 0x80}
#define MOTOR_1_ONOFF       {0x00, 0x10}
#define MOTOR_1_UPDOWN      {0x00, 0x20}
#define MOTOR_2_ONOFF       {0x00, 0x04}
#define MOTOR_2_UPDOWN      {0x00, 0x08}
#define MOTOR_3_ONOFF       {0x00, 0x01}
#define MOTOR_3_UPDOWN      {0x00, 0x02}
#define MOTOR_4_ONOFF       {0x40, 0x00}
#define MOTOR_4_UPDOWN      {0x80, 0x00}
#define MOTOR_SWITCH_DELAY  80

// debug LED outputs
#define LED_PCBCONFIG       1
#define LED_STATUS          PB1    //!< Yellow status LED
#define LED_ERROR           PD7    //!< Red error LED
#define LED_STATUS_DDR      DDRB
#define LED_ERROR_DDR       DDRD
#define LED_STATUS_PORT     PORTB
#define LED_ERROR_PORT      PORTD

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
