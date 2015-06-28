/*
 * bus_gateway.c
 *
 *  Created on: 17.01.2015
 *      Author: robert mueller
 */

#include <avr/io.h>

#include "appconfig.h"
#include "bus_intern.h"
#include "queue.h"
#include "serialcomm.h"

typedef enum bgw_state {
    eBGW_IDLE = 0,
    eBGW_WAIT_COMPLETE,
    eBGW_RECV,
    eBGW_ERROR
} bgw_state_t;

static bgw_state_t  g_curr_state;
static uint8_t      g_curr_recv_h;
static uint8_t      g_curr_recv_l;
static uint8_t      g_curr_len;

static void debug_sendval8 (scomm_phy_t* phy, uint8_t val)
{
    uint8_t nibble;

    nibble = (val & 0xF0) >> 4;
    if (nibble > 9) {
        q_put_byte(&phy->sendQ, nibble + 'A' - 10);
    } else {
        q_put_byte(&phy->sendQ, nibble + '0');
    }
    nibble = (val & 0x0F);
    if (nibble > 9) {
        q_put_byte(&phy->sendQ, nibble + 'A' - 10);
    } else {
        q_put_byte(&phy->sendQ, nibble + '0');
    }
    q_put_byte(&phy->sendQ, '\n');
    serial_phy_initiate_sending(phy);
}

static void debug_sendtrap (scomm_phy_t* phy)
{
    q_put_byte(&phy->sendQ, 'T');
    q_put_byte(&phy->sendQ, 'R');
    q_put_byte(&phy->sendQ, 'A');
    q_put_byte(&phy->sendQ, 'P');
    q_put_byte(&phy->sendQ, '\n');
    serial_phy_initiate_sending(phy);
}

static BOOL convert_nibbles_to_byte (uint8_t nibble_h, uint8_t nibble_l, uint8_t* byte)
{
    if (nibble_h >= '0' && nibble_h <= '9') {
        nibble_h -= '0';
    } else if (nibble_h >= 'a' && nibble_h <= 'f') {
        nibble_h -= ('a' - 10);
    } else if (nibble_h >= 'A' && nibble_h <= 'F') {
        nibble_h -= ('A' - 10);
    } else {
        *byte = 0;
        return FALSE;
    }
    *byte = nibble_h << 4;

    if (nibble_l >= '0' && nibble_l <= '9') {
        nibble_l -= '0';
    } else if (nibble_l >= 'a' && nibble_l <= 'f') {
        nibble_l -= ('a' - 10);
    } else if (nibble_l >= 'A' && nibble_l <= 'F') {
        nibble_l -= ('A' - 10);
    } else {
        *byte = 0;
        return FALSE;
    }
    *byte |= nibble_l;
    return TRUE;
}

static bgw_state_t bgw_send_message_to_bus (sBus_t* bus, scomm_phy_t* rs232_phy)
{
    uint16_t saved_writepos;
    uint8_t nibble_h, nibble_l, byte = 0, num_read = 0;

    // wake-up bus
    if (eMod_Sleeping == bus->eModuleState) {
        bus_send_wakeupbyte(bus);
        bus->eModuleState = eMod_Running;
        bus_flush_bus(bus);
    }

    // check if there is enough free space in the send queue
    // OVERALLLENGTH + SYNC + ADDR + LEN + RECV + EA + uLen
    if (q_get_free(&bus->tx_queue) < g_curr_len + 6) {
        return eBGW_RECV;
    }
    // save old bus queue write pointer
    saved_writepos = bus->tx_queue.writepos;
    // save length of message in queue
    q_put_byte(&bus->tx_queue, g_curr_len + 5); //SYNC + ADDR + LEN + RECV + EA (without CRC)

    // prepare message header
    q_put_byte(&bus->tx_queue, BUS_SYNCBYTE);
    q_put_byte(&bus->tx_queue, bus->sCfg.uOwnNodeAddress & 0x007F);
    q_put_byte(&bus->tx_queue, g_curr_len + 4); // RECV + EA + 2byte CRC
    q_put_byte(&bus->tx_queue, g_curr_recv_l);
    // EA - Extended address 4bit sender in higher nibble, 4bit receiver
    // in lower nibble.
    q_put_byte(&bus->tx_queue, (g_curr_recv_h & 0x0F) | ((bus->sCfg.uOwnNodeAddress & 0x0F00) >> 4));

    // copy data
    while (g_curr_len--) {
        nibble_h = q_read_byte(&rs232_phy->recvQ, num_read++);
        nibble_l = q_read_byte(&rs232_phy->recvQ, num_read++);
        if (!convert_nibbles_to_byte(nibble_h, nibble_l, &byte)) {
            bus->tx_queue.writepos = saved_writepos;
            return eBGW_ERROR;
        }
        q_put_byte(&bus->tx_queue, byte);
    }
    q_flush_bytes(&rs232_phy->recvQ, num_read);

    // check for final newline
    byte = q_get_byte(&rs232_phy->recvQ);
    if (byte != '\n') {
        bus->tx_queue.writepos = saved_writepos;
        return eBGW_ERROR;
    }

    return eBGW_IDLE;
}

