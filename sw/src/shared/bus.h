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
    eComm_InitWait,
    eComm_Idle,
    eComm_Receiving,
    eComm_Sending
} eBusState_t;

typedef struct busphy {
    uint8_t         uUart;
    uint8_t         uCurrentBytesToSend;
    const uint8_t*  puSendPtr;
} sBusPhy_t;

typedef struct busmsg {
    uint16_t        uReceiver;
    uint8_t         uLength;
    uint8_t         uOverallLength;
} sBusMsg_t;

typedef struct bus {
    eBusState_t     eState;  
    sBusPhy_t       sPhy;  
    sBusMsg_t       sMsg;
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
