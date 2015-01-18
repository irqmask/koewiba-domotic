/**
 * @addtogroup DEBUG
 * @brief   Debug output via LEDs.
 *
 * @{
 * @file    led_debug.h
 * @brief   Debug output via LEDs.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------

#ifndef LED_DEBUG_H_
#define LED_DEBUG_H_

// --- Include section ---------------------------------------------------------

#ifdef PRJCONF_UC_AVR
 #include <avr/io.h>
#endif

// --- Definitions -------------------------------------------------------------

#ifdef HAS_APPCONFIG_H
#include "appconfig.h"
#endif
#ifdef HAS_PCBCONFIG_H
#include "pcbconfig.h"
#endif

#ifndef LED_PCBCONFIG
// Pin assignments of board keys and LEDs
// Port D pin assignments
 #define LED_STATUS      0b01000000  //!< Yellow status LED
 #define LED_ERROR       0b00010000  //!< Red error LED
 #define BTN_TEST        0b00100000
 #define BTN_EXP         0b10000000
#endif

#ifdef PRJCONF_UC_AVR
 #define LED_ERROR_ON        PORTA |=  LED_ERROR
 #define LED_ERROR_OFF       PORTA &= ~LED_ERROR
 #define LED_ERROR_TOGGLE    PORTA ^=  LED_ERROR
#else
 #define LED_ERROR_ON
 #define LED_ERROR_OFF
 #define LED_ERROR_TOGGLE
#endif
// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void dbg_toggle_status_LED (void);

void dbg_status_LED_on (void);

void dbg_status_LED_off (void);

void dbg_toggle_error_LED (void);

void dbg_error_LED_on (void);

void dbg_error_LED_off (void);

#endif /* LED_DEBUG_H_ */
