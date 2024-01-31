/**
 * @addtogroup ROOMTHERMOSTAT_ATMEGA328_PCBCONFIG
 * @brief PCB configuration of the "roomthermostat_atmega328p" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "roomthermostat_atmega328p" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// roomthermostat_atmega328p specific pin assignments
// ----------------------------------------------------------------------------

#define BUS_PCBCONFIG 1
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
#define LED_PCBCONFIG        1
#define LED_STATUS          PD6    //!< Yellow status LED
#define LED_ERROR           PD7    //!< Red error LED
#define LED_STATUS_DDR      DDRD
#define LED_ERROR_DDR       DDRD
#define LED_STATUS_PORT     PORTD
#define LED_ERROR_PORT      PORTD

#define INPUT_PCBCONFIG     1
#define INPUT_NUM_PINS      3
#define INPUT_PORT_0        PORTC
#define INPUT_PIN_0         PINC
#define INPUT_DDR_0         DDRC
#define INPUT_0             PC0
#define INPUT_PORT_1        PORTC
#define INPUT_PIN_1         PINC
#define INPUT_DDR_1         DDRC
#define INPUT_1             PC1
#define INPUT_PORT_2        PORTC
#define INPUT_PIN_2         PINC
#define INPUT_DDR_2         DDRC
#define INPUT_2             PC2
#define INPUT_PCMSK         PCMSK1
#define INPUT_PCMSK_VAL     ((1<<PCINT8) | (1<<PCINT9) | (1<<PCINT10))


#define DISP_SH1106_PCBCONFIG  1
#define DISP_DDR_RES        DDRD
#define DISP_DDR_SS         DDRD
#define DISP_DDR_A0         DDRD
#define DISP_PORT_RES       PORTD
#define DISP_PORT_SS        PORTD
#define DISP_PORT_A0        PORTD
#define DISP_RES            PD3
#define DISP_SS             PD4
#define DISP_A0             PD5

#define ZAGWIRE_PCBCONFIG   1
#define ZAGW_DDR_DATA       DDRB
#define ZAGW_PORT_DATA      PORTB
#define ZAGW_PIN_DATA       PINB
#define ZAGW_DATA           PB0

#define ZAGW_DDR_EN         DDRB    //!< Defines the DDR of a port which contains the Zagwire enable signal.
#define ZAGW_PORT_EN        PORTB   //!< Defines the PORT which contains the Zagwire enable signal.
#define ZAGW_EN             PB1     //!< Defines the Zagwire enable pin (output).

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
