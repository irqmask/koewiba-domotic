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
#include "cmddef_common.h"
#include "clock.h"
//#include "led_debug.h"
#include "gdisplay.h"
#include "sleepmode.h"
#include "spi.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_sBus;
static sClkTimer_t g_sDisplayTimer;
static uint16_t    g_uTargetTemp;
static uint16_t    g_uCurrentTemp;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void vDrawTemp(uint16_t uTemperature)
{
    uint8_t byte;
    BOOL firstdigit = FALSE;
    
    if (uTemperature >= 27315) {
        uTemperature -= 27315;
        
        byte = uTemperature / 10000;
        if (byte > 0) {
            uTemperature -= byte * 10000;
            //GDISP_vPutChar(byte + 0x30);
            //firstdigit = TRUE;
        } else {
            //GDISP_vPutChar(' ');
        }
        
        byte = uTemperature / 1000;
        if (byte > 0 || firstdigit) {
            uTemperature -= byte * 1000;
            GDISP_vPutChar(byte + 0x30);
            firstdigit = TRUE;
        } else {
            GDISP_vPutChar(' ');
        }

        byte = uTemperature / 100;
        if (byte > 0 || firstdigit) {
            uTemperature -= byte * 100;
            GDISP_vPutChar(byte + 0x30);
            firstdigit = TRUE;
        } else {
            GDISP_vPutChar(' ');
        }
        GDISP_vPutChar(',');
        byte = uTemperature / 10;
        uTemperature -= byte * 10;
        GDISP_vPutChar(byte + 0x30);
    }
}

void vDrawTemperatures(void)
{
    GDISP_vGotoColLine(0, 1);
    GDISP_vChooseFont(GDISP_auFont1_x16);
    vDrawTemp(g_uCurrentTemp);
    //GDISP_vPutText("24,0");
    GDISP_vChooseFont(GDISP_auSymbols_x16);
    GDISP_vPutText(" "); //32
    GDISP_vChooseFont(GDISP_auFont1_x16);
    GDISP_vPutText("C");
    
    GDISP_vGotoColLine(61, 1);
    GDISP_vChooseFont(GDISP_auFont1_x16);
    vDrawTemp(g_uTargetTemp);
    //GDISP_vPutText("23,8");
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

// Interpret message
static void vInterpretMessage(uint8_t* puMsg, uint8_t uMsgLen, uint8_t uSender)
{
    switch (puMsg[0]) {
    case CMD_eStateBitfield:
        if (uSender == 0x0B) {
            g_uTargetTemp += 10;
            vDrawTemperatures();
        } else {
            if (puMsg[2] & 0b00000001) vDrawWindowOpened();
            else                       vDrawWindowClosed();
        }
        break;
    case CMD_eSleep:
        SLEEP_PinChange2_Enable();
        BUS_vSleep(&g_sBus);
        SLEEP_PinChange2_Disable();
        break;
    case CMD_eAck:
    	g_sBus.eModuleState = eMod_Running;
    
    default:
    	BUS_bSendAcknowledge(&g_sBus, g_sBus.sRecvMsg.uSender);
        break;
    }
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
    
    BUS_vConfigure(&g_sBus, 0x0E); // configure a bus node with address X
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0
    
    DDRC |= ((1<<PC3) | (1<<PC4));
    PORTC &= ~((1<<PC3) | (1<<PC4));
    SPI_vMasterInitBlk();
      
    sei();
    
    g_uCurrentTemp = 27315 + 2200;
    g_uTargetTemp = 27315 + 1800;
    
    GDISP_vInit();
    GDISP_vGotoColLine(0,0);
    GDISP_vPutText("aktuell");

    GDISP_vGotoColLine(61,0);
    GDISP_vPutText("Soll");

    vDrawTemperatures();
    vDrawWindowClosed();

    GDISP_vChooseFont(GDISP_auFont1_x8);
    GDISP_vGotoColLine(0,3);
    GDISP_vPutText("So 22.09.2013 21:45");
    
    CLK_bTimerStart(&g_sDisplayTimer, CLOCK_MS_2_TICKS(1000));
    while (1) {
        if (BUS_bGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
                vInterpretMessage(msg, msglen, sender);
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
