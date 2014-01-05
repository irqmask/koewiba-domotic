/**
 * @addtogroup TASTER8_PCBCONFIG
 * @brief PCB configuration of the taster8 application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the taster8 application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define BUS_PCBCONFIG 1
#define BUS_DDR_ENASND  DDRD
#define BUS_PORT_ENASND PORTD
#define BUS_ENASND      PD2
#define BUS_DDR_DISRCV  DDRD
#define BUS_PORT_DISRCV PORTD
#define BUS_DISRCV      PD3

// Pin assignments of board keys and LEDs
// Port D pin assignments
#define LED_STATUS      0b01000000  //!< Yellow status LED
#define LED_ERROR       0b00010000  //!< Red error LED
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000

#if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega8__)
#define LED_STATUS_ON       PORTD |=  LED_STATUS
#define LED_STATUS_OFF      PORTD &= ~LED_STATUS
#define LED_STATUS_TOGGLE   PORTD ^=  LED_STATUS

#define LED_ERROR_ON        PORTD |=  LED_ERROR
#define LED_ERROR_OFF       PORTD &= ~LED_ERROR
#define LED_ERROR_TOGGLE    PORTD ^=  LED_ERROR
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
