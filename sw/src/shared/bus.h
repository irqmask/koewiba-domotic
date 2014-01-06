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
#include "clock.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif
#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection BUS_PCBCONFIG
 * Configure bus hardware. E.g. define pins used by physical layer.
 * @{
 */
#ifndef BUS_PCBCONFIG
 #define BUS_PCBCONFIG      1
 #define BUS_DDR_ENASND     DDRD
 #define BUS_PORT_ENASND    PORTD
 #define BUS_ENASND         PD2
 #define BUS_DDR_DISRCV     DDRD
 #define BUS_PORT_DISRCV    PORTD
 #define BUS_DISRCV         PD3
#endif
/**
 * @}
 */

/**
 * @subsection BUS_APPCONFIG
 * Configure bus modules. E.g. activate scheduling capabilities
 * @{
 */
#ifndef BUS_APPCONFIG
 #define BUS_APPCONFIG      1
 #undef BUS_SCHEDULER
#endif
/**
 * @}
 */

#define BUS_MAXMSGLEN          16
//! length of message including header and footer
#define BUS_MAXTOTALMSGLEN		(BUS_MAXMSGLEN + 7)
#define BUS_MAXBIGMSGLEN       (64 + 2)
#define BUS_EMPTY_MSG_LEN      3
#define BUS_TOKEN_MSG_LEN      2

#ifndef BUS_MAXNODES
#define BUS_MAXNODES 16
#endif

#define BUS_BRDCSTADR   0x00 // Broadcastaddress

// --- Type definitions --------------------------------------------------------

//! Possible states of the subscriber
typedef enum modulestate {
	eMod_Discovery,	//!< Scheduler is in discovery-mode (looking for nodes)
	eMod_Running,	//!< Module is normal mode
	eMod_Sleeping,	//!< Module is sleeping
	eMod_AckWait	//!< Module is waiting for Acknowledge-Message
} eModState_t;

//! Possible states of the bus, as seen by each node
typedef enum busstate {
    eBus_InitWait,          //!< We are not synced on bus yet.
    eBus_Idle,              //!< Not sending and not receiving.
    eBus_SendingToken,      //!< Scheduler is sending the token.
    eBus_SendingAck,        //!< We are sending now our ACK.
    eBus_ReceivingWait,     //!< Waiting for incoming message.
    eBus_ReceivingActive,   //!< We suggest the message is for us.
    eBus_ReceivingPassive,  //!< We receive byte but those are not for us.
    eBus_GotToken,          //!< We have got the bus token.
    eBus_GotMessage,        //!< We have got a message.
    eBus_Sending,           //!< We are sending now our message.
    eBus_AckWaitReceiving,  //!< We are waiting for the ACK after receiving.
    eBus_AckWaitSending,    //!< We are waiting for the ACK after sending.
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
typedef uint8_t auSndBuf_t[BUS_MAXTOTALMSGLEN];

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
    uint8_t         uRetries;       //!< number of send retries.
    auSndBuf_t      auBuf;          //!< message data including header, data and crc.
} sSndBusMsg_t;

//! contains all information about a node needed for scheduling.
typedef struct nodeinfo {
    uint16_t        uAddress;
    uint16_t        uErrCnt;
} sNodeInfo_t;

//! data of bus. used as handle for all public methods.
typedef struct bus {
	eModState_t		eModuleState;					//!< current state of the module.
    eBusState_t     eState;                         //!< current state of the bus.
    BOOL            bMsgReceived;                   //!< flag, stating that there is an unread message
    sBusCfg_t       sCfg;                           //!< configuration data.
    sBusPhy_t       sPhy;                           //!< physical layer data.
    sRcvBusMsg_t    sRecvMsg;                       //!< contains current received message.
    sSndBusMsg_t    sSendMsg;                       //!< contains current message to be sent.
    uint8_t         auEmptyMsg[BUS_EMPTY_MSG_LEN];  //!< pre-compiled empty message.
    sClkTimer_t     sReceiveTimeout;                //!< receive timeout
    sClkTimer_t     sAckTimeout;                    //!< ack timeout

    // following data is only used by bus scheduler
#ifdef BUS_SCHEDULER
    sNodeInfo_t     asNodeList[BUS_MAXNODES];       //!< list of configured nodes
    sNodeInfo_t     asDiscoveryList[BUS_MAXNODES];  //!< list of empty nodes
    uint8_t         uCurrentNode;                   //!< current processed node
    uint8_t         uDiscoverNode;                  //!< current discovery-address
    uint8_t         auTokenMsg[BUS_TOKEN_MSG_LEN];  //!< pre-compiled token message.
    BOOL            bSchedDiscovery;                //!< bus-discovery mode.
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

void    BUS_vFlushBus               (sBus_t*        psBus);

BOOL    BUS_bGetMessage             (sBus_t*        psBus);

BOOL    BUS_bReadMessage            (sBus_t*        psBus,
                                     uint16_t*      puSender,
                                     uint8_t*       puLen,
                                     uint8_t*       puMsg);

BOOL    BUS_bSendMessage            (sBus_t*        psBus,
                                     uint16_t       uReceiver,
                                     uint8_t        uLen,
                                     uint8_t*       puMsg);

BOOL    BUS_bSendAckMessage         (sBus_t*        psBus,
                                     uint16_t       uReceiver);

#ifdef BUS_SCHEDULER
BOOL    BUS_bScheduleAndGetMessage  (sBus_t*        psBus);
void    BUS_vScheduleCheckAndSetSleep(sBus_t* psBus);
#endif

void    BUS_vSleep                   (sBus_t*       psBus);

#endif /* _BUS_H_ */
/** @} */
