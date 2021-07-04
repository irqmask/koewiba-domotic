/**
 * @addtogroup BUS_GATEWAY
 * @addtogroup PCBCONFIG
 * @brief PCB configuration of the "testgateway" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "testgateway" application.
 *
 * @preliminary     for using the pcb in bus-mode the following connections have to be established:
 *                  - connect CS (PB4) with PD0
 *                  - connect HS (PB3) with PD1
 *                  - connect serial interfaces TX line with PD2
 *                  - connect serial interfaces RX line with PD3
 *                  - connect serial interfaces CTS line with PD5
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
#define LED_STATUS          PB0  // left green LED
#define LED_ERROR           PB1  // right red LED
#define LED_STATUS_DDR      DDRB
#define LED_ERROR_DDR       DDRB
#define LED_STATUS_PORT     PORTB
#define LED_ERROR_PORT      PORTB


#define SCOMM_PORTOUT       PORTD
#define SCOMM_PORTIN        PIND
#define SCOMM_CTS           PD5

// Inputs
#define INPUT_PCBCONFIG     1

#define INPUT_NUM_PINS      4
#define INPUT_PORT_0        PORTA
#define INPUT_PIN_0         PINA
#define INPUT_DDR_0         DDRA
#define INPUT_0             PA0
#define INPUT_PORT_1        PORTA
#define INPUT_PIN_1         PINA
#define INPUT_DDR_1         DDRA
#define INPUT_1             PA1
#define INPUT_PORT_2        PORTA
#define INPUT_PIN_2         PINA
#define INPUT_DDR_2         DDRA
#define INPUT_2             PA2
#define INPUT_PORT_3        PORTA
#define INPUT_PIN_3         PINA
#define INPUT_DDR_3         DDRA
#define INPUT_3             PA3


// WakeUpPins
#define SLEEP_WakeupPinsPortA   (0<<PCINT7)|(0<<PCINT6)|(0<<PCINT5)|(0<<PCINT4)|(1<<PCINT3)|(1<<PCINT2)|(1<<PCINT1)|(1<<PCINT0)
#define SLEEP_WakeupPinsPortB   0
#define SLEEP_WakeupPinsPortC   0
#define SLEEP_PinChangeIR_Enable()   GIMSK |=  ((1<<PCIE2)|(1<<PCIE1)|(1<<PCIE0));
#define SLEEP_PinChangeIR_Disable()  GIMSK &= ~((1<<PCIE2)|(1<<PCIE1)|(1<<PCIE0));

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
