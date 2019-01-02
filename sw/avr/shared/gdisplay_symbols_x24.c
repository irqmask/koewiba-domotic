/**
 * @addtogroup DISPLAY
 * @addtogroup GDISPLAY
 * @brief Contains symbols with the size of 13x24 pixels for a graphic display.
 *
 * The following paragraph describes the format of the symbol data. At the
 * beginning three bytes of a header describe the font format:
 * 1st byte: Bytes per character
 * 2nd byte: Width in pixels
 * 3rd byte: Height in pixels
 * 4th byte till end: character data.
 *
 * With the forth byte the character data begins. Every character begins with
 * one length byte. it represents the width of the character in pixels. For
 * example the ! has a width of 4.
 * Then each byte represents a column of picels of a character. If the height is
 * greater than one byte, all bytes of one column are consecutively.
 * The space between each char has to be added manually.
 *
 * @{
 * @file    gdisplay_symbols_x24.c
 * @brief   Contains symbols with the size of 13x24 pixels for a graphic display.
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


// FontName : Symbols13x24
// FontSize : 13 x 24

const uint8_t gdisp_symbols_x24[] PROGMEM = {
        // header
        40, 13, 24,

        // degree symbol
        0x0D,
        0xE0, 0x00, 0x00,
        0xF8, 0x03, 0x00,
        0xBC, 0x07, 0x00,
        0x0E, 0x0E, 0x00,
        0x06, 0x0C, 0x00,
        0x07, 0x1C, 0x00,
        0x03, 0x18, 0x00,
        0x07, 0x1C, 0x00,
        0x06, 0x0C, 0x00,
        0x0E, 0x0E, 0x00,
        0xBC, 0x07, 0x00,
        0xF8, 0x03, 0x00,
        0xE0, 0x00, 0x00,

        // window closed symbol
        0x0D,
        0xFE, 0xFF, 0x3F,
        0x02, 0x00, 0x20,
        0xFA, 0xFF, 0x2F,
        0x0A, 0x08, 0x28,
        0x0A, 0x08, 0x28,
        0x0A, 0x08, 0x28,
        0xFA, 0xFF, 0x2F,
        0x0A, 0x08, 0x28,
        0x0A, 0x08, 0x28,
        0x0A, 0x08, 0x28,
        0xFA, 0xFF, 0x2F,
        0x02, 0x00, 0x20,
        0xFE, 0xFF, 0x3F,

        // window opened symbol
        0x0D,
        0xFE, 0xFF, 0x3F,
        0x02, 0x00, 0x20,
        0xFA, 0xFF, 0x2F,
        0x12, 0x10, 0x30,
        0x12, 0x10, 0x30,
        0x22, 0x20, 0x20,
        0xE2, 0xFF, 0x3F,
        0x42, 0x40, 0x40,
        0x42, 0x40, 0x40,
        0x82, 0x80, 0x80,
        0x82, 0xFF, 0xFF,
        0x02, 0x00, 0x20,
        0xFE, 0xFF, 0x3F
};
