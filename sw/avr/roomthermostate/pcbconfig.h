/**
 * @addtogroup ROOMTHERMOSTATE
 * @addtogroup ROOMTHERMOSTATE_PCBCONFIG
 * @brief PCB configuration of the "roomthermostate" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "roomthermostate" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// bus-module_atmega324 specific pin assignments
// ----------------------------------------------------------------------------

#define BUS_PCBCONFIG       1
#define BUS_DDR_ENASND0     DDRD
#define BUS_PORT_ENASND0    PORTD
#define BUS_ENASND0         PD4

#define SPI_PCBCONFIG       1
#define SPI_DDR_MOSI        DDRB
#define SPI_DDR_MISO        DDRB
#define SPI_DDR_SCK         DDRB
#define SPI_DDR_SS          DDRB
#define SPI_PORT_SS         PORTB
#define SPI_MOSI            PB5
#define SPI_MISO            PB6
#define SPI_SCK             PB7
#define SPI_SS              PB4 // same as EEPROM!

#define EEPROM_PCB_CONFIG   1
#define __25LC256__
#define EEP_CS_PORT         PORTB
#define EEP_CS_DDR          DDRB
#define EEP_CS              PB4

// Application specific pin assignments
// ----------------------------------------------------------------------------

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


#define GDISPLAY_PCBCONFIG  1
//! Defines the DDR of a port which contains the backlight LED signal.
#define DISP_DDR_LED   DDRD
//! Defines the PORT which contains the backlight LED signal.
#define DISP_PORT_LED  PORTD
//! Defines the LED pin.
#define DISP_LED       PD7

#define DISP_ST7565_PCBCONFIG  1
//! Defines the DDR of a port which contains the slave select signal of the
//! display.
#define DISP_DDR_SS    DDRB
//! Defines the DDR which contains the A0 signal of the display.
#define DISP_DDR_A0    DDRD
//! Defines the DDR which contains the RES signal of the display.
#define DISP_DDR_RES   DDRD
//! Defines the PORT which contains the slave select signal of the display.
#define DISP_PORT_SS   PORTB
//! Defines the PORT which contains the A0 signal of the display.
#define DISP_PORT_A0   PORTD
//! Defines the PORT which contains the RES signal of the display.
#define DISP_PORT_RES  PORTD
//! Defines the SS pin.
#define DISP_SS           PB3
//! Defines the A0 pin.
#define DISP_A0           PD6
//! Defines the reset pin
#define DISP_RES          PD5

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
/** @} */
