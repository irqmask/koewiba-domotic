/*
 * serialcomm.h
 *
 *  Created on: 28.12.2014
 *      Author: robert mueller
 */

#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_

#include "queue.h"

typedef enum serialflags {
    e_scommrxflag    = 0b00000001,
    e_scommtxflag    = 0b00000010,
    e_scommrxerrflag = 0b00000100,
    e_scommtxerrflag = 0b00001000,
    e_scommtimeout   = 0b00010000,
    e_scommspacelow  = 0b00100000
} eSerialFlags_t;

//! serial receive queue
typedef struct serialBuf {
    uint8_t         uReadPos;
    uint8_t         uWritePos;
    uint8_t         auBuf[100];
} sQueue_t;

//! serial receive structure
typedef struct serialMsg {
    uint8_t         uReceiver;
    uint8_t         uLength;
    sQueue_t        queue;
} sSerMsg_t;

//! data of the physical layer
typedef struct serialphy {
    uint8_t         uUart;
    eSerialFlags_t  uFlags;
    queue_t        sRecvQ;
    queue_t        sSendQ;
} sSerPhy_t;

// --- Global variables --------------------------------------------------------
extern sSerMsg_t   g_sSCom; // Serial-Communication

inline void set_cts_pin(void)
{
    SCOMM_PORTOUT |= (1<<SCOMM_CTS);
};
inline void clear_cts_pin(void)
{
    SCOMM_PORTOUT &= ~(1<<SCOMM_CTS);
};
inline bool cts_state(void)
{
    return SCOMM_PORTIN & (1<<SCOMM_CTS);
};


void scomm_initialize_uart1     (sSerPhy_t *psPhy);
BOOL serial_phy_initiate_sending(sSerPhy_t *psPhy);
BOOL serial_phy_msg_received    (sSerPhy_t* psPhy);

#endif /* SERIALCOMM_H_ */
