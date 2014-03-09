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
//#define LED_ERROR_ON
//#define LED_ERROR_OFF

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

#define PWM_PCBCONFIG      1
#define PWM_CHN0_PORT      PORTC
#define PWM_CHN1_PORT      PORTC
#define PWM_CHN2_PORT      PORTC

#define PWM_CHN0_DDR       DDRC
#define PWM_CHN1_DDR       DDRC
#define PWM_CHN2_DDR       DDRC

#define PWM_CHN0_PIN       0
#define PWM_CHN1_PIN       1
#define PWM_CHN2_PIN       2

#define PWMCOUNT           REG_TIMER1_TCNT
#define PWMCOMPARE         REG_TIMER1_OCRA
#define PWM_INTERRUPT      ISR(INTERRUPT_TIMER1_COMPA)

// Port D pin assignments
#define LED_STATUS      0b01000000  //!< Yellow status LED
#define LED_ERROR       0b00010000  //!< Red error LED

#if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88P__) || defined (__AVR_ATmega8__)
#define LED_STATUS_ON       PORTD |=  LED_STATUS
#define LED_STATUS_OFF      PORTD &= ~LED_STATUS
#define LED_STATUS_TOGGLE   PORTD ^=  LED_STATUS

#define LED_ERROR_ON        PORTD |=  LED_ERROR
#define LED_ERROR_OFF       PORTD &= ~LED_ERROR
#define LED_ERROR_TOGGLE    PORTD ^=  LED_ERROR
#else
#define LED_STATUS_ON
#define LED_STATUS_OFF
#define LED_STATUS_TOGGLE

#define LED_ERROR_ON
#define LED_ERROR_OFF
#define LED_ERROR_TOGGLE
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
