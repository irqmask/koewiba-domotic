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
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_sBus;
static sClkTimer_t g_sLedTimer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
	uint8_t light = 0, oldbutton = 0;
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGLEN];
    uint16_t sender = 0;

    CLK_vInitialize();

    //own address sends to receiver
    // 1 -> 3
    // 2 -> 4
    // 3 -> 5
    // 4 -> 6
    // 5 -> 1
    // 6 -> 2

    BUS_vConfigure(&g_sBus, 0xc); // configure a bus node with address 2
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0

    vInitLedAndKeys();
    sei();

    CLK_bTimerStart(&g_sLedTimer, 1000);

    while (1) {
        // check for message and read it
        if (BUS_bGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
                // switch light?
                // type bit-field?
                if (msg[0] == SLEEPCOMMAND) {
                	BUS_vSleep(&g_sBus);
                }
                if (msg[0] == 0x01) {
                    if (msg[2] & 0b00000001) LED_ERROR_ON;
                    else                     LED_ERROR_OFF;
                }
            }
        }
        // check button
        if ((PIND & BTN_TEST) ^ oldbutton) {
            oldbutton = PIND & BTN_TEST;
            if (oldbutton != 0) {
                if (light)  light = 0;
                else        light = 1;

                msg[0] = 0x01;
                msg[1] = 1;
                msg[2] = light;
                msg[3] = 0b00000001;
                msglen = 4;
                BUS_bSendMessage(&g_sBus, 5, msglen, msg);
            }
        }
        // check timer
      /*  if (CLK_bTimerIsElapsed(&g_sLedTimer)) {
            // TODO remove after debug
            if (light)  light = 0;
            else        light = 1;

            msg[0] = 0x01;
            msg[1] = 1;
            msg[2] = light;
            msg[3] = 0b00000001;
            msglen = 4;
            BUS_bSendMessage(&g_sBus, 5, msglen, msg);
            CLK_bTimerStart(&g_sLedTimer, 100);
        }*/
    }
    return 0;
}

/** @} */
