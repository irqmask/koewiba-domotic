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

#define BUS_PCBCONFIG 1
#define BUS_DDR_ENASND  DDRD
#define BUS_PORT_ENASND PORTD
#define BUS_ENASND      PD4
#define BUS_TXRX_COMBINED 1
#define LED_ERROR_ON
#define LED_ERROR_OFF


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

#define UART_APPCONFIG 1
#define  UART_WITH_BLOCKING 1
#define UART_WITH_IRQ          1
#define UART_RECV_QUEUE_SIZE   4  //!< Size of the UART recv-queue.
#define UART_SEND_QUEUE_SIZE   4  //!< Size of the UART send-queue.
#undef  UART_AUTOCR                //!< Don't send a CR when a newline

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
