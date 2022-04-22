/**
 * @addtogroup DISPLAY
 * @addtogroup GDISPLAY
 * @brief Contains functions to display content on a graphic display.
 *
 * @{
 * @file    gdisplay.c
 * @brief   Contains functions to display content on a graphic display.
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

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "prjtypes.h"
#include "ucontroller.h"

#include "gdisplay.h"

#ifdef GDISP_TYPE_ST7565
#include "disp_st7565.h"
#endif
#ifdef GDISP_TYPE_SH1106
#include "disp_sh1106.h"
#endif


// --- Definitions -------------------------------------------------------------

#ifdef GDISP_TYPE_ST7565
 #define GDISP_INITIALIZE    st7565_initialize
 #define GDISP_WRITE_DATA    st7565_write_data
 #define GDISP_PAGE_ADDR     st7565_page_addr
 #define GDISP_COLUMN_ADDR   st7565_column_addr
 #define GDISP_START_LINE    st7565_start_line
#endif
#ifdef GDISP_TYPE_SH1106
 #define GDISP_INITIALIZE    sh1106_initialize
 #define GDISP_WRITE_DATA    sh1106_write_data
 #define GDISP_PAGE_ADDR     sh1106_page_addr
 #define GDISP_COLUMN_ADDR   sh1106_column_addr
 #define GDISP_START_LINE    sh1106_start_line
 #define GDISP_DISPLAY_ONOFF sh1106_display_onoff
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

const uint8_t*  g_current_font      = NULL;
uint8_t         g_current_column    = 0;
uint8_t         g_current_page      = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Draw a char at the current column and line.
 */
