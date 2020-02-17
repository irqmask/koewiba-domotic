/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    rconnserial.cpp
 * @brief   Implementation of a route over a serial connection.
 *
 * This implements opening and closing as well as sending and receiving messages 
 * over a serial connection.
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

#include <stdio.h>
#include <string.h>

#include "log.h"

#include "rconnserial.h"

// --- Local functions ---------------------------------------------------------

/**
 * Callback handler for incomming messages.
 */
static void incommingMessageHdl(msg_t* message, void* reference, void* arg)
{
    RConnSerial* sercon = (RConnSerial*)arg;
    sercon->OnIncomingMessage(message, reference);
}

// --- Class member functions --------------------------------------------------

/**
 * Constructs a serial router connection object with usage of the ioloop
 * background routine.
 *
 * @param[in]   ioloop      Pointer to existing ioloop.
 */
RConnSerial::RConnSerial(ioloop_t* ioloop)
{
    msg_ser_init(&this->serial);
    memset(this->name, 0, sizeof(name));
    snprintf(this->name, sizeof(name) - 1, "SER");
    this->ioloop = ioloop;
}

/**
 * The destructor closes serial connection if still open.
 */
RConnSerial::~RConnSerial()
{
    msg_ser_close(&this->serial);
}

/**
 * Constructs a serial router connection object with usage of the ioloop
 * background routine.
 *
 * @param[in]   device      Path and filename of serial device e.g. /dev/ttyS0.
 * @param[in]   baudrate    Baudrate of serial connection.
 * 
 * @returns 0 when successful, otherwise error code.
 */
int RConnSerial::Open(const char* device, int baudrate)
{
    int retval = 0;

    retval = msg_ser_open(&this->serial, this->ioloop, device, baudrate);
    msg_ser_set_incomming_handler(&this->serial, incommingMessageHdl, this);
    return retval;
}

/**
 * Close the serial connection.
 */
void RConnSerial::Close()
{
    msg_ser_close(&this->serial);
}

//----------------------------------------------------------------------------
int RConnSerial::Send(msg_t* message)
{
    log_msg(LOG_VERBOSE1, "SERIAL %21s <-- message sent", this->GetName());
    return msg_ser_send(&this->serial, message);
}

//----------------------------------------------------------------------------
bool RConnSerial::IsSender(void *reference)
{
    return ((msg_serial_t*)reference == &this->serial);
}

//----------------------------------------------------------------------------
void RConnSerial::OnIncomingMessage(msg_t* message, void* reference)
{
    log_msg(LOG_VERBOSE1, "SERIAL %21s --> message received", this->GetName());
    RouteConnection::OnIncomingMessage(message, reference);
}

/** @} */
