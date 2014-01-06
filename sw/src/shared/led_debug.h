/**
 * @addtogroup BUS
 * @brief Public interface of bus protocol.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    led_debug.h
 * @brief   Declaration of communication driver routines.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------

#ifndef LED_DEBUG_H_
#define LED_DEBUG_H_

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

// --- Definitions -------------------------------------------------------------

// Pin assignments of board keys and LEDs
// Port D pin assignments
#define LED_STATUS      0b01000000  //!< Yellow status LED
#define LED_ERROR       0b00010000  //!< Red error LED
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000


// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------
void vInitLedAndKeys(void);


#endif /* LED_DEBUG_H_ */
