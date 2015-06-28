/**
 * @addtogroup BUS
 * @addtogroup BUS_INTERN
 * @brief Internal definitions for bus modules.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    bus_intern.h
 * @brief   Internal definitions for bus modules.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _BUS_INTERN_H_
#define _BUS_INTERN_H_

// --- Include section ---------------------------------------------------------

#include "bus.h"

// --- Definitions -------------------------------------------------------------

#define BUS_BAUDRATE            38400
#define BUS_MESSAGE_TIMEOUT     50      //!< Timeout in ms between sent token and complete received message.
#define BUS_ACK_TIMEOUT         30      //!< Timeout in ms between sent message and acknowledge byte.


#define TOKENBIT 0x80
#define ADDRMASK ~(TOKENBIT)

#define BUS_MAX_ANSWERFAILS		5	    //!< Maximum number a member's answer can be missing, before disconnecting.
#define BUS_MAX_MSGRETRIES		3	    //!< Maximum number of re-sending one message.
#define BUS_LOOPS_TILL_SLEEP	3       //!< Number of discovery-loops till going to sleep.

#define BUS_ACKBYTE             0x06    //!< ACK.
#define BUS_NAKBYTE             0x15    //!< NAK.
#define BUS_SYNCBYTE            0x9A    //!< 0b10011010 synchronization byte
                                        //!< every message begins with.
#define BUS_WAKEUPBYTE          0xFF    //!< Dummy-Byte that is send to the bus
                                        //!< as wakeup-call for other subscribers.

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

// queue
//void    bus_q_initialize            (queue_t *q);

//uint8_t bus_q_get_free              (queue_t *q);

//uint8_t bus_q_get_pending           (queue_t *q);

//void    bus_q_put_byte              (queue_t *q, uint8_t byte);

//uint8_t bus_q_get_byte              (queue_t *q);

// transport layer
void    bus_trp_reset               (sBus_t* psBus);

BOOL    bus_trp_send_and_receive    (sBus_t* psBus);

BOOL    bus_trp_send_sleepcmd       (sBus_t* psBus);

// physical layer
void    bus_phy_initialize          (sBusPhy_t*     psPhy,
                                     uint8_t        uUart);

void    bus_phy_activate_sender     (sBusPhy_t*     psPhy,
                                     BOOL           bActivate);

void    bus_phy_activate_receiver   (sBusPhy_t*     psPhy,
                                     BOOL           bActivate);

BOOL    bus_phy_send                (sBusPhy_t*     psPhy,
                                     const uint8_t* puMsg,
                                     uint8_t        uLen);

BOOL    bus_phy_sending             (sBusPhy_t*     psPhy);

BOOL    bus_phy_data_received       (sBusPhy_t*     psPhy);

uint8_t bus_phy_read                (sBusPhy_t*     psPhy,
                                     uint8_t*       puInBuf);

BOOL    bus_phy_read_byte           (sBusPhy_t*     psPhy,
                                     uint8_t*       puByte);

void 	bus_phy_flush				(sBusPhy_t* psPhy);

// --- Global functions --------------------------------------------------------

#endif /* _BUS_INTERN_H_ */
/** @} */
/** @} */
