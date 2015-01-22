/**
 * @addtogroup BUS
 * @addtogroup BUS_PHY_OS
 * @brief Physical layer of bus protocol for operating systems like Linux and
 * Windows.
 *
 * @{
 * @file    bus_phy_os.c
 * @brief   Physical layer of bus protocol.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

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

static void debug_log_hex (const uint8_t byte)
{
    static int count = 0;
    printf("%02X ", byte);
    count++;
    if (count >= 16) {
        count = 0;
        printf("\n");
    }
}

static void debug_log_hex_len (const uint8_t* data, uint8_t len)
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
void bus_phy_initialize(sBusPhy_t* phy, uint8_t uart)
{
    phy->uCurrentBytesToSend = 0;
    phy->uUart = uart;
    phy->uFlags = 0;

    do {
        if (uart >= BUSPHY_MAX_DEVICES) break;

        // sender is initial off, receiver is always on.
        bus_phy_activate_sender(phy, FALSE);
#ifndef BUS_TXRX_COMBINED
        bus_phy_activate_receiver(phy, TRUE);
#endif
    } while ( FALSE );
}

/**
 * Activate or deactivate bus's sender hardware (driver).
 *
 * @param[in] phy       Handle of bus physical layer.
 * @param[in] activate  TRUE: activate sender, FALSE: deactivate sender.
 */
void bus_phy_activate_sender(sBusPhy_t* phy, BOOL activate)
{
    // insert code for sender activation here, if needed.
}

/**
 * Activate or deactivate bus's receiver hardware.
 *
 * @param[in] phy       Handle of bus physical layer.
 * @param[in] activate TRUE: activate receiver, FALSE: deactivate receiver.
 */
#ifndef BUS_TXRX_COMBINED
void bus_phy_activate_receiver(sBusPhy_t* phy, BOOL activate)
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
 * @returns TRUE: sending successfully initiated, otherwise FALSE.
 */
BOOL bus_phy_send(sBusPhy_t* phy, const uint8_t* msg, uint8_t len)
{
    BOOL rc = TRUE;

    //if (psPhy->uCurrentBytesToSend != 0) {
    //    return FALSE;
    //}
    //psPhy->uCurrentBytesToSend = uLen;
    phy->uFlags |= e_uarttxflag;
    //psPhy->puSendPtr = puMsg;

    bus_phy_activate_sender(phy, TRUE);
    if (vos_send(msg_b_get_uart(phy->uUart), (void*)msg, len) == len) {
        phy->uFlags &= ~e_uarttxflag;
    } else {
        perror("bus_phy_send");
        rc = FALSE;
    }
    debug_log_hex_len(msg, len);

    bus_phy_activate_sender(phy, FALSE);
#ifndef BUS_TXRX_COMBINED
    bus_phy_activate_receiver(phy, TRUE);
#endif
    return rc;
}

/**
 * Checks if data is currently beeing sent.
 *
 * @param[in] phy       Handle of bus physical layer.
 *
 * @returns TRUE:       sending in progress.
 */
BOOL bus_phy_sending(sBusPhy_t* phy)
{
    return (vos_get_pending_send_bytes(msg_b_get_uart(phy->uUart)) != 0);
}

/**
 * Checks if data has been received.
 *
 * @param[in] phy       Handle of bus physical layer.
 *
 * @returns TRUE: at least one byte is waiting in receive buffer.
 */
BOOL bus_phy_data_received(sBusPhy_t* phy)
{
    return (vos_get_pending(msg_b_get_uart(phy->uUart)) > 0);
}

/**
 * Read a byte from the received bytes queue.
 *
 * @param[in] phy       Handle to bus's phsical layer
 * @param[out] byte     Received byte. *puByte remains unchange if no byte has been received.
 *
 * @returns TRUE if a byte has been received, otherwise false.
 */
BOOL bus_phy_read_byte(sBusPhy_t* phy, uint8_t *byte)
{
    BOOL rc = TRUE;

    if (vos_recv(msg_b_get_uart(phy->uUart), byte, 1) != 1) {
        perror("bus_phy_read_byte");
        rc = FALSE;
    }
    return rc;
}

/**
 * Flush all data from physical layer's input buffer.
 *
 * @param[in] phy
 * Handle of bus physical layer.
 */
void bus_phy_flush(sBusPhy_t* phy)
{
    vos_flush(msg_b_get_uart(phy->uUart));
}

// --- Global functions --------------------------------------------------------

/** @} */
/** @} */
