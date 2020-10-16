/**
 * @addtogroup MESSAGE
 * @brief Common datatypes for KoeWiBa messages.
 *
 * @{
 * @file    message.h
 * @brief   Common datatypes for KoeWiBa messages.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#include "bus.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

#if (BUS_MAXSENDMSGLEN > BUS_MAXRECVMSGLEN)
#define MAX_MSG_SIZE   BUS_MAXSENDMSGLEN
#else
#define MAX_MSG_SIZE   BUS_MAXRECVMSGLEN
#endif

// --- Type definitions --------------------------------------------------------

/**
 * Error codes regarding message handling
 */
typedef enum msg_error {
    eMSG_ERR_SIZE = 500,    //!< Size of message exceeds limits
    eMSG_ERR_BUSY,          //!< send or receive buffer full
} msg_error_t;

/**
 * Structure holding a complete message.
 */
typedef struct msg {
    uint16_t    sender;     //!< Sender node-id of the message
    uint16_t    receiver;   //!< Receiver node-id of the message
    uint8_t     length;     //!< Length of the message in bytes
    uint8_t     data[MAX_MSG_SIZE]; //!< message payload
    uint16_t    crc;        //!< Checksum of the message
} msg_t;

//! Function pointer type for incoming messages
typedef void (*msg_incom_func_t)(msg_t *message, void *reference, void *arg);
//! Function pointer type for closed connections
typedef void (*msg_conn_func_t)(const char *address, uint16_t port, void *reference, void *arg);

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * @param[in] message       Message to convert.
 * @param[in] max_length    Maximum number of data bytes to convert to string.
 * @returns Converted message as string.
 * @note This function is not thread-safe!
 */
const char *msg_to_string(const msg_t *message, uint8_t max_length);

/**
 * @param[in] keyword       Keyword to add to log message.
 * @param[in] message       Message to log.
 */
void msg_log(const char *keyword, const msg_t *message);

#ifdef __cplusplus
}
#endif

#endif /* _MESSAGE_H_ */
/** @} */
