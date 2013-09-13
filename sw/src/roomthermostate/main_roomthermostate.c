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
    GDISP_vInit();
    GDISP_vPutText8x8   ("Hallo Welt!");
    
    while (1) {
        _delay_ms(1);
    }
    return 0;
}

/** @} */