void bgw_forward_serial_msg (sBus_t* bus, scomm_phy_t* rs232_phy)
{
    uint8_t nibble_h, nibble_l, num_bytes;

    switch (g_curr_state) {
    case eBGW_IDLE:
        // check if message header is already in the queue
        if (q_get_pending(&rs232_phy->recvQ) >= 6) {
            // get receiver address
            nibble_h = q_read_byte(&rs232_phy->recvQ, 0);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 1);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_recv_h)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            nibble_h = q_read_byte(&rs232_phy->recvQ, 2);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 3);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_recv_l)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            // get and check message size
            nibble_h = q_read_byte(&rs232_phy->recvQ, 4);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 5);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_len)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            if (g_curr_len == 0 || g_curr_len > BUS_MAXSENDMSGLEN) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            q_flush_bytes(&rs232_phy->recvQ, 6);
            g_curr_state = eBGW_WAIT_COMPLETE;
            debug_sendtrap(rs232_phy);
            debug_sendval8(rs232_phy, g_curr_len);
        }
        break;

    case eBGW_WAIT_COMPLETE:
        // wait until complete message incuding newline is in the queue
        if (q_get_pending(&rs232_phy->recvQ) >= ((g_curr_len<<1) + 1)) {

            g_curr_state = eBGW_RECV;
        }
        break;

    case eBGW_RECV:
        g_curr_state = bgw_send_message_to_bus(bus, rs232_phy);
        break;

    //case eBGW_ERROR:
    default:

        // error state: read until next newline character
        num_bytes = q_get_pending(&rs232_phy->recvQ);
        while (num_bytes--) {
            if (q_get_byte(&rs232_phy->recvQ) == '\n') {
                g_curr_state = eBGW_IDLE;
                LED_STATUS_TOGGLE;
                break;
            }
        }
        break;
    }
    serial_phy_check_q_level(rs232_phy);
}

/*
BOOL busgateway_forward_serial_message (scomm_phy_t* phy, sBus_t *bus)
{
    uint8_t receiver_h = 0, receiver_l = 0, len = 0;

    BOOL    ret;

    // read receiver high byte
    receiver_h = q_read_byte(&phy->recvQ, num_read++);
    // read receiver low byte
    receiver_l = q_read_byte(&phy->recvQ, num_read++);
    // read length byte
    len = q_read_byte(&phy->recvQ, num_read++);

    // check length of message
    if ((q_get_pending(&phy->recvQ) < len+3) ||
        0 == len ||
        len > BUS_MAXSENDMSGLEN) {
        serial_phy_discard_messages(phy); // discard complete write buffer
        return FALSE;
    }

    ret = busgateway_send_message(bus, receiver_h, receiver_l, len, &phy->recvQ, &num_read);
    // if queue had temporary not enough space, the message remains in
    // serial input-queue, otherwise it has to be removed.
    if (ret == TRUE) {
        // message sent or to be discarded
        serial_phy_finish_message(phy, num_read);
    }
    // if (ret == FALSE): message remains in receive queue. Try again later.

    return FALSE;
}
*/
