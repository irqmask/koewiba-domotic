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
#define BUS_ENASND0         PD2
#define BUS_DDR_DISRCV0     DDRD
#define BUS_PORT_DISRCV0    PORTD
#define BUS_DISRCV0         PD3
#define TXRXEN0_SEPERATE    1

#define SPI_SCK_DDR         PORTC
#define SPI_SCK_PIN         PC1

// Pin assignments of board keys and LEDs
// Port D pin assignments
#define LED_PCBCONFIG       1
#define LED_STATUS          0b01000000  //!< Yellow status LED
#define LED_ERROR           0b00010000  //!< Red error LED
#define BTN_TEST            0b00100000
#define BTN_EXP             0b10000000

#define LED_STATUS_PORT     PORTD
#define LED_ERROR_PORT      PORTD

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
