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
#ifdef MESSAGING_SERIAL
  #include "serialcomm.h"
#endif
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

/**
 * @subsection MESSAGING_APPCONFIG
 * Configuration of messaging module.
 * @{
 */
#ifndef MESSAGING_APPCONFIG
  #define MESSAGING_APPCONFIG 1
  #define MESAGING_BUS        1 // default: messaging over bus
  #undef  MESAGING_SERIAL       // default: no messaging over serial
#endif // MESSAGING_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern sBus_t           g_bus;
#ifdef MESSAGING_SERIAL
  extern scomm_phy_t    *g_serphy;
#endif

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#if MESSAGING_SERIAL

inline bool message_send(uint16_t receiver, uint8_t length, const uint8_t *msg)
{
    bool retval = false;
    bool send_over_bus = true;
    bool send_over_serial = false;
    uint8_t receiver_bus_segment = (receiver & BUS_SEGBRDCSTMASK) >> 8;

    // is receiver the same module, then answer shall be routed over serial
    if (receiver == g_bus.sCfg.uOwnAddress) {
        send_over_serial = true;
        send_over_bus = false;
    }
    // broadcast? then send to bus and serial
    else if (receiver == BUS_BRDCSTADR) {
        // send_over_bus = true; already set
        send_over_serial = true;
    }
    // is receiver in other bus segment?
    else if (receiver_bus_segment != g_bus.sCfg.uOwnExtAddress) {
        // then it needs to be routed over serial line
        send_over_bus = false;
        send_over_serial = true;
    }
    else {
    }

    if (send_over_bus) {
        retval = bus_send_message(&g_bus, receiver, length, msg);
    }
    if (send_over_serial) {
        LED_ERROR_TOGGLE;

        retval |= serial_send_message(g_serphy, g_bus.sCfg.uOwnAddress, receiver, length, msg);
    }
    return retval;
}

#else

inline bool message_send(uint16_t receiver, uint8_t msglen, const uint8_t *msg)
{
    return bus_send_message(&g_bus, receiver, msglen, msg);
}

#endif
