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
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGLEN];
    uint16_t sender = 0;

    CLK_vInitialize();

    BUS_vConfigure(&g_sBus, 3); // configure a bus node with address 2
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0

    vInitLedAndKeys();
    sei();

    CLK_bTimerStart(&g_sLedTimer, 1000);


    while (1) {
        // check for message and read it
        if (BUS_bGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
            }
        }
    }
    return 0;
}

/** @} */
