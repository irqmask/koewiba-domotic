/**
 * @addtogroup UART
 * @brief UART driver.
 *
 * Contains functions to send and receive data over the UART interface.
 *
 * Supports appconfig.h for individual settings depending on the application.
 * Supports pcbconfig.h to choose which UART of the controller is used. If the
 * controller supports only one uart, uart0 is used as default, otherwise the
 * second uart is used as default.
 * @todo set uart controller default registers for pcbconfig.h.
 * @{
 * @file    uart.h
 * @brief   UART driver.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _UART_H_
#define _UART_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

#if defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__) || defined (__AVR_ATtiny1634__)
 #define HAS_UART1              1
#else
 #undef HAS_UART1
#endif

/**
 * @subsection UART_PCBCONFIG
 * Configure UART pins and registers.
 * @{
 */
#ifndef UART_PCBCONFIG
 #define UART_PCBCONFIG         1
#endif // UART_PCBCONFIG
/** @} */

/**
 * @subsection UART_APPCONFIG
 * Configure size of the UART driver. E.g. only blocking functions, queue size.
 * @{
 */
#ifndef UART_APPCONFIG
 #define UART_APPCONFIG 1
 #define UART_WITH_BLOCKING     1
 #undef UART_WITH_IRQ

// Use UART1 in case of ATmega324 or ATtiny1634, otherwise use UART0
 #ifdef HAS_UART1
  #ifdef __AVR_ATtiny1634__
   #define UART_DEFAULT_PORT    0
  #else
   #define UART_DEFAULT_PORT    1
  #endif
 #else
  #define UART_DEFAULT_PORT     0
 #endif

 #define UART_RECV_QUEUE_SIZE   32  //!< Size of the UART recv-queue.
 #define UART_SEND_QUEUE_SIZE   32  //!< Size of the UART send-queue.
 #define UART_AUTOCR            1   //!< Automatically send a CR when a newline
                                    //!< has been sent.
#endif // UART_APPCONFIG

//TODO CV: enable "UART_WITH_IRQ" when uart_with_irq_is_functional_again
#undef UART_WITH_IRQ

/** @} */

#if UART_DEFAULT_PORT == 0
 #define uart_init_blk       uart_init_blk0
 #define uart_put_char_blk   uart_put_char_blk0
 #define uart_put_hex8_blk   uart_put_hex8_blk0
 #define uart_put_string_blk uart_put_string_blk0
 #define uart_hex_dump_blk   uart_hex_dump_blk0
 #define uart_get_char_blk   uart_get_char_blk0
#else
 #define uart_init_blk       uart_init_blk1
 #define uart_put_char_blk   uart_put_char_blk1
 #define uart_put_hex8_blk   uart_put_hex8_blk1
 #define uart_put_string_blk uart_put_string_blk1
 #define uart_hex_dump_blk   uart_hex_dump_blk1
 #define uart_get_char_blk   uart_get_char_blk1
#endif

// --- Type definitions --------------------------------------------------------

//! UART status flags, which are included in the receive answer.
typedef enum {
    eUartFlag_NoData = 8,       //!< Receive buffer is empty
    eUartFlag_BufOverrun,       //!< Receive buffer is full, at least one byte
                                //!< has been lost.
    eUartFlag_FramingError,     //!< A framing error occurred, when a byte has
                                //!< been received.
    eUartFlag_Last
} eUartFlags_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            uart_init           (uint32_t               baudrate);

bool            uart_is_busy        (void);

void            uart_transmit       (uint8_t*               data,
                                     uint8_t                length);

void            uart_put_char       (char                   single_char);

void            uart_put_string     (const char*            string);

void            uart_put_hex8       (uint8_t                value);

void            uart_hex_dump       (const uint8_t*         data,
                                     uint8_t                length);

uint16_t        uart_get_char       (void);

// UART functions for UART0 (blocking)
void            uart_init_blk0      (uint32_t               baudrate);

void            uart_put_char_blk0  (char                   single_char);

void            uart_put_hex8_blk0  (uint8_t                value);

void            uart_put_string_blk0(const char*            string);

void            uart_hex_dump_blk0  (const uint8_t*         data,
                                     uint8_t                length);

char            uart_get_char_blk0  (void);

#ifdef HAS_UART1
// UART functions for UART1 (blocking)
void            uart_init_blk1      (uint32_t               baudrate);

void            uart_put_char_blk1  (char                   single_char);

void            uart_put_hex8_blk1  (uint8_t                value);
void            uart_put_hex16_blk1 (uint16_t               value);

void            uart_put_string_blk1(const char*            string);
void            uart_put_string_blk1_p(const char*          string);

void            uart_hex_dump_blk1  (const uint8_t*         data,
                                     uint8_t                length);

char            uart_get_char_blk1  (void);

bool            uart_is_rx_pending1 (void);
#endif

#endif // _UART_H_
/** @} */
