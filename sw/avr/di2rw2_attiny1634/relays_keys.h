/**
 * @addtogroup DI2RW2_ATTINY1634
 * @brief Control relays and keys.
 *
 * Switch relays in a matrix on or off. Read state of keys
 * and determine if those have been pressed short or long or repeatedly.
 *
 * @{
 * @file    relays_keys.h
 * @brief   Control relays and keys.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------
#ifndef _RELAYS_KEYS_H_
#define _RELAYS_KEYS_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"
#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef uint8_t     uKeyVal_t;      //!< Type to store key states.

typedef enum {
    eRelay_off,
    eRelay_on,
    eRelay_sec_delay_off,
    eRelay_sec_delay_on,
    eRelay_min_delay_off,
    eRelay_min_delay_on,
    eRelay_hour_delay_off,
    eRelay_hour_delay_on,
    eRelay_sec_toggle
} eRelayMode_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void        relays_keys_init          (void);

void        relays_keys_background    (void);

void        relay_switch            (uint8_t led_index, eRelayMode_t mode);

uint8_t     key_get_pressed         (uint8_t mask);

uint8_t     key_get_pressed_repeat  (uint8_t mask);

uint8_t     key_get_pressed_short   (uint8_t mask);

uint8_t     key_get_pressed_long    (uint8_t mask);

uint8_t     get_output_states       (void);

#endif // _RELAYS_KEYS_H_
/** @} */
