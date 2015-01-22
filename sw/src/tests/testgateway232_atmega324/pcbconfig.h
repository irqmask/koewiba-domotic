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

// KoeWib-Bus
#define BUS_PCBCONFIG 1

//#define TXRXEN1_SEPERATE 0

#define BUS_DDR_ENASND0  DDRB
#define BUS_DDR_ENASND1  DDRB
#define BUS_DDR_DISRCV0  DDRB
#define BUS_DDR_DISRCV1  DDRB
#define BUS_PORT_ENASND0 PORTB
#define BUS_PORT_DISRCV0 PORTB
//#define BUS_PORT_ENASND1 PORTB
//#define BUS_PORT_DISRCV1 PORTB
#define BUS_ENASND0      PB2
//#define BUS_ENASND1      PA5
#define BUS_DISRCV0      PB2
//#define BUS_DISRCV1      PA2

#define LED_PCBCONFIG 1
// Outputs (LEDs)
#define LED_ERROR           0b00100000  //!< Red error LED on PA5
#define LED_ERROR_ON        PORTA |=  LED_ERROR
#define LED_ERROR_OFF       PORTA &= ~LED_ERROR
#define LED_ERROR_TOGGLE    PORTA ^=  LED_ERROR

#define SCOMM_PORTOUT       PORTB
#define SCOMM_PORTIN        PINB
#define SCOMM_CTS           PB0

// Inputs (Buttons)

// WakeUpPins
#define SLEEP_WakeupPinsPortA   (0<<PCINT7)|(0<<PCINT6)|(0<<PCINT5)|(0<<PCINT4)|(1<<PCINT3)|(1<<PCINT2)|(1<<PCINT1)|(1<<PCINT0)
#define SLEEP_WakeupPinsPortB   0
#define SLEEP_WakeupPinsPortC   0
#define SLEEP_PinChangeIR_Enable()   GIMSK |=  ((1<<PCIE2)|(1<<PCIE1)|(1<<PCIE0));
#define SLEEP_PinChangeIR_Disable()  GIMSK &= ~((1<<PCIE2)|(1<<PCIE1)|(1<<PCIE0));

// SPI
#define SPI_SCK_DDR     PORTB
#define SPI_SCK_PIN     PB7

// EEPROM
#define EEPROM_PCB_CONFIG   1

#define EEP_CS_PORT  PORTB
#define EEP_CS_DDR   DDRB
#define EEP_CS       PB4
#define REG_SPI_DATA SPDR
#define REG_SPSR0    SPSR


// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
