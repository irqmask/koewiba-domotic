/**
 * @addtogroup INPUTS
 * @brief This module contains functions to detect the state of inputs.
 *
 * Inputs have negative logic, due to pull-up resistor.
 * An input is activated, when the input is electrically low.
 * @{
 * @file    inputs.c
 * @brief   This module contains functions to detect the state of inputs.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "clock.h"
#include "inputs.h"

// --- Definitions -------------------------------------------------------------

#define INPUT_UP_PIN            PC4
#define INPUT_DOWN_PIN          PC3
#define INPUT_WINDOW_PIN	    PC2
#define NBR_OF_INPUTS           3

#define INPUT_DEBOUNCE_TIME     80
#define INPUT_TIMER_INTERVAL    (INPUT_DEBOUNCE_TIME / 8)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static clock_timer_t    g_input_timer;
static uint8_t          g_debounce_array[NBR_OF_INPUTS];
static bool             g_last_state[NBR_OF_INPUTS];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static bool input_active           (uint8_t input_nbr)
{
    // Inputs have negative logic, due to pull-up resistor.
    // An input is activated, when the input is electrically low.
    return (g_debounce_array[input_nbr] == 0xFF);
}

static bool input_on_activation    (uint8_t input_nbr)
{
    bool temp;

    temp = input_active(input_nbr);
    if (g_last_state[input_nbr] != temp) {
        g_last_state[input_nbr] = temp;
        return temp;
    }
    return false;
}

/*static bool input_on_deactivation  (uint8_t input_nbr)
{
    bool temp;

    temp = input_active(input_nbr);
    if (g_last_state[input_nbr] != temp) {
        g_last_state[input_nbr] = temp;
        return !temp;
    }
    return false;
}*/

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize input module. Configure input pins.
 */
void input_initialize       (void)
{
    uint8_t n;

    DDRC &= ~((1<<INPUT_UP_PIN) | (1<<INPUT_DOWN_PIN) | (1<<INPUT_WINDOW_PIN));
    // activate internal pull-up resistors
    PORTC |= ((1<<INPUT_UP_PIN) | (1<<INPUT_DOWN_PIN) | (1<<INPUT_WINDOW_PIN));

    // activate pin-change-interrupts for the inputs
    PCMSK1 |= ((1<<PCINT12) | (1<<PCINT11) | (1<<PCINT10));

    // clear debounce array
    for(n=0; n<NBR_OF_INPUTS; n++) {
        g_debounce_array[n] = 0xFF;
        g_last_state[n] = true;
    }
    clk_timer_start(&g_input_timer, CLOCK_MS_2_TICKS(INPUT_TIMER_INTERVAL));
}

bool input_up               (void)
{
    //return ((PINC & (1<<INPUT_UP_PIN)) == 0);
    return input_on_activation(2);
}

bool input_down             (void)
{
    //return ((PINC & (1<<INPUT_DOWN_PIN)) == 0);
    return input_on_activation(1);
}

bool input_window_closed    (void)
{
    //return ((PINC & (1<<INPUT_WINDOW_PIN)) == 0);
    return input_active(0);
}

void input_background       (void)
{
    uint8_t n;

    if (clk_timer_is_elapsed(&g_input_timer)) {
        clk_timer_start(&g_input_timer, CLOCK_MS_2_TICKS(INPUT_TIMER_INTERVAL));
        // this part is called every INPUT_TIMER_INTERVAL milliseconds.
        for(n=2; n<2+NBR_OF_INPUTS; n++) {
            g_debounce_array[n-2] <<= 1;
            g_debounce_array[n-2] |= (PINC & (1<<n)) ? 1:0;
        }
    }
}

/** @} */
