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

//! Maximum length of message to be displayed
#if BUS_MAXTOTALMSGLEN > 32
    #define MAX_DISPLAYLEN 32
#else
    #define MAX_DISPLAYLEN BUS_MAXTOTALMSGLEN
#endif

// --- Type definitions --------------------------------------------------------

//! Type and status of the message
typedef enum {
    //! Type and status not yet set or unknown
    eMSG_NOTHING,
    //! Error occurred, message not parsable
    eMSG_ERROR,
    //! Message is a token
    eMSG_TOKEN,
    //! Message is an empty message
    eMSG_EMPTY,
    //! Message is complete with payload
    eMSG_COMPLETE,
    //! Message is an acknowledge message
    eMSG_ACK
} msg_status_t;

/**
 * Structure of last received bytes since last complete or aborted message.
 */
typedef struct bus_history {
    //! Timestamp of first received byte
    sys_time_t      time_start;
    //! Timestamp of last received byte (before current)
    sys_time_t      time_last_byte;
    //! Timestamp of current received byte
    sys_time_t      time_curr_byte;
    //! Total number of received bytes for this message
    uint8_t         current_msg_bytes;
    //! Expected length of currently received message
    uint8_t         expected_length;
    //! Expected checksum of this message
    uint16_t        expected_CRC;
    //! Current sender of the message
    uint8_t         current_sender;
    //! complete message header + payload + crc
    uint8_t         message[BUS_MAXTOTALMSGLEN + 10];
    //! status and type of message
    msg_status_t    last_message_status;
} bus_history_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize monitoring.
 *
 * @param[in]   history     History of current messages. Structure members will
 *                          be initialized.
 */
void monitor_init(bus_history_t *history);

/**
 * On incoming byte from the bus, add it to the history and check if the
 * message is complete.
 *
 * @param[in]   new_byte    Newly received byte
 * @param[in]   history     History of current message.
 */
void monitor_parse_message(uint8_t new_byte, bus_history_t *history);

/**
 * Toggle displaying empty messages.
 */
void monitor_toggle_display_empty_message(void);

/**
 * Toggle displaying token messages.
 */
void monitor_toggle_display_token_message(void);

#endif /* _KWBMONITOR_H_ */
/** @} */
