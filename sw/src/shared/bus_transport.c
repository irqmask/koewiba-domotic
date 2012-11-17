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

#define NOW 0

// --- Type definitions --------------------------------------------------------

typedef enum eCommState {
    eComm_InitWait,
    eComm_Idle,
    eComm_Receiving,
    eComm_Sending
} eCommState_t;

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

volatile eCommState_t       eState      = eComm_InitWait;
volatile eCommRecvState_t   eRecvState  = eComm_RecvAcknowledged;
volatile eCommSendState_t   eSendState  = eComm_SendIdle;
volatile BOOL               bBusIsRecovered = FALSE;
volatile BOOL               bBusBytePending = FALSE;
volatile uint8_t            uBusMirrorByte = 0;
volatile uint8_t            uCurrentReceivedBytes = 0;
volatile uint8_t            uCurrentBytesToSend = 0;
volatile uint8_t            auMessage[COMM_MAXMSGSIZE];
uint16_t                    uLastByteTime = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

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

// --- Global functions --------------------------------------------------------


/** @} */
/** @} */
