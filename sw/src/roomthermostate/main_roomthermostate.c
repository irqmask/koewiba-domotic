/**
 * @addtogroup TEST_PROTOCOL
 *
 * @{
 * @file    main_testprotocol.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>

//#include "bus.h"
//#include "clock.h"
//#include "led_debug.h"
#include "gdisplay.h"
#include "disp_st7565.h"
#include "spi.h"
//#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

//static sBus_t      g_sBus;
//static sClkTimer_t g_sLedTimer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    int8_t runner=0,incr=1;
    
    DDRC |= ((1<<PC3) | (1<<PC4));
    PORTC &= ~((1<<PC3) | (1<<PC4));
    SPI_vMasterInitBlk();
    GDISP_vInit();
    GDISP_vGotoColPage(0,0);
    GDISP_vPutText8x8   ("Hallo Welt!");
    GDISP_vGotoColPage(0,1);
    GDISP_vPutText8x8   ("!\"§$%&/()=");
    GDISP_vGotoColPage(0,2);
    GDISP_vPutText8x8   ("1234567890");
    GDISP_vGotoColPage(0,3);
    GDISP_vPutText8x8   ("@ABCDEFGHIJK");
    GDISP_vGotoColPage(0,4);
    GDISP_vPutText8x8   ("abcdef{}");

    
    while (1) {
        PORTC |= (1<<PC3);
        PORTC &= ~(1<<PC4);

        GDISP_vBacklight(TRUE);
        
        _delay_ms(250);
        PORTC |= (1<<PC4);
        PORTC &= ~(1<<PC3);

        GDISP_vBacklight(FALSE);
        _delay_ms(250);
        runner += incr;
        if (runner >= 15 && incr > 0) {
            runner = 15;
            incr = -1;
        } else if (runner <= 0 && incr < 0) {
            runner = 0;
            incr = 1;
        }
        ST7565_vStartLine(runner);
    }
    return 0;
}

/** @} */
