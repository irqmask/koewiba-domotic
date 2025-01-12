/**
 * @addtogroup DI2RW2_ATTINY1634
 *
 * @{
 * @file    relays_keys.c
 * @brief   Control relays and keys.
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

#include "appconfig.h"
#include "pcbconfig.h"
#include "sleepmode.h"
#include "bus.h"

#include "sn74595.h"
#include "spi.h"
#include "timer.h"

#include "relays_keys.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t     g_timer;

static uint8_t key_pressed;
static uint8_t key_repeated;
static uint8_t key_state;
static uint8_t g_relay_states;
// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------


static uint8_t relay_index_to_pin(uint8_t led_index)
{
    switch (led_index) {
    case 0: return RELAY0;
    case 1: return RELAY1;
    case 2: return OUT0;
    case 3: return OUT1;
    default: return 0;
    }
}
/*
static uint8_t relay_index_to_port(uint8_t led_index)
{
    switch (led_index) {
    case 0: return RELAY0_PORT;
    case 1: return RELAY1_PORT;
    case 2: return OUT0_PORT;
    case 3: return OUT1_PORT;
    default: return 0;
    }
}*/

static void relay_on(uint8_t relay_index)
{
    RELAY0_PORT |= (1<<relay_index);
}

static void relay_off(uint8_t relay_index)
{
    RELAY0_PORT &= ~(1<<relay_index);
}

static uint8_t keys_read(void)
{
    uint8_t keystate = 0xFF;

    if (INPUT_PORT_0 & (1<<INPUT_PIN_0)) keystate &= ~(1<<0);
    if (INPUT_PORT_1 & (1<<INPUT_PIN_1)) keystate &= ~(1<<1);

    return keystate;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialized the LED Display and key driver.
 * The timer irq 0 is started to generate an irq every 1,41 milliseconds.
 * Key and display data registers are cleared to zero.
 */
void relays_keys_init (void)
{
    // clear key data registers
    key_repeated    = 0;
    key_state       = 0;
    key_pressed     = 0;

    // clear display data registers
    g_relay_states = 0;

    // set in/out port data direction and initial values.

    // initialize LEDs and switch all off

    // set inputs for keys and enable pull-ups
    RELAY0_DDR  |=  (1<<RELAY0);
    RELAY0_PORT &= ~(1<<RELAY0);

    RELAY1_DDR  |=  (1<<RELAY1);
    RELAY1_PORT &= ~(1<<RELAY1);

    KEY0_WAKEUP_REG |= (1<<KEY0_WAKEUP);
    KEY1_WAKEUP_REG |= (1<<KEY1_WAKEUP);

    // initialize and start timer
    timer_start(&g_timer, TIMER_MS_2_TICKS(150));
}

void relays_keys_background (void)
{
    if (timer_is_elapsed(&g_timer)) {
        key_pressed = keys_read();

        timer_start(&g_timer, TIMER_MS_2_TICKS(150));

        if (key_pressed != 0xFF) sleep_prevent(0x01, 0); // Reset sleep prevention bit as soon as pinchange-interrupt is processed.
    }
}

void relay_switch (uint8_t relay_index, eRelayMode_t mode)
{
    uint8_t pin_idx;

    if (relay_index >= RELAY_NUM_PINS) return;

    pin_idx = relay_index_to_pin(relay_index);

    switch (mode) {
    case eRelay_on:
        relay_on(pin_idx);
        g_relay_states |=  (0x1<<relay_index);
        break;

    //case eRelay_off:
    default:
        relay_off(pin_idx);
        g_relay_states |=  (0x1<<relay_index);
        break;
    }
}

uint8_t get_output_states(void)
{
    return g_relay_states;
}


/**
 * Check if a key has been pressed. Each pressed key is reported only once.
 *
 * @param[in] uKeyMask
 * Mask of keys that should be checked.
 *
 * @returns
 * Mask of keys which are pressed.
 */
/*uint8_t key_get_pressed (uint8_t mask)
{
    // read and clear atomic !
    mask &= key_pressed; // read key(s)
    key_pressed ^= mask; // clear key(s)
    return mask;
}*/

/**
 * Check if a key has been pressed long enough such that the key repeat
 * functionality kicks in. After a small setup delay the key is reported being
 * pressed in subsequent calls to this function. This simulates the user
 * repeatedly pressing and releasing the key.
 *
 * @param[in] uKeyMask
 * Mask of keys that should be checked.
 *
 * @returns
 * Mask of keys which are pressed repetitive.
 */
/*
uint8_t key_get_pressed_repeat (uint8_t mask)
{
    // read and clear atomic !
    mask &= key_repeated; // read key(s)
    key_repeated ^= mask; // clear key(s)
    return mask;
}

uint8_t key_get_pressed_short (uint8_t mask)
{
    // read key state and key press atomic !
    return key_get_pressed(~key_state & mask);
}

uint8_t key_get_pressed_long (uint8_t mask)
{
    return key_get_pressed(key_get_pressed_repeat(mask));
}*/

/** @} */
