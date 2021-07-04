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
 * Copyright (C) 2019  christian <irqmask@web.de>
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

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

bool bgw_send_serial_msg(scomm_phy_t *serial,
                         uint16_t sender, uint16_t receiver,
                         uint8_t len, uint8_t* msg);

void bgw_forward_serial_msg (sBus_t* bus, scomm_phy_t* phy);

bool bgw_forward_bus_msg (sBus_t *bus, scomm_phy_t *serial,
                          uint16_t* sender, uint8_t* len, uint8_t* msg);

#endif /* BUS_GATEWAY_H_ */
