/**
 * @addtogroup BUS_GATEWAY
 * @brief Gateway to transmit messages from bus to serial line and vice versa.
 *
 * @{
 * @file    bus_gateway.c
 * @brief   Gateway to transmit messages from bus to serial line and vice versa.
 * Serial format of a message:
 * [SENDER-ADDR][RECEIVER-ADDR][LENGTH][DATA]       [NEWLINE]
 * [HH HL LH LL][HH HL LH LL]  [H L]   [0..LENGTH-1][\n]
 *
 * @author  Robert Mueller, Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "appconfig.h"
#include "bus_intern.h"
#include "cmddef_common.h"
#include "queue.h"
#include "serialcomm.h"
#include "sleepmode.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum bgw_state {
    eBGW_IDLE = 0,
    eBGW_WAIT_COMPLETE,
    eBGW_RECV,
    eBGW_ERROR
} bgw_state_t;

// --- Local variables ---------------------------------------------------------

static bgw_state_t  g_curr_state;
static uint8_t      g_curr_recv_h;
static uint8_t      g_curr_recv_l;
static uint8_t      g_curr_send_h;
static uint8_t      g_curr_send_l;
static uint8_t      g_curr_len;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/*static void debug_sendval8 (scomm_phy_t* phy, uint8_t val)
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
}*/

static bool convert_nibbles_to_byte (uint8_t nibble_h, uint8_t nibble_l, uint8_t* byte)
{
    if (nibble_h >= '0' && nibble_h <= '9') {
        nibble_h -= '0';
    } else if (nibble_h >= 'a' && nibble_h <= 'f') {
        nibble_h -= ('a' - 10);
    } else if (nibble_h >= 'A' && nibble_h <= 'F') {
        nibble_h -= ('A' - 10);
    } else {
        *byte = 0;
        return false;
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
        return false;
    }
    *byte |= nibble_l;
    return true;
}

static void convert_and_enqueue_byte (queue_t *q, uint8_t byte)
{
    uint8_t nibble;

    nibble = byte >> 4;
    if (nibble < 10) {
        q_put_byte(q, nibble + '0');
    } else {
        q_put_byte(q, nibble + 'A' - 10);
    }
    nibble = byte & 0x0F;
    if (nibble < 10) {
        q_put_byte(q, nibble + '0');
    } else {
        q_put_byte(q, nibble + 'A' - 10);
    }
}

static bgw_state_t bgw_send_message_to_bus (sBus_t* bus, scomm_phy_t* rs232_phy)
{
    uint16_t saved_writepos;
    uint8_t nibble_h, nibble_l, byte = 0, num_read = 0;

    // wake-up bus
    if (eMod_Sleeping == bus->eModuleState) {
        sleep_request(false);
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
    q_put_byte(&bus->tx_queue, (g_curr_recv_h & 0x0F) | (bus->sCfg.uOwnExtAddress << 4));

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

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Receive a message on a serial line and forward it to the bus.
 * This is implemented as a state-macine and this function has to be called
 * periodically.
 *
 * @param[in] bus       Handle of bus.
 * @param[in] rs232_phy Handle of serial line.
 */
void bgw_forward_serial_msg (sBus_t* bus, scomm_phy_t* rs232_phy)
{
    uint8_t nibble_h, nibble_l, num_bytes;

    switch (g_curr_state) {
    case eBGW_IDLE:
        // check if message header is already in the queue
        if (q_get_pending(&rs232_phy->recvQ) >= 10) {
            // get sender's address
            nibble_h = q_read_byte(&rs232_phy->recvQ, 0);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 1);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_send_h)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            nibble_h = q_read_byte(&rs232_phy->recvQ, 2);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 3);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_send_l)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            // get receiver address
            nibble_h = q_read_byte(&rs232_phy->recvQ, 4);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 5);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_recv_h)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            nibble_h = q_read_byte(&rs232_phy->recvQ, 6);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 7);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_recv_l)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            // get and check message size
            nibble_h = q_read_byte(&rs232_phy->recvQ, 8);
            nibble_l = q_read_byte(&rs232_phy->recvQ, 9);
            if (!convert_nibbles_to_byte(nibble_h, nibble_l, &g_curr_len)) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            if (g_curr_len == 0 || g_curr_len > BUS_MAXSENDMSGLEN) {
                g_curr_state = eBGW_ERROR;
                break;
            }
            q_flush_bytes(&rs232_phy->recvQ, 10);
            sleep_request(false);
            g_curr_state = eBGW_WAIT_COMPLETE;
        }
        break;

    case eBGW_WAIT_COMPLETE:
        // wait until complete message including newline is in the queue
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
                break;
            }
        }
        g_curr_state = eBGW_IDLE;
        break;
    }
    serial_phy_check_q_level(rs232_phy);
}

/**
 * Forward a message from bus to serial line.
 *
 * @param[in]   bus     Handle to bus.
 * @param[in]   serial  Handle to serial line.
 * @param[out]  sender  Sender of received message.
 * @param[out]  len     Length of received message.
 * @param[out]  msg     Message payload.
 *
 * @returns true if a message for the gateway module was received.
 */
bool bgw_forward_bus_msg (sBus_t *bus, scomm_phy_t *serial,
                          uint16_t* sender, uint8_t* len, uint8_t* msg)
{
    bool     msg_for_me, msg_for_others;
    uint8_t  l_curr;
    uint8_t  l_expected;
    uint16_t s, r;

    msg_for_me = false;

    do {
        // is there a new message pending?
        if (bus->msg_receive_state != eBUS_RECV_MESSAGE) break;

        msg_for_others = true;
        s = bus->sRecvMsg.uSender;
        r = bus->sRecvMsg.uReceiver;
        l_expected = bus->sRecvMsg.length - 4;

        if (r == bus->sCfg.uOwnAddress) {
            msg_for_me = true;
           msg_for_others = false;
        } 
        else if (r == (bus->sCfg.uOwnAddress & BUS_SEGBRDCSTMASK)) {
            msg_for_me = true;
            msg_for_others = false;
        }
        else if (r == BUS_BRDCSTADR) {
            msg_for_me = true;
        }

        l_curr = 0;
        if (msg_for_others) {
            // create message header on serial send queue
            convert_and_enqueue_byte(&serial->sendQ, (uint8_t)((s & 0xFF00)>>8));
            convert_and_enqueue_byte(&serial->sendQ, (uint8_t)( s & 0x00FF));
            convert_and_enqueue_byte(&serial->sendQ, (uint8_t)((r & 0xFF00)>>8));
            convert_and_enqueue_byte(&serial->sendQ, (uint8_t)( r & 0x00FF));
            convert_and_enqueue_byte(&serial->sendQ, l_expected);
        }
        // convert and copy message data
        while (l_curr < l_expected) {
            if (msg_for_others) convert_and_enqueue_byte(&serial->sendQ, bus->sRecvMsg.auBuf[5 + l_curr]);
            if (msg_for_me) msg[l_curr] = bus->sRecvMsg.auBuf[5 + l_curr];
            l_curr++;
        }
        // finally end message with a newline char.
        if (msg_for_others) {
            q_put_byte(&serial->sendQ, '\n');
            serial_phy_initiate_sending(serial);
        }
        if (msg_for_me) {
            *sender = s;
            *len = l_curr;
        }

        // reset bus to IDLE state, so we are ready to receive the next message
        bus->msg_receive_state = eBUS_RECV_NOTHING;
        bus->sRecvMsg.length = 0;
        bus->sRecvMsg.uOverallLength = 0;
        bus->eState = eBus_Idle;
    } while ( false );
    return msg_for_me;
}
