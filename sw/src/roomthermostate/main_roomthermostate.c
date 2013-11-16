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
#include <avr/interrupt.h>

#include "bus.h"
#include "clock.h"
//#include "led_debug.h"
#include "gdisplay.h"
#include "spi.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_sBus;
static sClkTimer_t g_sDisplayTimer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void vDrawTemperature(void)
{
    GDISP_vGotoColLine(0, 1);
    GDISP_vChooseFont(GDISP_auFont1_x16);
    GDISP_vPutText("24,0");
    GDISP_vChooseFont(GDISP_auSymbols_x16);
    GDISP_vPutText(" "); //32
    GDISP_vChooseFont(GDISP_auFont1_x16);
    GDISP_vPutText("C");
    
    GDISP_vGotoColLine(61, 1);
    GDISP_vChooseFont(GDISP_auFont1_x16);
    GDISP_vPutText("23,8");
    GDISP_vChooseFont(GDISP_auSymbols_x16);
    GDISP_vPutText(" "); //32
    GDISP_vChooseFont(GDISP_auFont1_x16);
    GDISP_vPutText("C");
}

void vDrawWindowOpened(void)
{
    GDISP_vGotoColLine(118, 0);
    GDISP_vChooseFont(GDISP_auSymbols_x24);
    GDISP_vPutText("\""); //34
}

void vDrawWindowClosed(void)
{
    GDISP_vGotoColLine(118, 0);
    GDISP_vChooseFont(GDISP_auSymbols_x24);
    GDISP_vPutText("!"); //33
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGLEN];
    uint16_t sender = 0;
    uint8_t displaystate=0;
    
    CLK_vInitialize();
    
    BUS_vConfigure(&g_sBus, 0x0F); // configure a bus node with address X
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0
    
    DDRC |= ((1<<PC3) | (1<<PC4));
    PORTC &= ~((1<<PC3) | (1<<PC4));
    SPI_vMasterInitBlk();
      
    sei();
    
    GDISP_vInit();
    GDISP_vGotoColLine(0,0);
    GDISP_vPutText("aktuell");

    GDISP_vGotoColLine(61,0);
    GDISP_vPutText("Soll");

    vDrawTemperature();
    vDrawWindowClosed();

    GDISP_vChooseFont(GDISP_auFont1_x8);
    GDISP_vGotoColLine(0,3);
    GDISP_vPutText("So 22.09.2013 21:45");
    
    CLK_bTimerStart(&g_sDisplayTimer, CLOCK_MS_2_TICKS(1000));
    while (1) {
        if (BUS_bGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
            }
        }
        /*if (CLK_bTimerIsElapsed(&g_sDisplayTimer)) {
            if (displaystate != 0) {
                displaystate = 0;
                PORTC |= (1<<PC3);
                PORTC &= ~(1<<PC4);
                vDrawWindowOpened();
                GDISP_vBacklight(TRUE);
            } else {
                displaystate = 1;
                PORTC |= (1<<PC4);
                PORTC &= ~(1<<PC3);
                vDrawWindowClosed();
                GDISP_vBacklight(FALSE);
            }
            CLK_bTimerStart(&g_sDisplayTimer, CLOCK_MS_2_TICKS(1000));
        }*/
    }
    return 0;
}

/** @} */
