/*
 * kwbkouter - A router for koewiba-domotic messages.
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
 *
 */

#include "rconnserial.h"

// --- Local functions ---------------------------------------------------------

static void incommingMessageHdl(msg_t* message, void* reference, void* arg)
{
    RConnSerial* sercon = (RConnSerial*)arg;
    sercon->OnIncommingMessage(message);
}

// --- Class member functions --------------------------------------------------

RConnSerial::RConnSerial() : RouteConnection()
{
    msg_ser_init(&this->serial);
}

RConnSerial::RConnSerial(ioloop_t* ioloop)
{
    RConnSerial();
    this->ioloop = ioloop;
}

RConnSerial::~RConnSerial()
{
    msg_ser_close(&this->serial);
}

int RConnSerial::Open(const char* device, int baudrate)
{
    int retval = 0;

    retval = msg_ser_open(&this->serial, this->ioloop, device, baudrate);
    msg_ser_set_incomming_handler(&this->serial, incommingMessageHdl, this);
    return retval;
}

void RConnSerial::Close()
{
    msg_ser_close(&this->serial);
}

int RConnSerial::Send(msg_t* message)
{
    return msg_ser_send(&this->serial, message);
}

void RConnSerial::OnIncommingMessage(msg_t* message)
{
    if (this->extOnIncommingMsg != NULL) {
        this->extOnIncommingMsg(message, this, this->extOnIncommingMsgArg);
    }
}

