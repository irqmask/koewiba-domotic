/**
 * @addtogroup TESTUART_APPCONFIG
 * @brief Application configuration of testuart application.
 *
 * @{
 * @file    appconfig.h
 * @brief   Application configuration of testuart application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------


#define UART_APPCONFIG 1
#define  UART_WITH_BLOCKING 1
#define UART_WITH_IRQ          1
#define UART_RECV_QUEUE_SIZE   64  //!< Size of the UART recv-queue.
#define UART_SEND_QUEUE_SIZE   20  //!< Size of the UART send-queue.
#undef  UART_AUTOCR                //!< Don't send a CR when a newline 

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _APPCONFIG_H_
/** @} */
