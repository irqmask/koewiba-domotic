/**
 * @addtogroup BUS
 * @addtogroup BUS_SCHEDUL
 * @brief Message scheduler of bus essages. Used by the master on the bus.
 *
 * @{
 * @file    bus_schedul.c
 * @brief   Bus message scheduler.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
//TODO remove after debug
#include <avr/io.h>
#include <util/delay.h>

#include "bus_intern.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Start sending of next token.
 */
BOOL bSendNextTimeSlotToken(sBus_t* psBus)
{
    sNodeInfo_t* node;

    node = &psBus->asNodeList[psBus->uCurrentNode];

    if (node->uAddress != 0) {
        psBus->auTokenMsg[1] = node->uAddress | 0x80;
        return BUS__bPhySend(&psBus->sPhy, psBus->auTokenMsg, BUS_TOKEN_MSG_LEN);
    }
    return FALSE;
}

/**
 * Count node errors.
 */
void vNodeError(sBus_t* psBus)
{
    if (psBus->asNodeList[psBus->uCurrentNode].uErrCnt < 255) {
        psBus->asNodeList[psBus->uCurrentNode].uErrCnt++;
    }
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize scheduler functions.
 *
 * @param[in] psBus
 * Handle of the bus.
 */
void BUS__vSchedulConfigure(sBus_t* psBus)
{
    uint8_t ii;

    psBus->uCurrentNode = BUS_MAXNODES - 1;
    for (ii=0; ii<BUS_MAXNODES; ii++) {
        psBus->asNodeList[ii].uAddress = ii;
        //psBus->asNodeList[ii].uAddress = 0;
        psBus->asNodeList[ii].uErrCnt = 0;
    }
    psBus->auTokenMsg[0] = BUS_SYNCBYTE;
    psBus->auTokenMsg[1] = 0;
}

// --- Global functions --------------------------------------------------------

/**
 * Add new node to scheduler list.
 *
 * @param[in] psBus
 * Handle of the bus.
 * @param[in] uNodeAddress
 * Node address.
 *
 * @returns TRUE f node has been successfully added, otherwise false.
 */
BOOL BUS_bSchedulAddNode(sBus_t* psBus, uint8_t uNodeAddress)
{
    uint8_t ii = 0;

    // check node address correctness
    if ((uNodeAddress < 1) || (uNodeAddress > 127)) {
        return FALSE;
    }
    // search free place in nodelist
    for (ii=0; ii<BUS_MAXNODES; ii++) {
        if (psBus->asNodeList[ii].uAddress == uNodeAddress) return FALSE; // Adresse
        if (psBus->asNodeList[ii].uAddress == 0) {
            psBus->asNodeList[ii].uAddress = uNodeAddress;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Schedule nodes on bus.
 * 
 * @param[in] psBus
 * Handle of the bus.
 */
BOOL BUS_bScheduleAndGetMessage(sBus_t* psBus)
{
    BOOL rc = FALSE;

    switch (psBus->eState) {
    case eBus_InitWait:
        psBus->eState = eBus_Idle;
        break;

    case eBus_Idle:
        // select next node
        psBus->uCurrentNode++;
        if (psBus->uCurrentNode >= BUS_MAXNODES) {
            psBus->uCurrentNode = 0;
        }

        // send token
        if (bSendNextTimeSlotToken(psBus)) {
            CLK_bTimerStart(&psBus->sNodeAnsTimeout, 100);
            psBus->eState = eBus_SendingToken;
        }
        break;

    case eBus_SendingToken:
        // wait for finished token sending
        if (!BUS__bPhySending(&psBus->sPhy)) {
            psBus->bSchedWaitingForAnswer = TRUE;
            psBus->eState = eBus_ReceivingWait;
        } else {
            //TODO CV: timeout and error handling, if token is not sent
        }
        break;

    default:
        break;
    }

    rc = BUS__bTrpSendReceive(psBus);

    if (psBus->bSchedWaitingForAnswer) {
        if (psBus->bSchedMsgReceived) {
            // TODO check node ID of received message

            psBus->bSchedMsgReceived = FALSE;
            psBus->bSchedWaitingForAnswer = FALSE;
            psBus->eState = eBus_Idle;
        } else {
            if (CLK_bTimerIsElapsed(&psBus->sNodeAnsTimeout)) {
                // pulse error LED
                PORTD |= 0b00010000;
                _delay_ms(5);
                PORTD &= ~(0b00010000);
                // save error to node
                vNodeError(psBus);
                // return to IDLE state
                psBus->bSchedMsgReceived = FALSE;
                psBus->bSchedWaitingForAnswer = FALSE;
                psBus->eState = eBus_Idle;
            }
        }
    }

    return rc;
}

/** @} */
/** @} */