static void put_char(const char single_char, uint8_t height, uint8_t space)
{
    uint16_t indexchar, index;
    uint8_t  width, colsleft;

    if (single_char >= 32 && single_char < 128) {
        // charindex * bytes per char + header
        indexchar = single_char - 32;
        // bytes per character
        indexchar *= pgm_read_byte(&g_current_font[0]);
        // add header size
        indexchar += 3;
        width = pgm_read_byte(&g_current_font[indexchar++]);
        // calculate number of bytes per columns
        height >>= 3;

        index = indexchar;
        colsleft = width;
        // draw first line of the character
        while (colsleft--) {

            GDISP_WRITE_DATA(pgm_read_byte(&g_current_font[index]));
            index += height;
        }
        // space between chars
        for (colsleft=0; colsleft<space; colsleft++) {
            GDISP_WRITE_DATA(0);
        }
        // character is at least 2 lines high?
        if (height >= 2) {
            // next 8 lines
            GDISP_PAGE_ADDR(g_current_page + 1);
            GDISP_COLUMN_ADDR(g_current_column);
            // reset index and width
            index = indexchar + 1;
            colsleft = width;
            // draw next 8 lines
            while (colsleft--) {
                GDISP_WRITE_DATA(pgm_read_byte(&g_current_font[index]));
                index += height;
            }
            // space between chars
            for (colsleft=0; colsleft<space; colsleft++) {
                GDISP_WRITE_DATA(0);
            }            
        }
        // character is at least 3 lines high?
        if (height >= 3) {
            // next 8 lines, reset column
            GDISP_PAGE_ADDR(g_current_page + 2);
            GDISP_COLUMN_ADDR(g_current_column);
            // reset index and width
            index = indexchar + 2;
            colsleft = width;
            // draw next 8 lines
            while (colsleft--) {
                GDISP_WRITE_DATA(pgm_read_byte(&g_current_font[index]));
                index += height;
            }
            // space between chars
            for (colsleft=0; colsleft<space; colsleft++) {
                GDISP_WRITE_DATA(0);
            }
        }
        // refresh current column count
        g_current_column += width;
        g_current_column += space;
        // leave current page unchanged
        GDISP_PAGE_ADDR(g_current_page);
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize the graphical display. The visible site of the display will be
 * cleared.
 */
void gdisp_initialize (void)
{
    uint8_t ii, page;

    // set portpin of backlight to output
    DISP_DDR_LED   |= (1<<DISP_LED);
    // backlight off
    DISP_PORT_LED  &= ~(1<<DISP_LED);
    GDISP_INITIALIZE();
    g_current_font    = gdisp_font1_x8;

    // clear display ram
    for (page=0; page<GDISP_RAM_PAGES; page++) {
        GDISP_PAGE_ADDR(page);
        GDISP_COLUMN_ADDR(0);
        for (ii=0; ii<GDISP_WIDTH; ii++)
            GDISP_WRITE_DATA(0);
    }
    // set cursor to 0,0
    GDISP_PAGE_ADDR(0);
    g_current_page = 0;
    GDISP_COLUMN_ADDR(0);
    g_current_column = 0;

    // set start line to the beginning of the display ram
    GDISP_START_LINE(0);
}

/**
 * Control the display's backlight.
 * Currently it will only be switched on or off.
 *
 * @param[in] level
 * Display brightness (if digital 0=backlight off >0 backlight on.
 */
void gdisp_backlight (uint8_t level)
{
    if (level) {
        DISP_PORT_LED |= (1<<DISP_LED);
    } else {
        DISP_PORT_LED  &= ~(1<<DISP_LED);
    }
}

/**
 * Go to a specific column and line. Output to the display will start at the
 * selected column and line. A line represents always eight rows of pixels.
 *
 * @param[in] uCol
 * Pixel column to select.
 * @param[in] uLine
 * Line to select.
 */
void gdisp_goto_col_line (uint8_t   col,
                          uint8_t   line)
{
    GDISP_PAGE_ADDR(line);
    g_current_page = line;
    GDISP_COLUMN_ADDR(col);
    g_current_column = col;
}

/**
 * Select the font, which will be used for the next GDISP_vPutText calls.
 *
 * @param[in] font
 * Pointer to font data.
 */
void gdisp_choose_font (const uint8_t* font)
{
    if (font != NULL) {
        g_current_font = font;
    }
}

void gdisp_put_spacer (uint8_t height)
{  
    GDISP_WRITE_DATA(0); // 1 pixel
    if (height > 8) {
        GDISP_PAGE_ADDR(g_current_page + 1);
        GDISP_COLUMN_ADDR(g_current_column);
        GDISP_WRITE_DATA(0); // second line
    }
    if (height > 16) {
        GDISP_PAGE_ADDR(g_current_page + 2);
        GDISP_COLUMN_ADDR(g_current_column);
        GDISP_WRITE_DATA(0); // third line
    }
    g_current_column++;
    GDISP_PAGE_ADDR(g_current_page);
}

/**
 * Put a character to the current drawing position.
 *
 * @param[in] single_char
 * Character to display.
 */
void gdisp_put_char (const char single_char)
{
    uint8_t height;

    height = pgm_read_byte(&g_current_font[2]);
    
    put_char(single_char, height, 1);
}

/**
 * Put a text to the current drawing position. Text resides in data memory.
 * @note Set the drawing position with GDISP_vGotoColLine().
 *
 * @param[in] text
 * Pointer to null terminated text in data memory.
 */
void gdisp_put_text (const char* text)
{
    char c;
    uint8_t height;

    height = pgm_read_byte(&g_current_font[2]);
    
    while ((c = *text++) != '\0') {
        put_char(c, height, 1);
    }
}

/**
 * Put a text to the current drawing position. Text resides in program memory.
 * @note Set the drawing position with GDISP_vGotoColLine().
 *
 * @param[in] text
 * Pointer to null terminated text in program memory.
 */
void gdisp_put_textp (const char* text)
{
    char c;
    uint8_t height;

    height = pgm_read_byte(&g_current_font[2]);
    
    while ((c = pgm_read_byte(text++)) != '\0') {
        put_char(c, height, 1);
    }
}

/**
 * Set start line of display to display.
 *
 * @param[in] line
 * Line number (first is 0)
 */
void gdisp_set_startline(uint8_t line)
{
    GDISP_START_LINE(line);
}

/** @} */
