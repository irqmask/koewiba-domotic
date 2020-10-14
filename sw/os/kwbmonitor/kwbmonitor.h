/**
 * @addtogroup KWBMONITOR
 * @brief Analyze KWB messages on a byte level.
 *
 * @{
 * @file    kwbmonitor.h
 * @brief   Analyze KWB messages on a byte level.
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
#ifndef _KWBMONITOR_H_
#define _KWBMONITOR_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>
#include "systime.h"

// --- Definitions -------------------------------------------------------------

//! (netto) length of the message
#define BUS_MAXMSGLEN          64

//! length of message including header and footer
#define BUS_MAXTOTALMSGLEN      (BUS_MAXMSGLEN + 7)

#if BUS_MAXTOTALMSGLEN > 32
    #define MAX_DISPLAYLEN 32
#else
    #define BUS_MAXTOTALMSGLEN 32
#endif

// --- Type definitions --------------------------------------------------------

typedef enum {
    eMSG_NOTHING,
    eMSG_ERROR,
    eMSG_TOKEN,
    eMSG_EMPTY,
    eMSG_COMPLETE,
    eMSG_ACK
} msg_status_t;

typedef struct bus_history {
    sys_time_t      time_start;
    sys_time_t      time_last_byte;
    sys_time_t      time_curr_byte;
    uint8_t         current_msg_bytes;
    uint8_t         expected_length;
    uint16_t        expected_CRC;
    uint8_t         current_sender;
    uint8_t         message[BUS_MAXTOTALMSGLEN + 10];
    msg_status_t    last_message_status;
} bus_history_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void monitor_init(bus_history_t *history);

void monitor_parse_message(uint8_t new_byte, bus_history_t *history);

void monitor_toggle_display_empty_message(void);

void monitor_toggle_display_token_message(void);

#endif /* _KWBMONITOR_H_ */
/** @} */
