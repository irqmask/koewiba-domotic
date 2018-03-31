/**
 * @addtogroup LEDS_KEYS
 * @brief Control LEDs and keys.
 *
 * Switch leds in a matrix on or off or let them blink. Read state of keys
 * and determine if those have been pressed short or long or repeatedly.
 *
 * @{
 * @file    ledskeys.h
 * @brief   Control LEDs and keys.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _LEDS_KEYS_H_
#define _LEDS_KEYS_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"
#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef uint8_t     uKeyVal_t;      //!< Type to store key states.

typedef enum {
    eLED_off,
    eLED_on,
    eLED_blink_slow,
    eLED_blink_fast
} eLEDMode_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void        leds_keys_init          (void);

void        leds_keys_background    (void);

void        led_switch              (uint8_t led_index, eLEDMode_t mode);

uint8_t     key_get_pressed         (uint8_t mask);

uint8_t     key_get_pressed_repeat  (uint8_t mask);

uint8_t     key_get_pressed_short   (uint8_t mask);

uint8_t     key_get_pressed_long    (uint8_t mask);

#endif // _LEDS_KEYS_H_
/** @} */
