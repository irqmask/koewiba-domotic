/**
 * @addtogroup PWM-DIMMER-3CHN
 * @addtogroup PWM-DIMMER-3CHN_PCBCONFIG
 * @brief PCB configuration of the "pwm-dimmer-3chn" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "pwm-dimmer-3chn" application.
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
#define BUS_ENASND      PD4
#define BUS_TXRX_COMBINED 1
#define LED_ERROR_ON
#define LED_ERROR_OFF

#define SPI_PCBCONFIG  1
#define SPI_DDR_MOSI   DDRB
#define SPI_DDR_MISO   DDRB
#define SPI_DDR_SCK    DDRB
#define SPI_DDR_SS     DDRC
#define SPI_PORT_SS    PORTC
#define SPI_MOSI       PB3
#define SPI_MISO       PB4
#define SPI_SCK        PB5
#define SPI_SS         PC3

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
