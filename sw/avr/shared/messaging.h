/**
 * @addtogroup MESSAGE
 * @brief A module to send and receive messages over bus and other media.
 * 
 * This message module abstacts the physical interface from the application.
 * This is needed especially for devices like gateway modules which conver messages
 * e.g. from bus to serial line and vice versa, while als beeing able to
 * accept and send messages on its own. 
 *
 * @{
 * @file    messaging.h
 * @brief   A module to send and receive messages over bus and other media.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@web.de>
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
#ifndef _MESSAGING_H_
#define _MESSAGING_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

bool message_send(uint16_t receiver, uint8_t msglen, const uint8_t *msg);

#endif // _MESSAGING_H_
/** @} */

