/**
 * @addtogroup BUS
 * @brief Public interface of bus protocol.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    bus.h
 * @brief   Declaration of communication driver routines.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _BUS_H_
#define _BUS_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define BUS_BAUDRATE           38400
#define BUS_MAXMSGSIZE         16
#define BUS_MAXBIGMSGSIZE      (64 + 2)

// --- Type definitions --------------------------------------------------------

typedef enum busstate {
    eBus_InitWait,
    eBus_Idle,
    eBus_Receiving,
    eBus_Sending
} eBusState_t;

typedef enum busflags
{
    e_uartrxflag    = 0b00000001,
    e_uarttxflag    = 0b00000010,
    e_uartrxerrflag = 0b00000100,
    e_uarttxerrflag = 0b00001000,
    e_timeout       = 0b00010000
} eBusFlags_t;

typedef uint8_t auRecBuf_t[BUS_MAXBIGMSGSIZE];
typedef uint8_t auSndBuf_t[BUS_MAXMSGSIZE];

typedef struct recvbuf {
    uint8_t		uReadPos;
    uint8_t		uWritePos;
    auRecBuf_t  auBuf;
} sBusRec_t;

typedef struct busphy {
    uint8_t         uUart;
    uint8_t         uCurrentBytesToSend;
    eBusFlags_t     uflags;
    sBusRec_t       sRecvBuf;
    const uint8_t* puSendPtr;
} sBusPhy_t;

typedef struct rcvbusmsg {
    uint16_t        uReceiver;
    uint8_t         uLength;
    uint8_t         uOverallLength;
    auSndBuf_t      auBuf;
} sRcvBusMsg_t;

typedef struct sndbusmsg {
    uint16_t        uReceiver;
    uint8_t         uLength;
    uint8_t         uOverallLength;
    auSndBuf_t      auBuf;
} sSndBusMsg_t;

typedef struct bus {
    eBusState_t     eState;  
    sBusPhy_t       sPhy;  
    sRcvBusMsg_t    sRecvMsg;
    sSndBusMsg_t    sSendMsg;
} sBus_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void    BUS_vInitialize             (sBus_t*        psBus,
                                     uint8_t        uUart);
                                     
BOOL    BUS_bGetMessage             (sBus_t*        psBus);

BOOL    BUS_bReadMessage            (sBus_t*        psBus, 
                                     uint8_t*       puSender, 
                                     uint8_t*       puLen, 
                                     uint8_t*       puMsg);
                                     
BOOL    BUS_bSendMessage            (sBus_t*        psBus, 
                                     uint8_t*       puReceiver, 
                                     uint8_t*       puLen, 
                                     uint8_t*       puMsg);
                                     
#endif /* _BUS_H_ */
/** @} */
