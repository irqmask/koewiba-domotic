/**
 * @addtogroup BUS
 * @brief Public interface of bus protocol.
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
#include "queue.h"

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
 #define BUS_DDR_ENASND0    DDRD
 #define BUS_PORT_ENASND0   PORTD
 #define BUS_ENASND0        PD4
 #define BUS_TXRX_COMBINED0 1
 #define BUS_DDR_DISRCV0    DDRD
 #define BUS_PORT_DISRCV0   PORTD
 #define BUS_DISRCV0        PD5
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
 //! size in bytes of the message transmit queue.
 #define BUS_TX_QUEUE_SIZE  100
 //! maximum length of a message to be sent.
 #define BUS_MAXSENDMSGLEN   36
 //! maximum length of a message to be received.
 #define BUS_MAXRECVMSGLEN   64 + 2
#endif
/**
 * @}
 */

//! length of send message buffer including message header and footer.
#define BUS_MAXTOTALSENDMSGLEN  (BUS_MAXSENDMSGLEN + 7)
//! length of receive message buffer including message header and footer.
#define BUS_MAXTOTALRECVMSGLEN  (BUS_MAXRECVMSGLEN + 7)
//! length of an empty message.
#define BUS_EMPTY_MSG_LEN   3
//! length of a token message (sent by bus-scheduler).
#define BUS_TOKEN_MSG_LEN   2


// check constitency of message buffers
#if ((BUS_MAXTOTALSENDMSGLEN + 2) >= BUS_TX_QUEUE_SIZE)
#error "Size of message to be sent is bigger than the send queue size! Queue buffer BUS_TX_QUEUE_SIZE has to be at least 10 bytes bigger than maximum send message size BUS_MAXSENDMSGLEN!"
#endif

//! first usable node address of a bus-node.
#define BUS_FIRSTNODE       0x01
//! last usable node address of a bus-node.
#define BUS_LASTNODE        0x7F

#define BUS_BRDCSTADR       0x0000  //!< global broadcast address.
#define BUS_UNKNOWNADR      0xFFFF  //!< unknown address.

// --- Type definitions --------------------------------------------------------

//! Possible states of the subscriber
typedef enum modulestate {
	eMod_Running,	//!< Module is normal mode
	eMod_Sleeping,	//!< Module is sleeping
} eModState_t;

//! Possible states of the bus, as seen by each node
typedef enum busstate {
    eBus_InitWait,          //!< 0 We are not synced on bus yet.
    eBus_Idle,              //!< 1 Not sending and not receiving.
    eBus_SendingToken,      //!< 2 Scheduler is sending the token.
    eBus_SendingAck,        //!< 3 We are sending now our ACK.
    eBus_ReceivingWait,     //!< 4 Waiting for incoming message.
    eBus_ReceivingActive,   //!< 5 We suggest the message is for us.
    eBus_ReceivingPassive,  //!< 6 We receive byte but those are not for us.
    eBus_GotToken,          //!< 7 We have got the bus token.
    eBus_GotMessage,        //!< 8 We have got a message.
    eBus_Sending,           //!< 9 We are sending now our message.
    eBus_AckWaitReceiving,  //!< 10 We are waiting for the ACK after receiving.
    eBus_AckWaitSending,    //!< 11 We are waiting for the ACK after sending.
    eBus_Last
} eBusState_t;

typedef enum bus_recv_state {
    eBUS_RECV_NOTHING,
    eBUS_RECV_EMPTY_MESSAGE,
    eBUS_RECV_FOREIGN_MESSAGE,
    eBUS_RECV_MESSAGE
} msg_recv_state_t;

typedef enum busflags {
    e_uartrxflag    = 0b00000001,
    e_uarttxflag    = 0b00000010,
    e_uartrxerrflag = 0b00000100,
    e_uarttxerrflag = 0b00001000,
    e_timeout       = 0b00010000
} eBusFlags_t;

//! Possible errors of the bus-sender.
typedef enum errorcodes {
    eBUSERR_NOERR = 0,   //!< Message successfully transferred into tx_queue.
    eBUSERR_MSGSIZE,     //!< Message size exceeds buffer-size.
    eBUSERR_QUEUESPACE,  //!< Insufficient queue-space - please wait!
} eBusSendError_t;

//! configuration data of the bus
typedef struct busconfig {
    uint16_t       uOwnAddress;
    uint8_t        uOwnNodeAddress;    //!< address of node on bus.
    uint8_t        uOwnExtAddress;     //!< extensionaddress of node on bus.
    bool           router_mode;
} sBusCfg_t;

typedef uint8_t auRecBuf_t[BUS_MAXTOTALRECVMSGLEN];
typedef uint8_t auSndBuf_t[BUS_MAXTOTALSENDMSGLEN];

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
    uint8_t         length;
    uint8_t         uOverallLength;
    auRecBuf_t      auBuf;
    uint16_t        uCRC;
} sRcvBusMsg_t;

//! data of message to be sent
typedef struct sndbusmsg {
    uint16_t        uReceiver;
    uint8_t         uOverallLength;
    uint8_t         uRetries;       //!< number of send retries.
    auSndBuf_t      auBuf;          //!< message data including header, data and crc.
} sSndBusMsg_t;

//! data of bus. used as handle for all public methods.
typedef struct bus {
    eModState_t         eModuleState;                   //!< current state of the module.
    eBusState_t         eState;                         //!< current state of the bus.
    sBusCfg_t           sCfg;                           //!< configuration data.
    msg_recv_state_t    msg_receive_state;              //!< stating if a message and what kind of message has been received.
    sBusPhy_t           sPhy;                           //!< physical layer data.
    sRcvBusMsg_t        sRecvMsg;                       //!< contains current received message.
    sSndBusMsg_t        sSendMsg;                       //!< contains current message to be sent.
    queue_t             tx_queue;                       //!< transmit queue for outgoing messages.
    uint8_t             auEmptyMsg[BUS_EMPTY_MSG_LEN];  //!< pre-compiled empty message.
    sClkTimer_t         sReceiveTimeout;                //!< receive timeout
    sClkTimer_t         sAckTimeout;                    //!< ack timeout
} sBus_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void    bus_configure                       (sBus_t*        psBus,
                                             uint16_t       uNodeAddress);

void 	bus_initialize                      (sBus_t*        psBus,
                                             uint8_t        uUart);

void    bus_set_router_mode                 (sBus_t*        bus,
                                             bool           router_mode);

void 	bus_flush_bus                       (sBus_t*        psBus);

BOOL 	bus_get_message                     (sBus_t*        psBus);

BOOL 	bus_read_message                    (sBus_t*        psBus,
                                             uint16_t*      puSender,
                                             uint8_t*       puLen,
                                             uint8_t*       puMsg);

BOOL    bus_read_message_verbose            (sBus_t*        psBus,
                                             uint16_t*      puSender,
                                             uint16_t*      puReceiver,
                                             uint8_t*       puLen,
                                             uint8_t*       puMsg,
                                             uint16_t*      puCRC);

BOOL 	bus_send_message                    (sBus_t*        psBus,
                                             uint16_t       uReceiver,
                                             uint8_t        uLen,
                                             uint8_t*       puMsg);

BOOL 	bus_is_idle                         (sBus_t*        psBus);

void    bus_sleep                           (sBus_t*      	psBus);

BOOL    bus_send_wakeupbyte                 (sBus_t*        psBus);

#endif /* _BUS_H_ */
/** @} */
