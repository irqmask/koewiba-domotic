/**
 * @addtogroup INPUTS
 * @brief This module contains functions to detect the state of inputs.
 *
 * @{
 * @file    inputs.c
 * @brief   This module contains functions to detect the state of inputs.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "inputs.h"

// --- Definitions -------------------------------------------------------------

#define INPUT_UP_PIN        PC4
#define INPUT_DOWN_PIN      PC3
#define INPUT_WINDOW_PIN	PC2

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize input module. Configure input pins.
 */
void input_initialize       (void)
{
    DDRC &= ~((1<<INPUT_UP_PIN) | (1<<INPUT_DOWN_PIN) | (INPUT_WINDOW_PIN));
    // activate internal pull-up resistors
    PORTC |= ((1<<INPUT_UP_PIN) | (1<<INPUT_DOWN_PIN) | (INPUT_WINDOW_PIN));
}

bool input_up               (void)
{
    return ((PINC & (1<<INPUT_UP_PIN)) == 0);
}

bool input_down             (void)
{
    return ((PINC & (1<<INPUT_DOWN_PIN)) == 0);
}

bool input_window_closed    (void)
{
    return ((PINC & (1<<INPUT_WINDOW_PIN)) == 0);
}

/** @} */
