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
