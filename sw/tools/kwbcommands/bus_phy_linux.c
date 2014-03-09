/**
 * @addtogroup BUS
 * @addtogroup BUS_PHY_LINUX
 * @brief Physical layer of bus protocol for linux.
 *
 * @{
 * @file    bus_phy_linux.c
 * @brief   Physical layer of bus protocol.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "bus_intern.h"
#include "BusCommunication.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void vDebugHexOutput(uint8_t uData)
{
    static int count = 0;
    printf("%02X ", uData);
    count++;
    if (count >= 16) {
        count = 0;
        printf("\n");
    }
}

static void vDebugHexOutputLen(uint8_t* puData, uint8_t uLen)
{
    while (uLen--) {
        vDebugHexOutput(*puData++);
    }
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize data and hardware of bus's physical layer.
 *
 * @param[in] psPhy		Handle of bus physical layer.
 * @param[in] uUart		Number of the UART. 0=first.
 */
void BUS__vPhyInitialize(sBusPhy_t* psPhy, uint8_t uUart)
{
    psPhy->uCurrentBytesToSend = 0;
    psPhy->uUart = uUart;
    psPhy->uFlags = 0;

    do {
        if (uUart >= BUSPHY_MAX_DEVICES) break;

        printf("%d %s   ", uUart, BUSCOMM__asDevices[uUart].pcPortName);
        if (PSerLib_open(BUSCOMM__asDevices[uUart].pcPortName,
                         &BUSCOMM__asDevices[uUart].psHandle) != PSL_ERROR_none) {
            printf("Open failed\n");
            break;
        } else {
            printf("Open succeeded\n");
        }
        if (PSerLib_setParams(BUSCOMM__asDevices[uUart].psHandle, PSL_BR_38400,
                             PSL_DB_8, PSL_P_none, PSL_SB_1, PSL_FC_none) != PSL_ERROR_none) {
            printf("PSerLib_setParams failed.\n");
            break;
        } else {
            printf("setParams succeeded\n");
        }
        if (PSerLib_setBlocking(BUSCOMM__asDevices[uUart].psHandle, 0) != PSL_ERROR_none) {
            printf("PSerLib_setBlocking failed.\n");
            break;
        }

        // sender is initial off, receiver is always on.
        BUS__vPhyActivateSender(psPhy, FALSE);
#ifndef BUS_TXRX_COMBINED
        BUS__vPhyActivateReceiver(psPhy, TRUE);
#endif
    } while ( FALSE );
}

/**
 * Activate or deactivate bus's sender hardware (driver).
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate
 * @returns TRUE: activate sender, FALSE: deactivate sender.
 */
void BUS__vPhyActivateSender(sBusPhy_t* psPhy, BOOL bActivate)
{
    // insert code for sender activation here, if needed.
}

/**
 * Activate or deactivate bus's receiver hardware.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate
 * @returns TRUE: activate receiver, FALSE: deactivate receiver.
 */
#ifndef BUS_TXRX_COMBINED
void BUS__vPhyActivateReceiver(sBusPhy_t* psPhy, BOOL bActivate)
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
BOOL BUS__bPhySend(sBusPhy_t* psPhy, const uint8_t* puMsg, uint8_t uLen)
{
    BOOL rc = TRUE;

    //if (psPhy->uCurrentBytesToSend != 0) {
    //    return FALSE;
    //}
    //psPhy->uCurrentBytesToSend = uLen;
    psPhy->uFlags |= e_uarttxflag;
    //psPhy->puSendPtr = puMsg;

    BUS__vPhyActivateSender(psPhy, TRUE);
    if (PSerLib_writeBinaryData(BUSCOMM__asDevices[psPhy->uUart].psHandle,
                                puMsg, uLen, NULL) == PSL_ERROR_none) {
        psPhy->uFlags &= ~e_uarttxflag;
    } else {
        printf("BUS__bPhySend error\n");
        rc = FALSE;
    }
    vDebugHexOutputLen(puMsg, uLen);

    BUS__vPhyActivateSender(psPhy, FALSE);
#ifndef BUS_TXRX_COMBINED
    BUS__vPhyActivateReceiver(psPhy, TRUE);
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
BOOL BUS__bPhySending(sBusPhy_t* psPhy)
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
BOOL BUS__bPhyDataReceived(sBusPhy_t* psPhy)
{
    int bytes = 0;

    PSerLib_GetPendingBytesToRead(BUSCOMM__asDevices[psPhy->uUart].psHandle, 
                                  &bytes);
    return (bytes != 0);
}

/**
 * Read a byte from the received bytes queue.
 *
 * @param[in] psPhy     Handle to bus's phsical layer
 * @param[out] puByte   Received byte. *puByte remains unchange if no byte has been received.
 *
 * @returns TRUE if a byte has been received, otherwise false.
 */
BOOL BUS__bPhyReadByte(sBusPhy_t* psPhy, uint8_t *puByte)
{
    BOOL rc = TRUE;

    if (PSerLib_readBinaryData(BUSCOMM__asDevices[psPhy->uUart].psHandle, 
                               puByte, 1, NULL) != PSL_ERROR_none) {
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
void BUS__uPhyFlush(sBusPhy_t* psPhy)
{
    PSerLib_FlushReadWrite(BUSCOMM__asDevices[psPhy->uUart].psHandle);
}

// --- Global functions --------------------------------------------------------

/** @} */
/** @} */

