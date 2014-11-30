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
 #define BUS_TX_QUEUE_SIZE  100
#endif
/**
 * @}
 */

#define BUS_MAXMSGLEN       16
//! length of message including header and footer
#define BUS_MAXTOTALMSGLEN  (BUS_MAXMSGLEN + 7)
#define BUS_MAXBIGMSGLEN    (64 + 2)
#define BUS_EMPTY_MSG_LEN   3
#define BUS_TOKEN_MSG_LEN   2

#define BUS_FIRSTNODE       0x01
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

//! configuration data of the bus
typedef struct busconfig {
    uint16_t       uOwnAddress;
    uint8_t        uOwnNodeAddress;    //!< address of node on bus.
    uint8_t        uOwnExtAddress;     //!< extensionaddress of node on bus.
    bool           router_mode;
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
    uint8_t         uOverallLength;
    uint8_t         uRetries;       //!< number of send retries.
    auSndBuf_t      auBuf;          //!< message data including header, data and crc.
} sSndBusMsg_t;

typedef struct queue {
    uint8_t pread;
    uint8_t pwrite;
    uint8_t data[BUS_TX_QUEUE_SIZE];
} queue_t;

//! data of bus. used as handle for all public methods.
typedef struct bus {
	eModState_t		    eModuleState;					//!< current state of the module.
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

void 	bus_configure						(sBus_t* 		psBus,
											uint16_t 		uNodeAddress);

void 	bus_initialize						(sBus_t* 		psBus,
											 uint8_t 	    uUart);

void    bus_set_router_mode                 (sBus_t*        bus,
                                             bool           router_mode);

void 	bus_flush_bus						(sBus_t* 		psBus);

BOOL 	bus_get_message						(sBus_t* 		psBus);

BOOL 	bus_read_message					(sBus_t*        psBus,
											 uint16_t*      puSender,
											 uint8_t*       puLen,
										     uint8_t*       puMsg);

BOOL    bus_read_message_verbose            (sBus_t*        psBus,
                                             uint16_t*      puSender,
                                             uint16_t*      puReceiver,
                                             uint8_t*       puLen,
                                             uint8_t*       puMsg,
                                             uint16_t*      puCRC);

BOOL 	bus_send_message					(sBus_t*    	psBus,
                      	  	  	  	  	  	 uint16_t   	uReceiver,
                      	  	  	  	  	  	 uint8_t    	uLen,
                      	  	  	  	  	  	 uint8_t*   	puMsg);

BOOL 	bus_is_idle							(sBus_t*        psBus);

void    bus_sleep                           (sBus_t*      	psBus);

#endif /* _BUS_H_ */
/** @} */
