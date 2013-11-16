/**
 * @addtogroup UART
 * @brief UART driver.
 *
 * Contains functions to send and receive data over the UART interface.
 *
 * Supports appconfig.h for individual settings depending on the application.
 *
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
 #undef  UART_WITH_BLOCKING
 #define UART_WITH_IRQ          1

 #define UART_RECV_QUEUE_SIZE   32  //!< Size of the UART recv-queue.
 #define UART_SEND_QUEUE_SIZE   32  //!< Size of the UART send-queue.
 #define UART_AUTOCR            1   //!< Automatically send a CR when a newline 
                                    //!< has been sent.
#endif // UART_APPCONFIG
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

void            UART_vInit          (uint32_t               uBaud);

uint8_t         UART_uSendQueueSpace(void);

BOOL            UART_bIsBusy        (void);

void            UART_vTransmit      (uint8_t*               puSendBuf,
                                     uint8_t                uLength);

uint16_t        UART_uReceive       (void);

void            UART_vPutChar       (char                   cChar);

void            UART_vPutString     (const char*            pcString);

void            UART_vPutHex        (uint8_t                uValue);

void            UART_vHexDump       (const uint8_t*         puArray,
                                     uint8_t                uSize);


void            UART_vInitBlk       (uint32_t               uBaud);

uint16_t        UART_uReceiveBlk    (void);

void            UART_vPutCharBlk    (char                   cChar);

void            UART_vPutHexBlk     (uint8_t                uValue);

void            UART_vPutStringBlk  (const char*            pcChar);

void            UART_vHexDumpBlk    (const uint8_t*         puArray,
                                     uint8_t                uSize);

#endif // _UART_H_
/** @} */
