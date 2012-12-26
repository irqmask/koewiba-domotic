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

#include "config.h"
#include "prjtypes.h"
#include "clock.h"

// --- Definitions -------------------------------------------------------------

#define BUS_BAUDRATE           38400
#define BUS_MAXMSGLEN          16
#define BUS_MAXBIGMSGLEN       (64 + 2)
#define BUS_EMPTY_MSG_LEN      3
#define BUS_TOKEN_MSG_LEN      2

#ifndef BUS_MAXNODES
#define BUS_MAXNODES 16
#endif

// --- Type definitions --------------------------------------------------------

//! Possible states of the bus, as seen by each node
typedef enum busstate {
    eBus_InitWait,          //!< We are not synced on bus yet.
    eBus_Idle,              //!< Not sending and not receiving.
    eBus_SendingToken,      //!< Scheduler is sending the token.
    eBus_ReceivingWait,     //!< Waiting for incoming message.
    eBus_ReceivingActive,   //!< We suggest the message is for us.
    eBus_ReceivingPassive,  //!< We receive byte but those are not for us.
    eBus_GotToken,          //!< We have got the bus token.
    eBus_Sending,           //!< We are sending now our message.
    eBus_Last
} eBusState_t;

typedef enum busflags {
    e_uartrxflag    = 0b00000001,
    e_uarttxflag    = 0b00000010,
    e_uartrxerrflag = 0b00000100,
    e_uarttxerrflag = 0b00001000,
    e_timeout       = 0b00010000
} eBusFlags_t;

//! configuration data of the bus
typedef struct busconfig {
    uint16_t        uOwnNodeAddress;    //!< address of node on bus.
} sBusCfg_t;

typedef uint8_t auRecBuf_t[BUS_MAXBIGMSGLEN];
typedef uint8_t auSndBuf_t[BUS_MAXMSGLEN];

//! common receive queue
typedef struct recvbuf {
    uint8_t         uReadPos;
    uint8_t         uWritePos;
    auRecBuf_t      auBuf;
} sBusRec_t;

//! data of the physical layer of the bus
typedef struct busphy {
    uint8_t         uUart;
    uint8_t         uCurrentBytesToSend;
    eBusFlags_t     uFlags;
    sBusRec_t       sRecvBuf;
    const uint8_t*  puSendPtr;
} sBusPhy_t;

//! data and metadata of received message
typedef struct rcvbusmsg {
    uint16_t        uSender;
    uint16_t        uReceiver;
    uint8_t         uLength;
    uint8_t         uOverallLength;
    auSndBuf_t      auBuf;
    uint16_t        uCRC;
} sRcvBusMsg_t;

//! data of message to be sent
typedef struct sndbusmsg {
    uint16_t        uReceiver;
    uint8_t         uLength;
    uint8_t         uOverallLength;
    auSndBuf_t      auBuf;          //!< message data including header, data and crc.
} sSndBusMsg_t;

//! contains all information about a node needed for scheduling.
typedef struct nodeinfo {
    uint16_t        uAddress;
    uint16_t        uErrCnt;
} sNodeInfo_t;

//! data of bus. used as handle for all public methods.
typedef struct bus {
    eBusState_t     eState;                         //!< current state of the bus.
    BOOL            bMsgReceived;                   //!< flag, stating that there is an unread message
    sBusCfg_t       sCfg;                           //!< configuration data.
    sBusPhy_t       sPhy;                           //!< physical layer data.
    sRcvBusMsg_t    sRecvMsg;                       //!< contains current received message.
    sSndBusMsg_t    sSendMsg;                       //!< contains current message to be sent.
    uint8_t         auEmptyMsg[BUS_EMPTY_MSG_LEN];  //!< pre-compiled empty message.
    sClkTimer_t     sReceiveTimeout;                //!< receive timeout
    
    // following data is only used by bus scheduler
#ifdef BUS_SCHEDULER
    sNodeInfo_t     asNodeList[BUS_MAXNODES];       //!< list of configured nodes
    uint8_t         uCurrentNode;                   //!< current processed node
    uint8_t         auTokenMsg[BUS_TOKEN_MSG_LEN];  //!< pre-compiled token message.
    BOOL            bSchedWaitingForAnswer;         //!< flag, if scheduler is waiting for an answer
    BOOL            bSchedMsgReceived;              //!< flag, if any message has been received
    sClkTimer_t     sNodeAnsTimeout;                //!< node answer timeout
#endif
} sBus_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void    BUS_vConfigure              (sBus_t*        psBus, 
                                       uint16_t       uNodeAddress);

void    BUS_vInitialize             (sBus_t*        psBus,
                                        uint8_t        uUart);
                                     
BOOL    BUS_bGetMessage             (sBus_t*        psBus);

BOOL    BUS_bReadMessage            (sBus_t*        psBus, 
                                       uint8_t*       puSender,
                                       uint8_t*       puLen,
                                       uint8_t*       puMsg);
                                     
BOOL    BUS_bSendMessage             (sBus_t*        psBus,
                                       uint8_t*       puReceiver,
                                       uint8_t*       puLen,
                                       uint8_t*       puMsg);

BOOL    BUS_bIsIdle                  (sBus_t*       psBus);

#ifdef BUS_SCHEDULER
BOOL    BUS_bSchedulAddNode         (sBus_t*        psBus,
                                      uint8_t       uNodeAddress);

BOOL    BUS_bScheduleAndGetMessage  (sBus_t*        psBus);
#endif
                                     
#endif /* _BUS_H_ */
/** @} */
