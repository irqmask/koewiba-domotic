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
 * Initialize LEDs and keys.
 */
void vInitLedAndKeys(void)
{
    DDRD |= (LED_ERROR | LED_STATUS);
    DDRD &= ~(BTN_TEST | BTN_EXP);

    PORTD &= ~(LED_ERROR | LED_STATUS); // switch LEDs off.
    PORTD |= (BTN_TEST | BTN_EXP);      // set pull-up for buttons

#if defined (__AVR_ATmega88__)
    // Pin-Change-Interrupt
    PCICR  = ((0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0)); //disable IR_PinChange2
    PCMSK2 = ((1<<PCINT23) | (0<<PCINT22) | (1<<PCINT21) | (0<<PCINT20) | (0<<PCINT19) | (0<<PCINT18) | (0<<PCINT17) | (0<<PCINT16));
    PCMSK1 = (               (0<<PCINT14) | (0<<PCINT13) | (0<<PCINT12) | (0<<PCINT11) | (0<<PCINT10) | (0<<PCINT9 ) | (0<<PCINT8 ));
    PCMSK0 = ((0<<PCINT7 ) | (0<<PCINT6 ) | (0<<PCINT5 ) | (0<<PCINT4 ) | (0<<PCINT3 ) | (0<<PCINT2 ) | (0<<PCINT1 ) | (0<<PCINT0 ));
#endif
}

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
