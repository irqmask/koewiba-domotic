/**
 * @addtogroup BUS_GATEWAY
 * @brief Gateway to transmit messages from bus to serial line and vice versa.
 *
 * @{
 * @file    bus_gateway.c
 * @brief   Gateway to transmit messages from bus to serial line and vice versa.

 * @author  Robert Mueller, Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@web.de>
 *                     robert <djrfm@gmx.net>
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
#ifndef BUS_GATEWAY_H_
#define BUS_GATEWAY_H_

// --- Include section ---------------------------------------------------------

#include "bus.h"
#include "serialcomm.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum bgw_state {
    eBGW_IDLE = 0,
    eBGW_WAIT_COMPLETE,
    eBGW_RECV,
    eBGW_ERROR
} bgw_state_t;

/**
 * Structure carrying serial message runtime data
 */
typedef struct serial_msg {
    bgw_state_t curr_state;
    uint8_t     curr_recv_h;
    uint8_t     curr_recv_l;
    uint8_t     curr_send_h;
    uint8_t     curr_send_l;
    uint8_t     curr_len;
    scomm_phy_t serphy;
} serial_msg_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void bgw_initialize(serial_msg_t *serial);

bool bgw_get_serial_msg(serial_msg_t *serial);

bool bgw_forward_serial_msg(sBus_t *bus, serial_msg_t *serial,
                            uint16_t *sender, uint8_t *length, uint8_t *msg);

bool bgw_forward_bus_msg(sBus_t *bus, serial_msg_t *serial,
                         uint16_t *sender, uint8_t *length, uint8_t *msg);

#endif /* BUS_GATEWAY_H_ */
