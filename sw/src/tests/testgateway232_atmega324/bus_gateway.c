/*
 * bus_gateway.c
 *
 *  Created on: 17.01.2015
 *      Author: robert mueller
 */

#include "appconfig.h"
#include "bus_intern.h"
#include "queue.h"


eBusSendError_t busgateway_send_message (sBus_t* psBus, uint8_t len, uint8_t recH, uint8_t recL, queue_t* recQ, uint8_t oset)
{
    eBusSendError_t retval = eBUSERR_NOERR;

    do {
        // wake-up bus
        if(eMod_Sleeping == psBus->eModuleState) {
            bus_send_wakeupbyte(psBus);
            psBus->eModuleState = eMod_Running;
            bus_flush_bus(psBus);
        }

        // check length of message to be sent.
        if (0 == len || len > BUS_MAXSENDMSGLEN) {
            retval = eBUSERR_MSGSIZE;
            break;
        }

        // check if there is enough free space in the send queue
        // OVERALLLENGTH + SYNC + ADDR + LEN + RECV + EA + uLen
        if (bus_q_get_free(&psBus->tx_queue) < len + 6) {
            retval = eBUSERR_QUEUESPACE;
            break;
        }


        // save length of message in queue
        bus_q_put_byte(&psBus->tx_queue, len + 5); //SYNC + ADDR + LEN + RECV + EA (without CRC)

        // prepare message header
        bus_q_put_byte(&psBus->tx_queue, BUS_SYNCBYTE);
        bus_q_put_byte(&psBus->tx_queue, psBus->sCfg.uOwnNodeAddress & 0x007F);
        bus_q_put_byte(&psBus->tx_queue, len + 4); // RECV + EA + 2byte CRC
        bus_q_put_byte(&psBus->tx_queue, recL);
        // EA - Extended address 4bit sender in higher nibble, 4bit receiver
        // in lower nibble.
        bus_q_put_byte(&psBus->tx_queue, (recH & 0x0F) | ((psBus->sCfg.uOwnNodeAddress & 0x0F00) >> 4));

        // copy data
        while (len--) {
            bus_q_put_byte(&psBus->tx_queue, q_read_byte(recQ, oset++));
        }

    } while ( FALSE );

    return retval;
}


BOOL busgateway_forward_serial_message (queue_t *psRecQ, sBus_t *psBus)
{
    uint8_t uReceiverH = q_read_byte(psRecQ, 0);
    uint8_t uReceiverL = q_read_byte(psRecQ, 1);
    uint8_t uLen       = q_read_byte(psRecQ, 2);
    eBusSendError_t ret;
    /*
    q_put_byte(&g_sSerPhy.sSendQ, q_read_byte(&g_sSerPhy.sRecvQ, 0));
    q_put_byte(&g_sSerPhy.sSendQ, q_read_byte(&g_sSerPhy.sRecvQ, 1));
    q_put_byte(&g_sSerPhy.sSendQ, q_read_byte(&g_sSerPhy.sRecvQ, 2));
    q_put_byte(&g_sSerPhy.sSendQ, q_read_byte(&g_sSerPhy.sRecvQ, 3));
    q_put_byte(&g_sSerPhy.sSendQ, q_read_byte(&g_sSerPhy.sRecvQ, 4));
    q_put_byte(&g_sSerPhy.sSendQ, q_read_byte(&g_sSerPhy.sRecvQ, 5));
    q_put_byte(&g_sSerPhy.sSendQ, q_read_byte(&g_sSerPhy.sRecvQ, 6));
    serial_phy_initiate_sending();
    */


    ret = busgateway_send_message(psBus, uLen, uReceiverH, uReceiverL, psRecQ, 3);
    // if queue had temporary not enough space, the message remains in serial input-queue, otherwise it has to be removed.
    if(eBUSERR_QUEUESPACE != ret) {
        q_flush_bytes(psRecQ, uLen+3);
        if(eBUSERR_NOERR == ret) return TRUE;
    }
    return FALSE;
}
