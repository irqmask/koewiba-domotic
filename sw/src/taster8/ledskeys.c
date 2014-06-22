/**
 * @addtogroup LEDS_KEYS
 *
 * @{
 * @file    ledskeys.c
 * @brief   Control LEDs and keys.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

#include "appconfig.h"
#include "pcbconfig.h"

#include "ledskeys.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

uKeyVal_t key_pressed;
uKeyVal_t key_repeated;
uKeyVal_t key_state;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialized the LED Display and key driver.
 * The timer irq 0 is started to generate an irq every 1,41 milliseconds.
 * Key and display data registers are cleared to zero.
 */
void        leds_keys_init          (void)
{
    // clear key data registers
    key_repeated    = 0;
    key_state       = 0;
    key_pressed     = 0;

    // clear display data registers

    // set in/out port data direction and initial values.

    // initialize and start timer
}

void        led_switch              (uint8_t uLED, eLEDMode_t eMode)
{
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
uKeyVal_t   key_get_pressed         (uKeyVal_t uKeyMask)
{
    cli();
    // read and clear atomic !
    uKeyMask &= key_pressed; // read key(s)
    key_pressed ^= uKeyMask; // clear key(s)
    sei();
    return uKeyMask;
}

/**
 * Check if a key has been pressed long enough such that the key repeat
 * functionality kicks in. After a small setup delay the key is reported beeing
 * pressed in subsequent calls to this function. This simulates the user
 * repeatedly pressing and releasing the key.
 *
 * @param[in] uKeyMask
 * Mask of keys that should be checked.
 *
 * @returns
 * Mask of keys which are pressed repetitive.
 */
uKeyVal_t   key_get_pressed_repeat  (uKeyVal_t uKeyMask)
{
    cli();
    // read and clear atomic !
    uKeyMask &= key_repeated; // read key(s)
    key_repeated ^= uKeyMask; // clear key(s)
    sei();
    return uKeyMask;
}

uKeyVal_t   key_get_pressed_short   (uKeyVal_t uKeyMask)
{
    cli();
    // read key state and key press atomic !
    return key_get_pressed(~key_state & uKeyMask);
}

uKeyVal_t   key_get_pressed_long    (uKeyVal_t uKeyMask)
{
    return key_get_pressed(key_get_pressed_repeat(uKeyMask));
}

/**
 * Clear current key status.
 */
void        key_clear               (void)
{
    cli();
    key_repeated    = 0;
    key_state       = 0;
    key_pressed     = 0;
    sei();
}

/** @} */
