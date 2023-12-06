/**
 * @addtogroup TASTER8_PCBCONFIG
 * @brief PCB configuration of the taster8 application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the taster8 application.
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

#define SPI_PCBCONFIG  1
//! Defines the DDR of a port which contains the MOSI signal.
#define SPI_DDR_MOSI   DDRB
//! Defines the DDR of a port which contains the MISO signal.
#define SPI_DDR_MISO   DDRB
//! Defines the DDR of a port which contains the SCK signal.
#define SPI_DDR_SCK    DDRB
//! Defines the DDR of a port which contains the standard slave select signal.
#define SPI_DDR_SS     DDRB
//! Defines the PORT which contains the standard slave select signal.
#define SPI_PORT_SS    PORTB
//! Defines the MOSI pin.
#define SPI_MOSI       PB3
//! Defines the MISO pin.
#define SPI_MISO       PB4
//! Defines the SCK pin.
#define SPI_SCK        PB5
//! Defines the SS (slave select) pin.
#define SPI_SS         PB2

// Pin assignments of board keys and LEDs
#define LEDSKEYS_PCBCONFIG  1
#define KEY0_DDR            DDRD
#define KEY0_PORT           PORTD
#define KEY0_IPORT          PIND
#define KEY0_PIN            PD3
#define KEY1_DDR            DDRC
#define KEY1_PORT           PORTC
#define KEY1_IPORT          PINC
#define KEY1_PIN            PC4
#define KEY2_DDR            DDRC
#define KEY2_PORT           PORTC
#define KEY2_IPORT          PINC
#define KEY2_PIN            PC2
#define KEY3_DDR            DDRC
#define KEY3_PORT           PORTC
#define KEY3_IPORT          PINC
#define KEY3_PIN            PC0
#define KEY4_DDR            DDRD
#define KEY4_PORT           PORTD
#define KEY4_IPORT          PIND
#define KEY4_PIN            PD4
#define KEY5_DDR            DDRC
#define KEY5_PORT           PORTC
#define KEY5_IPORT          PINC
#define KEY5_PIN            PC5
#define KEY6_DDR            DDRC
#define KEY6_PORT           PORTC
#define KEY6_IPORT          PINC
#define KEY6_PIN            PC3
#define KEY7_DDR            DDRC
#define KEY7_PORT           PORTC
#define KEY7_IPORT          PINC
#define KEY7_PIN            PC1

#define KEY0_WAKEUP         PCINT19 // PD3
#define KEY0_WAKEUP_REG     PCMSK2
#define KEY1_WAKEUP         PCINT12 // PC4
#define KEY1_WAKEUP_REG     PCMSK1
#define KEY2_WAKEUP         PCINT10 // PC2
#define KEY2_WAKEUP_REG     PCMSK1
#define KEY3_WAKEUP         PCINT8  // PC0
#define KEY3_WAKEUP_REG     PCMSK1
#define KEY4_WAKEUP         PCINT20 // PD4
#define KEY4_WAKEUP_REG     PCMSK2
#define KEY5_WAKEUP         PCINT13 // PC5
#define KEY5_WAKEUP_REG     PCMSK1
#define KEY6_WAKEUP         PCINT11 // PC3
#define KEY6_WAKEUP_REG     PCMSK1
#define KEY7_WAKEUP         PCINT9  // PC1
#define KEY7_WAKEUP_REG     PCMSK1


#define LED_OE_DDR          DDRD
#define LED_OE_PORT         PORTD
#define LED_OE_PIN          PD6
#define LED_STB_DDR         DDRD
#define LED_STB_PORT        PORTD
#define LED_STB_PIN         PD5
#define LED0                6
#define LED1                5
#define LED2                0
#define LED3                4
#define LED4                7
#define LED5                3
#define LED6                2
#define LED7                1

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
