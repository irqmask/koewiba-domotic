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
#include "ucontroller.h"
#include <avr/interrupt.h>


// --- Definitions -------------------------------------------------------------



// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------


/**
 * Toggle the status LED.
 */
void vToggleStatusLED(void)
{
    PORTD ^= LED_STATUS;
}
void vStatusLED_on(void)
{
    PORTD |= LED_STATUS;
}
void vStatusLED_off(void)
{
    PORTD &= ~LED_STATUS;
}

void vToggleErrorLED(void)
{
    PORTD ^= LED_ERROR;
}
void vErrorLED_on(void)
{
    PORTD |= LED_ERROR;
}
void vErrorLED_off(void)
{
    PORTD &= ~LED_ERROR;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------
#if defined (__AVR_ATmega88__)
ISR(INTERRUPT_PINCHANGE2)
{

}
#endif
