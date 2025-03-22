/**
 * @addtogroup CONTATCS
 *
 * @{
 * @file    contacts.c
 * @brief   Monitor input contact e.g. of a window.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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

#include "contacts.h"

#include <stdint.h>

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif
#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

// include
#include "cmddef_common.h"
// shared
#include "bus.h"
#include "prjtypes.h"
// avr/shared
#include "input.h"
#include "messaging.h"

// --- Definitions -------------------------------------------------------------

#define CONTACT0    0   //!< Contact 0 index
#define CONTACT1    1   //!< Contact 1 index

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Increase month by one.
 */
static void send_contact_value(uint8_t index, uint8_t value)
{
    uint8_t msg[4];

    if (index >= 2) return;
    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_Contact0State + index;
    msg[2] = value;
    message_send(BUS_BRDCSTADR, 3, msg);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize contacts module
 */
void contacts_initialize(void)
{
    input_initialize();
}

/**
 * Monitor contact inputs
 */
void contacts_background(void)
{
    input_background();
    uint8_t toggled = input_toggled();
    uint8_t state = input_state();
    if (toggled & (1<<CONTACT0)) {
        send_contact_value(0, (state & (1<<CONTACT0)) != 0);
    }
    if (toggled & (1<<CONTACT1)) {
        send_contact_value(1, (state & (1<<CONTACT1)) != 0);
    }
}

/** @} */
