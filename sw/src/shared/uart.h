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

/**
 * @subsection UART_PCBCONFIG
 * Configure UART pins and registers.
 * @{
 */
#ifndef UART_PCBCONFIG
 #define UART_PCBCONFIG  1
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

 #define UART_RECV_QUEUE_SIZE   32  //!< Size of the UART recv-queue.
 #define UART_SEND_QUEUE_SIZE   32  //!< Size of the UART send-queue.
 #define UART_AUTOCR            1   //!< Automatically send a CR when a newline
                                    //!< has been sent.
#endif // UART_APPCONFIG

//TODO CV: enable "UART_WITH_IRQ" when uart_with_irq_is_functional_again
#undef UART_WITH_IRQ

/** @} */

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

//TODO CV: remove? uint8_t         uart_space_in_send_q(void);

BOOL            uart_is_busy        (void);

void            uart_transmit       (uint8_t*               data,
                                     uint8_t                length);

void            uart_put_char       (char                   single_char);

void            uart_put_string     (const char*            string);

void            uart_put_hex8       (uint8_t                value);

void            uart_hex_dump       (const uint8_t*         data,
                                     uint8_t                length);

uint16_t        uart_get_char       (void);


void            uart_init_blk       (uint32_t               baudrate);

void            uart_put_char_blk   (char                   single_char);

void            uart_put_hex8_blk   (uint8_t                value);

void            uart_put_string_blk (const char*            string);

void            uart_hex_dump_blk   (const uint8_t*         data,
                                     uint8_t                length);

char            uart_get_char_blk   (void);

#endif // _UART_H_
/** @} */
