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
#include "prjconf.h"

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
#define LED_PCBBONFIG 1
// Pin assignments of board keys and LEDs
// Port D pin assignments
 #define LED_STATUS      PD6  //!< Yellow status LED
 #define LED_STATUS_PORT PORTD
 #define LED_ERROR       PD4  //!< Red error LED
 #define LED_ERROR_PORT  PORTD
 #define BTN_TEST        PD5
 #define BTN_EXP         PD7
#endif

#ifdef PRJCONF_UC_AVR
 #define LED_ERROR_ON        LED_ERROR_PORT &= ~(1<<LED_ERROR)
 #define LED_ERROR_OFF       LED_ERROR_PORT |=  (1<<LED_ERROR)
 #define LED_ERROR_TOGGLE    LED_ERROR_PORT ^=  (1<<LED_ERROR)

 #define LED_STATUS_ON       LED_STATUS_PORT &= ~(1<<LED_STATUS)
 #define LED_STATUS_OFF      LED_STATUS_PORT |=  (1<<LED_STATUS)
 #define LED_STATUS_TOGGLE   LED_STATUS_PORT ^=  (1<<LED_STATUS)
 
#else
 #define LED_ERROR_ON
 #define LED_ERROR_OFF
 #define LED_ERROR_TOGGLE

 #define LED_STATUS_ON
 #define LED_STATUS_OFF
 #define LED_STATUS_TOGGLE
#endif
// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* LED_DEBUG_H_ */
