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

uint8_t* puCurrFont = NULL;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

inline void vPutChar(const char cChar)
{
    uint8_t index, width;
    
    if (cChar >= 32 && cChar < 128) {
        // charindex * bytes per char + header
        index = cChar - 32;
        index *= pgm_read_byte(&puCurrFont[0]);  // bytes per character
        index += 3;                             // header size
        width = pgm_read_byte(&puCurrFont[index++]);
        
        while (width--) {
            ST7565_vWriteData(pgm_read_byte(&puCurrFont[index++]));
        }        
    }
    
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            GDISP_vInit         (void)
{
    uint8_t ii, page;
    
    // set portpin of backlight to output 
    DISP_DDR_LED   |= (1<<DISP_LED);
    // backlight off
    DISP_PORT_LED  &= ~(1<<DISP_LED);
    ST7565_vInit();
    puCurrFont = GDISP_auFont1_8x8;
    
    // clear display ram
    for (page=0; page<GDISP_RAM_PAGES; page++) {
        ST7565_vPageAddr(page);
        ST7565_vColumnAddr(0);
        for (ii=0; ii<GDISP_WIDTH; ii++)
            ST7565_vWriteData(0);
    }
    // set cursor to 0,0
    ST7565_vPageAddr(0);
    ST7565_vColumnAddr(0);
    
    // set start line to the beginning of the display ram
    ST7565_vStartLine(0);    
}    

void            GDISP_vBacklight    (uint8_t uLevel)
{
    if (uLevel) {
        DISP_PORT_LED |= (1<<DISP_LED);
    } else {
        DISP_PORT_LED  &= ~(1<<DISP_LED);
    }   
}    

void            GDISP_vGotoColPage  (uint8_t               uCol,
                                     uint8_t               uPage)
{
    ST7565_vPageAddr(uPage);
    ST7565_vColumnAddr(uCol);
}
                                     
void            GDISP_vPutText8x8   (const char*           pcText)
{
    char c;
    uint8_t first = 1;
    
    while ((c = *pcText++) != '\0') {
        if (first) first = 0;
        else ST7565_vWriteData(0); // 1 pixel distance between chars
        vPutChar(c);
    }   
}

void            GDISP_vPutText8x8p   (const char*           pcTextPgm)
{
    char c;
    uint8_t first = 1;
    
    while ((c = pgm_read_byte(pcTextPgm++)) != '\0') {
        if (first) first = 0;
        else ST7565_vWriteData(0); // 1 pixel distance between chars
        vPutChar(c);
    }
}

/** @} */
