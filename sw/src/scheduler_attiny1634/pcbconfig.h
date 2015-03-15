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

#define TXRXEN1_SEPERATE 1

#define BUS_DDR_ENASND0  DDRA
#define BUS_DDR_ENASND1  DDRA
#define BUS_DDR_DISRCV0  DDRA
#define BUS_DDR_DISRCV1  DDRA
#define BUS_PORT_ENASND0 PORTA
#define BUS_PORT_ENASND1 PORTA
#define BUS_PORT_DISRCV0 PORTA
#define BUS_PORT_DISRCV1 PORTA
#define BUS_ENASND0      PA6
#define BUS_ENASND1      PA5
#define BUS_DISRCV0      PA6
#define BUS_DISRCV1      PA2

// Pin assignments of board keys and LEDs
// Port D pin assignments
#define LED_PCBCONFIG       1
// not used #define LED_STATUS          0b01000000  //!< Yellow status LED
#define LED_ERROR           0b00000001  //!< Red error LED

#define LED_STATUS_PORT     PORTA
#define LED_ERROR_PORT      PORTA

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
