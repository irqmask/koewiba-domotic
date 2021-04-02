/**
 * @addtogroup TESTSCHEDULER_PCBCONFIG
 * @addtogroup PCBCONFIG
 * @brief   PCB configuration of the "testscheduler" application.
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

// bus-module_attiny1634 specific pin assignments
// ----------------------------------------------------------------------------

#define BUS_PCBCONFIG 1
#define BUS_DDR_ENASND0     DDRA
#define BUS_PORT_ENASND0    PORTA
#define BUS_ENASND0         PA6

#define SPI_PCBCONFIG       1
#define SPI_DDR_MOSI        DDRB
#define SPI_DDR_MISO        DDRB
#define SPI_DDR_SCK         DDRC
#define SPI_DDR_SS          DDRC
#define SPI_PORT_SS         PORTC
#define SPI_MOSI            PB2
#define SPI_MISO            PB1
#define SPI_SCK             PC1
#define SPI_SS              PC0 // same as EEPROM!

#define EEPROM_PCB_CONFIG   1
#define __25LC256__
#define EEP_CS_PORT         PORTC
#define EEP_CS_DDR          DDRC
#define EEP_CS              PC0

// Application specific pin assignments
// ----------------------------------------------------------------------------

// Pin assignments of board keys and LEDs
#define LED_PCBCONFIG       1
#define LED_RED       PORTA0  //!< Red LED
#define LED_YELLOW    PORTA2  //!< Yellow LED
#define LED_GREEN     PORTA4  //!< Green LED

#define LED_RED_DDR        DDRA
#define LED_RED_PORT       PORTA
#define LED_YELLOW_DDR     DDRA
#define LED_YELLOW_PORT    PORTA
#define LED_GREEN_DDR      DDRA
#define LED_GREEN_PORT     PORTA

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
