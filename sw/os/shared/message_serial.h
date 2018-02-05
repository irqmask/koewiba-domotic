/**
 * @addtogroup MESSAGE_SERIAL
 * @brief Transmit bus-messages between KWB applications via RS232 to a gateway
 * which routes the messages to/from the bus.
 *
 * Bus messages are transmitted or received via a serial connection between the
 * application and a gateway.
 *
 * @{
 * @file    message_serial.h
 * @brief   Transmit bus-messages over a serial connection.
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
#ifndef _MESSAGE_SERIAL_H_
#define _MESSAGE_SERIAL_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#include "ioloop.h"
#include "message.h"
#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

//! Maximum size of serial message:
//! [16bit receiver] + [8bit length] + [message data] + NL + 0
//! 4 + 2 + MAX_MSG_SIZE * 2 + 1 + 1
#define MAX_SERIAL_MSG_SIZE (6 + MAX_MSG_SIZE * 2 + 2)

// --- Type definitions --------------------------------------------------------

typedef void (*msg_newconn_func_t)(char* address, uint16_t port, void* reference, void* arg);

typedef struct msg_serial {
    sys_fd_t            fd;
    ioloop_t*           ioloop;
    char                device[256];
    int                 baudrate;
    msg_incom_func_t    incomming_handler;
    void*               incomming_arg;

    // buffer for incomming messages
    char                incomming_buffer[MAX_SERIAL_MSG_SIZE*2];
    msg_t               incomming_message;
    uint8_t             incomming_state;
    uint8_t             incomming_num_received;

    // buffer for outgoing messages
    char                ser_data[MAX_SERIAL_MSG_SIZE];
    size_t              ser_data_length;
    size_t              ser_data_written;
} msg_serial_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_ser_init   (msg_serial_t*  msg_serial);

int msg_ser_open    (msg_serial_t*  msg_serial,
                     ioloop_t*      ioloop,
                     const char*    device,
                     int            baudrate);

void msg_ser_close  (msg_serial_t*  msg_serial);

void msg_ser_set_incomming_handler (msg_serial_t* msg_serial,
                                    msg_incom_func_t func,
                                    void* arg);

int msg_ser_send    (msg_serial_t*  msg_serial,
                     msg_t*         message);

int msg_ser_continue_sending (msg_serial_t* msg_serial);

bool msg_ser_is_tx_empty (msg_serial_t* msg_serial);

#ifdef __cplusplus
}
#endif

#endif /* _MESSAGE_SOCKET_H_ */
/** @} */
