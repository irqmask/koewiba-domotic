/**
 * @addtogroup DISPLAY
 * @addtogroup DISPDRV_SH1106
 * @brief Driver for graphical LCD chip SH1106
 *
 * Contains functions to send the commands via SPI to the SH1106 chip. The chip
 * in SPI mode is write-only.
 * The blocking functions of the SPI driver are used to send commands to the chip.
 *
 * Supports pcbconfig.h for individual PCB settings of the LCD and SPI pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    disp_sh1106.h
 * @brief   Driver for graphical LCD chip SH1106.
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
#ifndef _DISP_SH1106_H_
#define _DISP_SH1106_H_

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "prjtypes.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection DISP_SH1106_PCBCONFIG
 * Configure LCD Pinout.
 * @{
 */
#ifndef DISP_SH1106_PCBCONFIG
 #define DISP_SH1106_PCBCONFIG  1
 //! Defines the DDR of a port which contains the slave select signal of the 
 //! display.
 #define DISP_DDR_SS    DDRB
 //! Defines the DDR which contains the A0 signal of the display.
 #define DISP_DDR_A0    DDRB
 //! Defines the PORT which contains the slave select signal of the display.
 #define DISP_PORT_SS   PORTB
 //! Defines the PORT which contains the A0 signal of the display.
 #define DISP_PORT_A0   PORTB
 //! Defines the SS pin.
 #define DISP_SS           PB0
 //! Defines the A0 pin.
 #define DISP_A0           PB1
#endif // DISP_SH1106_PCBCONFIG
/** @} */

/**
 * @subsection DISP_SH1106_APPCONFIG
 * Configure size of the SPI driver. E.g. only blocking functions.
 * @{
 */
#ifndef DISP_SH1106_APPCONFIG
 #define DISP_SH1106_APPCONFIG 1
#endif // DISP_SH1106_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            sh1106_column_addr  (uint8_t                column_address);

void            sh1106_page_addr    (uint8_t                page_address);

void            sh1106_start_line   (uint8_t                start_line);

void            sh1106_contrast     (uint8_t                contrast);

void            sh1106_display_on   (uint8_t                on);

void            sh1106_disp_reverse (uint8_t                reverse);

void            sh1106_display_offset(uint8_t               offset);

void            sh1106_nop          (void);

void            sh1106_initialize   (void);

void            sh1106_write_data   (uint8_t                data);

#endif // _DISP_SH1106_H_
/** 
 * @} 
 * @}
 */
