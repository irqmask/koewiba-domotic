/**
 * @addtogroup TEST_SCHEDULER
 *
 * @{
 * @file    main_testscheduler.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "bus.h"
#include "clock.h"
// TODO remove after debug
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// Pin assignments of board keys and LEDs
// Port D pin assignments
#define LED_STATUS      0b01000000  //!< Yellow status LED
#define LED_ERROR       0b00010000  //!< Red error LED
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t 		g_sBus;
static sClkTimer_t 	g_sLedTimer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint16_t sender = 0;
    uint8_t msg[BUS_MAXMSGLEN];

    CLK_vInitialize();
    BUS_vConfigure(&g_sBus, 1); // configure a bus node with address 1
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0

    vInitLedAndKeys();
    sei();

    CLK_bTimerStart(&g_sLedTimer, 1000);

    while (1) {
        if (BUS_bScheduleAndGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
            }
        }

        if (CLK_bTimerIsElapsed(&g_sLedTimer)) {
        	// TODO remove after debug
            LED_STATUS_TOGGLE;
            CLK_bTimerStart(&g_sLedTimer, 1000);
        }
    }
    return 0;
}

/** @} */
