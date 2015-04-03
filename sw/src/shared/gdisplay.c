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

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "disp_st7565.h"

#include "gdisplay.h"

// --- Definitions -------------------------------------------------------------

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
            st7565_write_data(pgm_read_byte(&g_current_font[index]));
            index += height;
        }
        // space between chars
        for (colsleft=0; colsleft<space; colsleft++) {
            st7565_write_data(0);
        }
        // character is at least 2 lines high?
        if (height >= 2) {
            // next 8 lines
            st7565_page_addr(g_current_page + 1);
            st7565_column_addr(g_current_column);
            // reset index and width
            index = indexchar + 1;
            colsleft = width;
            // draw next 8 lines
            while (colsleft--) {
                st7565_write_data(pgm_read_byte(&g_current_font[index]));
                index += height;
            }
            // space between chars
            for (colsleft=0; colsleft<space; colsleft++) {
                st7565_write_data(0);
            }            
        }
        // character is at least 3 lines high?
        if (height >= 3) {
            // next 8 lines, reset column
            st7565_page_addr(g_current_page + 2);
            st7565_column_addr(g_current_column);
            // reset index and width
            index = indexchar + 2;
            colsleft = width;
            // draw next 8 lines
            while (colsleft--) {
                st7565_write_data(pgm_read_byte(&g_current_font[index]));
                index += height;
            }
            // space between chars
            for (colsleft=0; colsleft<space; colsleft++) {
                st7565_write_data(0);
            }
        }
        // refresh current column count
        g_current_column += width;
        g_current_column += space;
        // leave current page unchanged
        st7565_page_addr(g_current_page);
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
    st7565_initialize();
    g_current_font    = gdisp_font1_x8;

    // clear display ram
    for (page=0; page<GDISP_RAM_PAGES; page++) {
        st7565_page_addr(page);
        st7565_column_addr(0);
        for (ii=0; ii<GDISP_WIDTH; ii++)
            st7565_write_data(0);
    }
    // set cursor to 0,0
    st7565_page_addr(0);
    g_current_page = 0;
    st7565_column_addr(0);
    g_current_column = 0;

    // set start line to the beginning of the display ram
    st7565_start_line(0);
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
    st7565_page_addr(line);
    g_current_page = line;
    st7565_column_addr(col);
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
    st7565_write_data(0); // 1 pixel
    if (height > 8) {
        st7565_page_addr(g_current_page + 1);
        st7565_column_addr(g_current_column);
        st7565_write_data(0); // second line
    }
    if (height > 16) {
        st7565_page_addr(g_current_page + 2);
        st7565_column_addr(g_current_column);
        st7565_write_data(0); // third line
    }
    g_current_column++;
    st7565_page_addr(g_current_page);
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

/** @} */
