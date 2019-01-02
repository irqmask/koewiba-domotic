/**
 * @addtogroup TEST_BUS-MODULE_ATMEGA328_PCBCONFIG
 * @brief PCB configuration of the "test_bus-module_atmega328" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "test_bus-module_atmega328" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// bus-module_atmega328 specific pin assignments
// ----------------------------------------------------------------------------

#define BUS_PCBCONFIG       1
#define BUS_DDR_ENASND0     DDRD
#define BUS_PORT_ENASND0    PORTD
#define BUS_ENASND0         PD2

#define SPI_PCBCONFIG       1
#define SPI_DDR_MOSI        DDRB
#define SPI_DDR_MISO        DDRB
#define SPI_DDR_SCK         DDRB
#define SPI_DDR_SS          DDRB
#define SPI_PORT_SS         PORTB
#define SPI_MOSI            PB3
#define SPI_MISO            PB4
#define SPI_SCK             PB5
#define SPI_SS              PB2 // same as EEPROM!

#define EEPROM_PCB_CONFIG   1
#define __25LC256__
#define EEP_CS_PORT         PORTB
#define EEP_CS_DDR          DDRB
#define EEP_CS              PB2

// Application specific pin assignments
// ----------------------------------------------------------------------------

// debug LED outputs
#define LED_PCBCONFIG       1
#define LED_STATUS          PD6    //!< Yellow status LED
#define LED_ERROR           PD7    //!< Red error LED
#define LED_STATUS_DDR      DDRD
#define LED_ERROR_DDR       DDRD
#define LED_STATUS_PORT     PORTD
#define LED_ERROR_PORT      PORTD

#define ZAGWIRE_PCBCONFIG   1
#define ZAGW_DDR_DATA       DDRD
#define ZAGW_PORT_DATA      PORTD
#define ZAGW_PIN_DATA       PIND
#define ZAGW_DATA           PD5

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
