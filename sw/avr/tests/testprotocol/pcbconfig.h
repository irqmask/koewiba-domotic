/**
 * @addtogroup TESTSCHEDULER_PCBCONFIG
 * @brief PCB configuration of the "testscheduler" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "testscheduler" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define BUS_PCBCONFIG 1
#define BUS_DDR_ENASND0     DDRD
#define BUS_PORT_ENASND0    PORTD
#define BUS_ENASND0         PD4
#undef  TXRXEN0_SEPERATE

// Pin assignments of board keys and LEDs
#define LED_PCBCONFIG       1
#define LED_STATUS          PC3  //left yellow LED
#define LED_ERROR           PC4  //right yellow LED
#define BTN_TEST            PC5
#define BTN_EXP             PC6
#define LED_STATUS_DDR      DDRC
#define LED_ERROR_DDR       DDRC
#define LED_STATUS_PORT     PORTC
#define LED_ERROR_PORT      PORTC

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
