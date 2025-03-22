/**
 * @addtogroup MESSAGE
 *
 * @{
 * @file    messaging.c
 * @brief   A module to send and receive messages over bus and other media.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025 christian <irqmask@web.de>
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

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "messaging.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#include "bus.h"

// --- Definitions -------------------------------------------------------------

/**
 * @subsection ALARMCLOCK_APPCONFIG
 * Configuration of alarmclock module.
 * @{
 */
#ifndef MESSAGE_APPCONFIG
  #define MESSAGE_SOMETHING 42
#endif // ALARMCLOCK_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern sBus_t  g_bus;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

inline bool message_send(uint16_t receiver, uint8_t msglen, const uint8_t *msg)
{
    return bus_send_message(&g_bus, receiver, msglen, msg);
}
