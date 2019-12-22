/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    rconnserial.h
 * @brief   Declaration of a route over a serial connection.
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
#ifndef RCONNSERIAL_H
#define RCONNSERIAL_H

#include "routeconnection.h"
#include "message_serial.h"

/**
 * This class implements a route for KWB messages over a serial line.
 */
class RConnSerial : public RouteConnection
{
private:
    msg_serial_t serial;    //!< Concrete serial connetion.

public:
    RConnSerial();
    RConnSerial(ioloop_t* ioloop);
    ~RConnSerial();

    int Open(const char* device, int baudrate);
    void Close();

    int Send(msg_t* message);
    void OnIncomingMessage(msg_t* message);
};

#endif // RCONNSERIAL_H
/** @} */
