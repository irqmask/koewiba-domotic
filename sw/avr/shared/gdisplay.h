/**
 * @addtogroup DISPLAY
 * @addtogroup GDISPLAY
 * @brief Graphical LCD routines.
 *
 * Contains functions to display content on a graphic display.
 * Supports ST7565 driver chip.
 *
 * Supports pcbconfig.h for individual PCB settings of the LCD pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    gdisplay.h
 * @brief   Graphical LCD routines.
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

#ifndef _GDISPLAY_H_
#define _GDISPLAY_H_

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
 * @subsection DISP_ST7565_PCBCONFIG
 * Configure LCD Pinout.
 * @{
 */
#ifndef GDISPLAY_PCBCONFIG
 #define GDISPLAY_PCBCONFIG  1
 //! Defines the DDR of a port which contains the backlight LED signal.
 #define DISP_DDR_LED   DDRD
 //! Defines the PORT which contains the backlight LED signal.
 #define DISP_PORT_LED  PORTD
 //! Defines the LED pin.
 #define DISP_LED       PD7
#endif // GDISPLAY_PCBCONFIG
/** @} */

/**
 * @subsection GDISPLAY_APPCONFIG
 * Application specific settings of the graphical display.
 * @{
 */
#ifndef GDISPLAY_APPCONFIG
 #define GDISPLAY_APPCONFIG 1
 #define GDISP_RAM_PAGES    4
 #define GDISP_WIDTH        132
 #define GDISP_HEIGHT       32
 #define  GDISP_TYPE_ST7565 1
 #undef  GDISP_TYPE_SH1106
#endif // GDISPLAY_APPCONFIG
/** @} */

#define gdisp_put_text_8x8P(__str) gdisp_put_text_8x8p(PSTR(__str))

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const uint8_t gdisp_font1_x8[];
extern const uint8_t gdisp_font1_x16[];
extern const uint8_t gdisp_font1_x24[];
extern const uint8_t gdisp_symbols_x16[];
extern const uint8_t gdisp_symbols_x24[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            gdisp_initialize    (void);

void            gdisp_backlight     (uint8_t                level);

void            gdisp_choose_font   (const uint8_t*         font);

void            gdisp_goto_col_line (uint8_t                col,
                                     uint8_t                page);

void            gdisp_put_spacer    (uint8_t                height);

void            gdisp_put_char      (const char             single_char);

void            gdisp_put_text      (const char*            text);

void            gdisp_put_textp     (const char*            text);

void            gdisp_set_startline (uint8_t                line);

#endif // _GDISPLAY_H_
/**
 * @}
 * @}
 */
