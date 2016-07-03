/**
 * @addtogroup BUS_GATEWAY
 * @brief Gateway to transmit messages from bus to serial line and vice versa.
 *
 * @{
 * @file    bus_gateway.c
 * @brief   Gateway to transmit messages from bus to serial line and vice versa.
 * Serial format of a message:
 * [RECEIVER-ADDR][LENGTH][DATA]       [NEWLINE]
 * [HH HL LH LL]  [H L]   [0..LENGTH-1][\n]
 * @author  Robert Mueller, Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "appconfig.h"
#include "bus_intern.h"
#include "cmddef_common.h"
#include "queue.h"
#include "serialcomm.h"

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
 * @param[in] Handle to bus.
 * @param[in] Handle to serial line.
 *
 * @returns TURE, if message was correctly read from bus and sent to serial
 * line transmit buffer.
 */
bool bgw_forward_bus_msg (sBus_t *bus, scomm_phy_t *serial)
{
    uint8_t  len = 0;
    uint8_t  length = 0;
    uint16_t sender = 0;

    do {
        // is there a new message pending?
        if (bus->msg_receive_state != eBUS_RECV_MESSAGE) {
            return false;
        }
        sender   = bus->sRecvMsg.uSender;
        length   = bus->sRecvMsg.length - 4;
        // create message header on serial send queue
        convert_and_enqueue_byte(&serial->sendQ, (uint8_t)((sender & 0xFF00)>>8));
        convert_and_enqueue_byte(&serial->sendQ, (uint8_t)( sender & 0x00FF));
        convert_and_enqueue_byte(&serial->sendQ, length);
        // convert and copy message data
        while (len < length) {
            convert_and_enqueue_byte(&serial->sendQ, bus->sRecvMsg.auBuf[5 + len]);
            len ++;
        }
        // finally end message with a newline char.
        q_put_byte(&serial->sendQ, '\n');
        serial_phy_initiate_sending(serial);
        // reset bus to IDLE state, so we are ready to receive the next message
        bus->msg_receive_state = eBUS_RECV_NOTHING;
        bus->sRecvMsg.length = 0;
        bus->sRecvMsg.uOverallLength = 0;
        bus->eState = eBus_Idle;

        if (eCMD_SLEEP == bus->sRecvMsg.auBuf[5]) {
            //sleep_pinchange2_enable();
            bus_sleep(bus);
            //sleep_pinchange2_disable();
        }
    } while ( false );
    return true;
}
