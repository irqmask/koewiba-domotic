/**
 * @addtogroup SCHEDULER_PCBCONFIG
 * @brief PCB configuration of the "scheduler" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "scheduler" application.
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

#define BUS_PCBCONFIG 1

#define TXRXEN1_SEPERATE 1

#define BUS_DDR_ENASND0  DDRA
#define BUS_DDR_ENASND1  DDRA
#define BUS_DDR_DISRCV0  DDRA
#define BUS_DDR_DISRCV1  DDRA
#define BUS_PORT_ENASND0 PORTA
#define BUS_PORT_ENASND1 PORTA
#define BUS_PORT_DISRCV0 PORTA
#define BUS_PORT_DISRCV1 PORTA
#define BUS_ENASND0      PA6
#define BUS_ENASND1      PA5
#define BUS_DISRCV0      PA6
#define BUS_DISRCV1      PA2

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

// Application specific pin assignments
// ----------------------------------------------------------------------------

// Pin assignments of board keys and LEDs
#define LED_PCBCONFIG       1
#define LED_ERROR           PA0  //!< Red error LED
#define LED_ERROR_DDR       DDRA
#define LED_ERROR_PORT      PORTA
#define LED_STATUS          PA1  //!< Yellow status
#define LED_STATUS_DDR      DDRA
#define LED_STATUS_PORT     PORTA

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
