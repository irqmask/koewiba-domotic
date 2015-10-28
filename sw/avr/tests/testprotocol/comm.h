/**
 * @addtogroup COMM
 * @brief Communication driver.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    comm.h
 * @brief   Declaration of communication driver routines.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _COMM_H__
#define _COMM_H__

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define UART_PORT               PORTD
#define UART_INPORT             PIND
#define UART_DDR                DDRD
#define UART_DRIVER             0b00000100
#define UART_RECVSTOP           0b00001000

#define COMM_BAUDRATE           38400
#define COMM_MAXMSGSIZE         16
#define COMM_MAXBIGMSGSIZE      (64 + 2)

#define COMM_BITTIME            10 //TODO
#define COMM_BUS_INITWAIT_TIME  (24 * COMM_BITTIME)
#define COMM_BUS_RECOVERY       (12 * COMM_BITTIME)
#define COMM_INTERBYTE_TIMEOUT  (8 * COMM_BITTIME)

// --- Type definitions --------------------------------------------------------

typedef enum eCommMsgByteIndex {
    eMsgLength,
    eMsgSender,
    eMsgData0,
    eMsgData1,
    eMsgData2,
    eMsgData3,
    eMsgData4,
    eMsgData5,
    eMsgData6,
    eMsgData7,
    eMsgData8,
    eMsgData9,
    eMsgData10,
    eMsgData11,
    eMsgCRCHigh,
    eMsgCRCLow,
    eMsgLast
} eCommMsgByteIndex_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void COMM_Initialize        (void);
void COMM_ActivateSender    (BOOL bActivate);
void COMM_ActivateReceiver  (BOOL bActivate);
void COMM_Communicate       (void);
BOOL COMM_RecvMessage       (uint8_t** ppuMsg, uint8_t* puMsgLen);
void COMM_AcknowledgeMessage(void);
BOOL COMM_SendMessage       (uint8_t* puMsg, uint8_t uMsgLen);

#endif /* _COMM_H__ */
/** @} */
