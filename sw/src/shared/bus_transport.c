/**
 * @addtogroup BUS
 * @addtogroup BUS_TRANSPORT
 * @brief Transport protocol for sending and receiving messages on the bus.
 *
 * @{
 * @file    bus_transport.c
 * @brief   Transport protocol for sending and receiving messages on the bus.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "bus_intern.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum eCommRecvState {
    eComm_RecvCheckBusRecovery,
    eComm_RecvFirstByte,
    eComm_RecvByte,
    eComm_RecvMessageComplete,
    eComm_RecvError,
    eComm_RecvAcknowledged,
} eCommRecvState_t;

typedef enum eCommSendState {
    eComm_SendIdle,
    eComm_SendError,
} eCommSendState_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

BOOL bReceive(sBus_t* psBus)
{
    uint8_t u;
    
    do {
        if (!BUS__bPhyDataReceived(psBus->sPhy)) {
            break;
        }
        u = BUS__uPhyReceiveByte(psBus->sPhy);
        
        // 1. byte: check sync byte
        if (psBus->sMsg.uOverallLength == 0) {
            
        // 2. byte: check token byte
        } else if (psBus->sMsg.uOverallLength == 1) {
            
        }
        psBus->sMsg.uOverallLength++;
        return TRUE;
    } while ( FALSE );
    
    return FALSE;
}

// --- Module global functions -------------------------------------------------

BOOL BUS__vTrpSendReceive(sBus_t* psBus)
{
    BOOL rc = FALSE;
    
    switch (psBus->eState) {
    case eComm_InitWait:
        break;
    case eComm_Idle:
        break;
    case eComm_Receiving:
        bReceive(psBus);
        break;
    case eComm_Sending:
        break;
    default:
        break;
    }
    
    return rc;
}

// --- Global functions --------------------------------------------------------

void BUS_vInitialize(sBus_t* psBus, uint8_t uUart)
{
    psBus->eState = eComm_InitWait;
    BUS__vPhyInitialize(&psBus->sPhy, uUart);
}

BOOL BUS_bGetMessage(sBus_t* psBus)
{
    return BUS__vTrpSendReceive(psBus);  
}

BOOL BUS_bReadMessage(sBus_t* psBus, 
                      uint8_t* puSender, 
                      uint8_t* puLen, 
                      uint8_t* puMsg)
{
    return FALSE;
}


/*

uint8_t COMM__Receive(void)
{
    switch (eRecvState) {
    case eComm_RecvFirstByte:
        // save first byte and initiate receiving
        auMessage[eMsgLength] = UDR0;
        uCurrentReceivedBytes = 1;
        uLastByteTime = NOW;
        eRecvState = eComm_RecvByte;
        break;

    case eComm_RecvByte:
        if (bBusBytePending) {
            // check if desired length has been reached
            if (uCurrentReceivedBytes == auMessage[eMsgLength]) {
                eRecvState = eComm_RecvMessageComplete;
            }
            // have more bytes been sent as expected?
            else if (uCurrentReceivedBytes > auMessage[eMsgLength]) {
                eRecvState = eComm_RecvError;
            }
            bBusBytePending = FALSE;
        }
        else if ((NOW - uLastByteTime) > COMM_INTERBYTE_TIMEOUT) {
            eRecvState = eComm_RecvError;
        }
        break;

    case eComm_RecvMessageComplete:
        // nothing to do, wait for application to read message
        break;

    case eComm_RecvAcknowledged:
        // application has acknowledged message
        return 0;

    case eComm_RecvError:
        //TODO define error behavior
        return 0;

    default:
        eRecvState = eComm_RecvError;
        break;
    }

    return 1;
}
*/

/** @} */
/** @} */
