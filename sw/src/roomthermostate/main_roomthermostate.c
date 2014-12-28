/**
 * @addtogroup ROOMTHERMOSTATE
 *
 * @{
 * @file    main_roomthermostate.c
 * @brief   MAin entry point of roomthermostate.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "cmddef_common.h"

#include "bus.h"
#include "clock.h"
#include "gdisplay.h"
#include "register.h"
#include "sleepmode.h"
#include "spi.h"
#include "ucontroller.h"
#include "zagwire.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_sBus;
static sClkTimer_t g_sAppTimer;
static uint16_t    g_uTargetTemp;
static uint16_t    g_uCurrentTemp;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void vDrawNibble(uint8_t uNibble)
{
    uNibble &= 0x0F;
    if (uNibble > 9) {
        uNibble -= 10;
        uNibble += 'A';
    } else {
        uNibble += '0';
    }
    GDISP_vPutChar(uNibble);
}

void vDrawHex8Value(uint8_t uVal)
{
    GDISP_vPutChar('0');
    GDISP_vPutChar('x');
    vDrawNibble(uVal >> 4);
    vDrawNibble(uVal & 0x0F);
}

void vDrawHex16Value(uint16_t uVal)
{
    GDISP_vPutChar('0');
    GDISP_vPutChar('x');
    vDrawNibble((uVal >> 12) & 0x000F);
    vDrawNibble((uVal >> 8) & 0x000F);
    vDrawNibble((uVal >> 4) & 0x000F);
    vDrawNibble(uVal & 0x000F);
}

void vDrawTemp(uint16_t uTemperature)
{
    uint8_t byte;
    BOOL firstdigit = FALSE;

    if (uTemperature >= 27315) {
        uTemperature -= 27315;

        byte = uTemperature / 10000;
        if (byte > 0) {
            uTemperature -= byte * 10000;
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
    GDISP_vChooseFont(GDISP_auSymbols_x16);
    GDISP_vPutText(" "); //32
    GDISP_vChooseFont(GDISP_auFont1_x16);
    GDISP_vPutText("C");

    GDISP_vGotoColLine(61, 1);
    GDISP_vChooseFont(GDISP_auFont1_x16);
    vDrawTemp(g_uTargetTemp);
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
static void vInterpretMessage(sBus_t* psBus, uint8_t* puMsg, uint8_t uMsgLen, uint16_t uSender)
{
    if (puMsg[0] <= CMD_eStateDateTime) {
        // state messages
        if (puMsg[0] == CMD_eStateBitfield) {
            if (uSender == 0x0B) {
                g_uTargetTemp += 10;
                vDrawTemperatures();
            } else {
                if (puMsg[2] & 0b00000001) vDrawWindowOpened();
                else                       vDrawWindowClosed();
            }
        }
    } else if (puMsg[0] <= CMD_eSetRegister32bit) {
        // register messages
        register_do_command(psBus, puMsg, uMsgLen, uSender);
    } else {
        // system messages
        switch (puMsg[0]) {
        case CMD_eAck:
            g_sBus.eModuleState = eMod_Running;
            break;
        case CMD_eSleep:
            sleep_pinchange2_enable();
            bus_sleep(psBus);
            sleep_pinchange2_disable();
            break;
        default:
            break;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t module_id = BUS_UNKNOWNADR, sender = 0, val;

    DDRC |= ((1<<PC3) | (1<<PC4));
    PORTC &= ~((1<<PC3) | (1<<PC4));

    clk_initialize();

    register_set_u16(MOD_eReg_ModuleID, 0x000E);

    // configure a bus node with address X
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_sBus, module_id);
    bus_initialize(&g_sBus, 0);// initialize bus on UART 0

    SPI_vMasterInitBlk();
    ZAGW_vInit();

    sei();

    register_get(APP_eReg_DesiredTempDay1, 0, &g_uTargetTemp);

    GDISP_vInit();
    GDISP_vGotoColLine(0,0);
    GDISP_vPutText("aktuell");

    GDISP_vGotoColLine(61,0);
    GDISP_vPutText("Soll");

    vDrawTemperatures();
    vDrawWindowClosed();

    GDISP_vChooseFont(GDISP_auFont1_x8);
    GDISP_vGotoColLine(0,3);
    GDISP_vPutText("Addr: ");
    vDrawHex16Value(g_sBus.sCfg.uOwnNodeAddress);

    clk_timer_start(&g_sAppTimer, 100);
    while (1) {
        if (bus_get_message(&g_sBus)) {
            if (bus_read_message(&g_sBus, &sender, &msglen, msg)) {
                vInterpretMessage(&g_sBus, msg, msglen, sender);
            }
        }
        if (clk_timer_is_elapsed(&g_sAppTimer)) {
            clk_timer_start(&g_sAppTimer,10);
            GDISP_vChooseFont(GDISP_auFont1_x8);
            GDISP_vGotoColLine(0,3);
            GDISP_vPutText("Temp: ");
            if (ZAGW_uReceive()) {
                val = ZAGW_uGetBits();
                vDrawHex16Value(val);
                g_uCurrentTemp = ZAGW_uGetTemperature();
                vDrawTemperatures();
            } else {
                GDISP_vPutText("PERR");
            }
        }
    }
    return 0;
}

/** @} */
