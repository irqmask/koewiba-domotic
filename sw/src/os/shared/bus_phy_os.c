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
#include "sysserial.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void vDebugHexOutput(const uint8_t uData)
{
    static int count = 0;
    printf("%02X ", uData);
    count++;
    if (count >= 16) {
        count = 0;
        printf("\n");
    }
}

static void vDebugHexOutputLen(const uint8_t* puData, uint8_t uLen)
{
    while (uLen--) {
        vDebugHexOutput(*puData++);
    }
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize data and hardware of bus's physical layer.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] uUart     Number of the UART. 0=first.
 */
void bus_phy_initialize(sBusPhy_t* psPhy, uint8_t uUart)
{
    psPhy->uCurrentBytesToSend = 0;
    psPhy->uUart = uUart;
    psPhy->uFlags = 0;

    do {
        if (uUart >= BUSPHY_MAX_DEVICES) break;

        // sender is initial off, receiver is always on.
        bus_phy_activate_sender(psPhy, FALSE);
#ifndef BUS_TXRX_COMBINED
        bus_phy_activate_receiver(psPhy, TRUE);
#endif
    } while ( FALSE );
}

/**
 * Activate or deactivate bus's sender hardware (driver).
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate TRUE: activate sender, FALSE: deactivate sender.
 */
void bus_phy_activate_sender(sBusPhy_t* psPhy, BOOL bActivate)
{
    // insert code for sender activation here, if needed.
}

/**
 * Activate or deactivate bus's receiver hardware.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate TRUE: activate receiver, FALSE: deactivate receiver.
 */
#ifndef BUS_TXRX_COMBINED
void bus_phy_activate_receiver(sBusPhy_t* psPhy, BOOL bActivate)
{
    // insert code for receiver activation here, if needed.
}
#endif

/**
 * Send given number of data on the bus.
 * Sending is initiated, if the previous sending process finished.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] puMsg     Pointer to message to be sent.
 * @param[in] uLen      Length in bytes of mesage to be sent.
 *
 * @returns TRUE: sending successfully initiated, otherwise FALSE.
 */
BOOL bus_phy_send(sBusPhy_t* psPhy, const uint8_t* puMsg, uint8_t uLen)
{
    BOOL rc = TRUE;

    //if (psPhy->uCurrentBytesToSend != 0) {
    //    return FALSE;
    //}
    //psPhy->uCurrentBytesToSend = uLen;
    psPhy->uFlags |= e_uarttxflag;
    //psPhy->puSendPtr = puMsg;

    bus_phy_activate_sender(psPhy, TRUE);
    if (sys_serial_send(msg_b_get_uart(psPhy->uUart), (void*)puMsg, uLen) == uLen) {
        psPhy->uFlags &= ~e_uarttxflag;
    } else {
        printf("BUS__bPhySend error\n");
        rc = FALSE;
    }
    vDebugHexOutputLen(puMsg, uLen);

    bus_phy_activate_sender(psPhy, FALSE);
#ifndef BUS_TXRX_COMBINED
    bus_phy_activate_receiver(psPhy, TRUE);
#endif
    return rc;
}

/**
 * Checks if data is currently beeing sent.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns TRUE:       sending in progress.
 */
BOOL bus_phy_sending(sBusPhy_t* psPhy)
{
    return FALSE;
}

/**
 * Checks if data has been received.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns TRUE: at least one byte is waiting in receive buffer.
 */
BOOL bus_phy_data_received(sBusPhy_t* psPhy)
{
    return (sys_serial_get_pending(msg_b_get_uart(psPhy->uUart)) > 0);
}

/**
 * Read a byte from the received bytes queue.
 *
 * @param[in] psPhy     Handle to bus's phsical layer
 * @param[out] puByte   Received byte. *puByte remains unchange if no byte has been received.
 *
 * @returns TRUE if a byte has been received, otherwise false.
 */
BOOL bus_phy_read_byte(sBusPhy_t* psPhy, uint8_t *puByte)
{
    BOOL rc = TRUE;

    if (sys_serial_recv(msg_b_get_uart(psPhy->uUart),
                        puByte, 1) != 1) {
        rc = FALSE;
    }
    return rc;
}

/**
 * Flush all data from physical layer's input buffer.
 *
 * @param[in] psPhy
 * Handle of bus physical layer.
 */
void bus_phy_flush(sBusPhy_t* psPhy)
{
    sys_serial_flush(msg_b_get_uart(psPhy->uUart));
}

// --- Global functions --------------------------------------------------------

/** @} */
/** @} */

