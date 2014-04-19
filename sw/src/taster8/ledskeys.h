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
    eLed_off,
    eLed_on,
    eLed_blink_slow,
    eLed_blink_fast
} eLEDMode_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void        leds_keys_init          (void);

void        led_switch              (uint8_t uLED, eLEDMode_t eMode);

uKeyVal_t   key_get_pressed         (uKeyVal_t uKeyMask);

uKeyVal_t   key_get_pressed_repeat  (uKeyVal_t uKeyMask);

uKeyVal_t   key_get_pressed_short   (uKeyVal_t uKeyMask);

uKeyVal_t   key_get_pressed_long    (uKeyVal_t uKeyMask);

void        key_clear               (void);

#endif // _LEDS_KEYS_H_
/** @} */
