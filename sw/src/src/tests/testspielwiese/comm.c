/**
 * @addtogroup COMM
 *
 * @{
 * @file    comm.c
 * @brief   Communication driver.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include "prjtypes.h"
#include "comm.h"

// --- Definitions -------------------------------------------------------------

#define NOW 0

#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * COMM_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

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

ISR(SIG_USART_RECV)
{
    switch (eState) {
    case eComm_Idle:
        //if (not error frame)
            bBusBytePending = TRUE;
        break;

    case eComm_Receiving:
        if (eRecvState == eComm_RecvByte) {
            bBusBytePending = TRUE;
            auMessage[uCurrentReceivedBytes++] = UDR0;
        }
        break;

    case eComm_Sending:
        bBusBytePending = TRUE;
        uBusMirrorByte = UDR0;
        break;

    default:
        break;
    }
}

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


uint8_t COMM__Send(void)
{
    switch (eSendState) {
    case eComm_SendIdle:
        break;

    case eComm_SendError:
        eSendState = eComm_SendIdle;
        return 0;

    default:
        eSendState = eComm_SendError;
        break;
    }
    return 1;
}

// --- Global functions --------------------------------------------------------

void COMM_Initialize(void)
{
    // initialize UART
#if defined (__AVR_ATmega8__)
    UBRRH = UBRRVALH;
    UBRRL = UBRRVALL;
    UCSRB |= 0b10011000;
#elif defined (__AVR_ATmega88__)
    UBRR0H = UBRRVALH;
    UBRR0L = UBRRVALL;
    UCSR0B |= 0b10011000;
#endif

    // sender is initial off, receiver is always on.
    COMM_ActivateSender(FALSE);
    COMM_ActivateReceiver(TRUE);

    bBusIsRecovered = FALSE;
    bBusBytePending = FALSE;
    uCurrentReceivedBytes = 0;
    uCurrentBytesToSend = 0;
    uLastByteTime = NOW;
    eState = eComm_InitWait;
    eRecvState = eComm_RecvAcknowledged;
    eSendState = eComm_SendIdle;
}

void COMM_ActivateSender(BOOL bActivate)
{
    if (bActivate) {
        UART_PORT |= UART_DRIVER;
    }
    else {
        UART_PORT &= ~UART_DRIVER;
    }
}

void COMM_ActivateReceiver(BOOL bActivate)
{
    if (bActivate) {
        UART_PORT &= ~UART_RECVSTOP;
    }
    else {
        UART_PORT |= UART_RECVSTOP;
    }
}

void COMM_Communicate(void)
{
    switch (eState) {
    case eComm_Idle:
        // check bus recovery state
        if (bBusIsRecovered) {
            if (bBusBytePending) {
                eRecvState = eComm_RecvFirstByte;
                eState = eComm_Receiving;
                bBusIsRecovered = FALSE;
            }
        }
        else {
            if ((NOW - uLastByteTime) > COMM_BUS_RECOVERY) {
                bBusIsRecovered = TRUE;
            }
        }
        break;

    case eComm_InitWait:
        // wait initial bus idle time
        //if (now - uLastByteTime)
        eState = eComm_Idle;
        break;

    case eComm_Receiving:
        if (0 == COMM__Receive()) eState = eComm_Idle;
        break;

    case eComm_Sending:
        if (0 == COMM__Send()) eState = eComm_Idle;
        break;

    default:
        break;
    }

}

BOOL COMM_RecvMessage(uint8_t** ppuMsg, uint8_t* puMsgLen)
{
    if (eState == eComm_Receiving && eRecvState == eComm_RecvMessageComplete) {
        *ppuMsg = &auMessage[0];
        *puMsgLen = uCurrentReceivedBytes;
        return TRUE;
    }
    return FALSE;
}

void COMM_AcknowledgeMessage(void)
{
    if (eState == eComm_Receiving && eRecvState == eComm_RecvMessageComplete) {
        eRecvState = eComm_RecvAcknowledged;
        // set global state back to Idle
        eState = eComm_Idle;
    }
}

//BOOL COMM_SendMessage(uint8_t* puMsg, uint8_t uMsgLen)
//{
//    if ((eState != eComm_Idle) || (eSendState != eComm_SendIdle)) {
//        return FALSE;
//    }
//    uCurrentBytesToSend = uMsgLen;
//    return TRUE;
//}

/** @} */
