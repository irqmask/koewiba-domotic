/**
 * @addtogroup DISPLAY
 * @addtogroup GDISPLAY
 * @brief Contains symbols with a height of 16 pixels for a graphic display.
 *
 * The following paragraph describes the format of the font data. At the
 * beginning three bytes of a header describe the font format:
 * 1st byte: Bytes per character
 * 2nd byte: Width in pixels
 * 3rd byte: Height in pixels
 * 4th byte till end: character data.
 *
 * With the forth byte the character data begins. Every character begins with
 * one length byte. it represents the width of the character in pixels. For
 * example the ! has a width of 3.
 * Then each byte represents a column of picels of a character. If the height is
 * greater than one byte, all bytes of one column are consecutively.
 * The space between each char has to be added manually.
 *
 * @{
 * @file    gdisplay_symbols_x16.c
 * @brief   Contains symbols with a height of 16 pixels for a graphic display.
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

// --- Include section ---------------------------------------------------------

#include <avr/pgmspace.h>

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

const uint8_t gdisp_symbols_x16[] PROGMEM = {
        0x0F, 0x0F, 0x10,
        0x07, 0x1E, 0x00, 0x3F, 0x00, 0x33, 0x00, 0x33, 0x00, 0x33, 0x00, 0x3F, 0x00, 0x1E, 0x00, // Code for char
        };
//        0x07, 0x78, 0x00, 0xFC, 0x00, 0xCC, 0x00, 0xCC, 0x00, 0xCC, 0x00, 0xFC, 0x00, 0x78, 0x00, // Code for char

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
