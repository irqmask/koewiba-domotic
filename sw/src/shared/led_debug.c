/**
 * @addtogroup TEST_PROTOCOL
 *
 * @{
 * @file    led_debug.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include "led_debug.h"


// --- Definitions -------------------------------------------------------------



// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Initialize LEDs and keys.
 */
void vInitLedAndKeys(void)
{
    DDRD |= (LED_ERROR | LED_STATUS);
    DDRD &= ~(BTN_TEST | BTN_EXP);

    PORTD &= ~(LED_ERROR | LED_STATUS); // switch LEDs off.
    PORTD |= (BTN_TEST | BTN_EXP);      // set pull-up for buttons
}

/**
 * Toggle the status LED.
 */
void vToggleStatusLED(void)
{
    PORTD ^= LED_STATUS;
}

void vToggleErrorLED(void)
{
    PORTD ^= LED_ERROR;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------
