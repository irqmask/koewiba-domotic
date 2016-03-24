/**
 * @addtogroup ELEKTOR_BUS-NODE
 * @addtogroup PCBCONFIG
 * @brief PCB configuration of the "elektor_bus-node" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "elektor_bus-node" application.
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
//#undef TXRXEN0_SEPERATE

// Pin assignments of board keys and LEDs
#define LED_PCBCONFIG       1
#define LED_STATUS          PD6  // yellow LED
#define LED_ERROR           PD4  // red LED
#define BTN_TEST            PD5
#define BTN_EXP             PD7
#define LED_STATUS_DDR      DDRD
#define LED_ERROR_DDR       DDRD
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
