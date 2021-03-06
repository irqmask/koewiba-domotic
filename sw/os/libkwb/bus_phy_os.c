/**
 * @addtogroup BUS
 * @{
 * @addtogroup BUS_PHY_OS
 * @{
 *
 * @brief Physical layer of bus protocol for operating systems like Linux and
 * Windows.
 * @file    bus_phy_os.c
 * @brief   Physical layer of bus protocol.
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

// --- Include section ---------------------------------------------------------

#include <stdio.h>

#include "bus_intern.h"
#include "message_bus.h"
#include "vos.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void debug_log_hex(const uint8_t byte)
{
    static int count = 0;
    printf("%02X ", byte);
    count++;
    if (count >= 16) {
        count = 0;
        printf("\n");
    }
}

static void debug_log_hex_len(const uint8_t *data, uint8_t len)
{
    while (len--) {
        debug_log_hex(*data++);
    }
    fflush(stdout);
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize data and hardware of bus's physical layer.
 *
 * @param[in] phy       Handle of bus physical layer.
 * @param[in] uart      Number of the UART. 0=first.
 */
void bus_phy_initialize(sBusPhy_t *phy, uint8_t uart)
{
    phy->uCurrentBytesToSend = 0;
    phy->uUart = uart;
    phy->uFlags = 0;

    do {
        if (uart >= BUSPHY_MAX_DEVICES) {
            break;
        }

        // sender is initial off, receiver is always on.
        bus_phy_activate_sender(phy, false);
#ifndef BUS_TXRX_COMBINED
        bus_phy_activate_receiver(phy, true);
#endif
    } while (false);
}

/**
 * Activate or deactivate bus's sender hardware (driver).
 *
 * @param[in] phy       Handle of bus physical layer.
 * @param[in] activate  true: activate sender, false: deactivate sender.
 */
void bus_phy_activate_sender(sBusPhy_t *phy, bool activate)
{
    // insert code for sender activation here, if needed.
}

/**
 * Activate or deactivate bus's receiver hardware.
 *
 * @param[in] phy       Handle of bus physical layer.
 * @param[in] activate true: activate receiver, false: deactivate receiver.
 */
#ifndef BUS_TXRX_COMBINED
void bus_phy_activate_receiver(sBusPhy_t *phy, bool activate)
{
    // insert code for receiver activation here, if needed.
}
#endif

/**
 * Send given number of data on the bus.
 * Sending is initiated, if the previous sending process finished.
 *
 * @param[in] phy       Handle of bus physical layer.
 * @param[in] msg       Pointer to message to be sent.
 * @param[in] len       Length in bytes of mesage to be sent.
 *
 * @returns true: sending successfully initiated, otherwise false.
 */
bool bus_phy_send(sBusPhy_t *phy, const uint8_t *msg, uint8_t len)
{
    bool rc = true;

    //if (psPhy->uCurrentBytesToSend != 0) {
    //    return false;
    //}
    //psPhy->uCurrentBytesToSend = uLen;
    phy->uFlags |= e_uarttxflag;
    //psPhy->puSendPtr = puMsg;

    bus_phy_activate_sender(phy, true);
    if (vos_send(msg_b_get_uart(phy->uUart), (void *)msg, len) == len) {
        phy->uFlags &= ~e_uarttxflag;
    }
    else {
        perror("bus_phy_send");
        rc = false;
    }
    debug_log_hex_len(msg, len);

    bus_phy_activate_sender(phy, false);
#ifndef BUS_TXRX_COMBINED
    bus_phy_activate_receiver(phy, true);
#endif
    return rc;
}

/**
 * Checks if data is currently beeing sent.
 *
 * @param[in] phy       Handle of bus physical layer.
 *
 * @returns true:       sending in progress.
 */
bool bus_phy_sending(sBusPhy_t *phy)
{
    return (vos_get_pending_send_bytes(msg_b_get_uart(phy->uUart)) != 0);
}

/**
 * Checks if data has been received.
 *
 * @param[in] phy       Handle of bus physical layer.
 *
 * @returns true: at least one byte is waiting in receive buffer.
 */
bool bus_phy_data_received(sBusPhy_t *phy)
{
    return (vos_get_pending(msg_b_get_uart(phy->uUart)) > 0);
}

/**
 * Read a byte from the received bytes queue.
 *
 * @param[in] phy       Handle to bus's phsical layer
 * @param[out] byte     Received byte. *puByte remains unchange if no byte has been received.
 *
 * @returns true if a byte has been received, otherwise false.
 */
bool bus_phy_read_byte(sBusPhy_t *phy, uint8_t *byte)
{
    bool rc = true;

    if (vos_recv(msg_b_get_uart(phy->uUart), byte, 1) != 1) {
        perror("bus_phy_read_byte");
        rc = false;
    }
    return rc;
}

/**
 * Flush all data from physical layer's input buffer.
 *
 * @param[in] phy
 * Handle of bus physical layer.
 */
void bus_phy_flush(sBusPhy_t *phy)
{
    vos_flush(msg_b_get_uart(phy->uUart));
}

// --- Global functions --------------------------------------------------------

/** @} */
/** @} */
