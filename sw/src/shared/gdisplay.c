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

uint8_t* g_puCurrFont   = NULL;
uint8_t g_uCurrCol      = 0;
uint8_t g_uCurrPage     = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Draw a char at the current column and line.
 */
static void vPutChar(const char cChar)
{
    uint16_t indexchar, index;
    uint8_t  width, colsleft, bytespercol;

    if (cChar >= 32 && cChar < 128) {
        // charindex * bytes per char + header
        indexchar = cChar - 32;
        // bytes per character
        indexchar *= pgm_read_byte(&g_puCurrFont[0]);
        // add header size
        indexchar += 3;
        width = pgm_read_byte(&g_puCurrFont[indexchar++]);
        // calculate number of bytes per columns
        bytespercol = pgm_read_byte(&g_puCurrFont[2]) >> 3;

        index = indexchar;
        colsleft = width;
        // draw first line of the character
        while (colsleft--) {
            ST7565_vWriteData(pgm_read_byte(&g_puCurrFont[index]));
            index += bytespercol;
        }
        // character is at least 2 lines high?
        if (bytespercol >= 2) {
            // next 8 lines
            ST7565_vPageAddr(g_uCurrPage + 1);
            ST7565_vColumnAddr(g_uCurrCol);
            // reset index and width
            index = indexchar + 1;
            colsleft = width;
            // draw next 8 lines
            while (colsleft--) {
                ST7565_vWriteData(pgm_read_byte(&g_puCurrFont[index]));
                index += bytespercol;
            }
        }
        // character is at least 3 lines high?
        if (bytespercol >= 3) {
            // next 8 lines, reset column
            ST7565_vPageAddr(g_uCurrPage + 2);
            ST7565_vColumnAddr(g_uCurrCol);
            // reset index and width
            index = indexchar + 2;
            colsleft = width;
            // draw next 8 lines
            while (colsleft--) {
                ST7565_vWriteData(pgm_read_byte(&g_puCurrFont[index]));
                index += bytespercol;
            }
        }
        // refresh current column count
        // leave current page unchanged
        g_uCurrCol += width;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize the graphical display. The visible site of the display will be
 * cleared.
 */
void            GDISP_vInit         (void)
{
    uint8_t ii, page;

    // set portpin of backlight to output
    DISP_DDR_LED   |= (1<<DISP_LED);
    // backlight off
    DISP_PORT_LED  &= ~(1<<DISP_LED);
    ST7565_vInit();
    g_puCurrFont    = GDISP_auFont1_x8;

    // clear display ram
    for (page=0; page<GDISP_RAM_PAGES; page++) {
        ST7565_vPageAddr(page);
        ST7565_vColumnAddr(0);
        for (ii=0; ii<GDISP_WIDTH; ii++)
            ST7565_vWriteData(0);
    }
    // set cursor to 0,0
    ST7565_vPageAddr(0);
    g_uCurrPage = 0;
    ST7565_vColumnAddr(0);
    g_uCurrCol = 0;

    // set start line to the beginning of the display ram
    ST7565_vStartLine(0);
}

/**
 * Control the display's backlight.
 * Currently it will only be switched on or off.
 *
 * @param[in] uLevel
 * Display brightness (if digital 0=backlight off >0 backlight on.
 */
void            GDISP_vBacklight    (uint8_t                uLevel)
{
    if (uLevel) {
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
void            GDISP_vGotoColLine  (uint8_t                uCol,
                                     uint8_t                uLine)
{
    ST7565_vPageAddr(uLine);
    g_uCurrPage = uLine;
    ST7565_vColumnAddr(uCol);
    g_uCurrCol = uCol;
}

/**
 * Select the font, which will be used for the next GDISP_vPutText calls.
 *
 * @param[in] puFont
 * Pointer to font data.
 */
void            GDISP_vChooseFont   (const uint8_t*         puFont)
{
    if (puFont != NULL) {
        g_puCurrFont = puFont;
    }
}

/**
 * Put a text to the current drawing position. Text resides in data memory.
 * @note Set the drawing position with GDISP_vGotoColLine().
 *
 * @param[in] pcText
 * Pointer to null terminated text in data memory.
 */
void            GDISP_vPutText      (const char*            pcText)
{
    char c;
    uint8_t first = 1;

    while ((c = *pcText++) != '\0') {
        if (first) {
            first = 0;
        } else {
            ST7565_vWriteData(0); // 1 pixel distance between chars
            g_uCurrCol++;
        }
        vPutChar(c);
    }
}

/**
 * Put a text to the current drawing position. Text resides in program memory.
 * @note Set the drawing position with GDISP_vGotoColLine().
 *
 * @param[in] pcText
 * Pointer to null terminated text in program memory.
 */
void            GDISP_vPutTextp      (const char*           pcTextPgm)
{
    char c;
    uint8_t first = 1;

    while ((c = pgm_read_byte(pcTextPgm++)) != '\0') {
        if (first) {
            first = 0;
        } else {
            ST7565_vWriteData(0); // 1 pixel distance between chars
            g_uCurrCol++;
        }
        vPutChar(c);
    }
}

/** @} */
