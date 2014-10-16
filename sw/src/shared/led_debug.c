/**
 * @addtogroup DEBUG
 * @brief   Debug output via LEDs.
 *
 * @{
 * @file    led_debug.c
 * @brief   Debug output via LEDs.
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
void dbg_toggle_status_LED (void)
{
    PORTD ^= LED_STATUS;
}

void dbg_status_LED_on (void)
{
    PORTD |= LED_STATUS;
}

void dbg_status_LED_off (void)
{
    PORTD &= ~LED_STATUS;
}

void dbg_toggle_error_LED (void)
{
    PORTD ^= LED_ERROR;
}

void dbg_error_LED_on (void)
{
    PORTD |= LED_ERROR;
}

void dbg_error_LED_off (void)
{
    PORTD &= ~LED_ERROR;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------
