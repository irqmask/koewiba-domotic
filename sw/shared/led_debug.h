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
#include "pcbconfig.h"

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
 #define LED_ERROR       PD4  //!< Red error LED
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
 #define LED_RED_OFF       	 LED_RED_PORT &= ~(1<<LED_RED)
 #define LED_RED_TOGGLE    	 LED_RED_PORT ^=  (1<<LED_RED)

 #define LED_YELLOW_ON       LED_YELLOW_PORT |=  (1<<LED_YELLOW)
 #define LED_YELLOW_OFF      LED_YELLOW_PORT &= ~(1<<LED_YELLOW)
 #define LED_YELLOW_TOGGLE   LED_YELLOW_PORT ^=  (1<<LED_YELLOW)

 #define LED_GREEN_ON        LED_GREEN_PORT |=  (1<<LED_GREEN)
 #define LED_GREEN_OFF       LED_GREEN_PORT &= ~(1<<LED_GREEN)
 #define LED_GREEN_TOGGLE    LED_GREEN_PORT ^=  (1<<LED_GREEN)
 
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

/*
void dbg_toggle_status_LED (void);

void dbg_status_LED_on (void);

void dbg_status_LED_off (void);

void dbg_toggle_error_LED (void);

void dbg_error_LED_on (void);

void dbg_error_LED_off (void);
*/
#endif /* LED_DEBUG_H_ */
