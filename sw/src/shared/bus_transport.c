/**
 * @addtogroup BUS
 * @addtogroup BUS_TRANSPORT
 * @brief Transport protocol for sending and receiving messages on the bus.
 *
 * @{
 * @file    bus_transport.c
 * @brief   Transport protocol for sending and receiving messages on the bus.
 * @todo    1. Describe file purpose
 * @todo    2. Implement a queue of received messages instead of only one
            message. Many nodes can send one node a message.
 * @todo    3. Decide if an outgoing message queue is needed. For the first step
            surely not.
 * @todo    4. Further improvements :-)
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
#include <util/delay.h>
#endif

#include "bus_intern.h"
#include "cmddef_common.h"
#include "clock.h"
#include "crc16.h"
#include "sleepmode.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// Reset bus to Idle state.
static void reset_bus(sBus_t* psBus)
{
    psBus->eState = eBus_Idle;
    psBus->sRecvMsg.uOverallLength = 0;
    psBus->sRecvMsg.uLength = 0;
}

// Create empty message depending on bus's configuration.
static void create_empty_message(sBus_t* psBus)
{
    psBus->auEmptyMsg[0] = BUS_SYNCBYTE;
    psBus->auEmptyMsg[1] = psBus->sCfg.uOwnNodeAddress & 0x007F; // local address on bus
    psBus->auEmptyMsg[2] = 0; // length
}

// Start sending the wake-up-byte.
static BOOL send_wakeupbyte(sBus_t* psBus)
{
	uint8_t msg = BUS_WAKEUPBYTE;
	if (bus_phy_send(&psBus->sPhy, &msg, 1)) {
		while( bus_phy_sending(&psBus->sPhy) ) {}; // Wait till message is sent completely.
		return TRUE;
	}
	return FALSE;
}

// Send ACK-Byte.
static void send_ack(sBus_t* psBus)
{
    uint8_t ack = BUS_ACKBYTE;

    bus_phy_send(&psBus->sPhy, &ack, 1);
}

// Receive and interpret data.
static BOOL receive (sBus_t* psBus)
{
    uint16_t crc;
    uint8_t u;
    BOOL    bytereceived;

    do {
        if (!(bytereceived = bus_phy_data_received(&psBus->sPhy)) ||
            psBus->msg_receive_state == eBUS_RECV_MESSAGE) {
            break; // No byte received or message not retrieved.
        }
        bus_phy_read_byte(&psBus->sPhy, &u);

        // 1. byte: check sync byte
        if (psBus->sRecvMsg.uOverallLength == 0) {
            if (u != BUS_SYNCBYTE) {
                reset_bus(psBus);
                break; // not the sync byte, wait for next byte
            }

        // 2. byte: check token or sender byte
        } else if (psBus->sRecvMsg.uOverallLength == 1) {
            // token received?
            if (u & TOKENBIT) {
                // is it me?
                if ((u & ADDRMASK) == (psBus->sCfg.uOwnNodeAddress & 0x007f)) {
                    psBus->eState = eBus_GotToken;
                } else {
                    psBus->eState = eBus_Idle;
                }
                psBus->sRecvMsg.uOverallLength = 0;
                psBus->sRecvMsg.uLength = 0;
                break;
            }
            else {
                // message to be received. save sender-address lower byte
                psBus->sRecvMsg.uSender = u;
                psBus->eState = eBus_ReceivingActive;
            }
        }
        //TODO CV TODO RM: for performance, insert else if here?
        // If sender lower byte has been received OverallLength is 1
        // and therefore nothing is to do until:
        // psBus->sRecvMsg.auBuf[psBus->sRecvMsg.uOverallLength] = u;

        // active receiving state, receive and check message
        // and then go back to eBus_Idle state.
        if (psBus->eState == eBus_ReceivingActive) {
            // 3. byte: LE - Length of message from AR to CRCL
            if (psBus->sRecvMsg.uOverallLength == 2) {
                // check correctness of length
                if ((0==u)) {
                    // empty message received
                    psBus->eState = eBus_Idle;
                    psBus->sRecvMsg.uOverallLength = 0;
                    psBus->msg_receive_state = eBUS_RECV_EMPTY_MESSAGE;
                    break;
                }
                else if (u > BUS_MAXBIGMSGLEN) {
                    // length is zero or length is too big
                    reset_bus(psBus); // wait for next message on bus
                    break;
                }
                psBus->sRecvMsg.uLength = u;

            // 4. byte: AR - Address receiver 7bit
            } else if (psBus->sRecvMsg.uOverallLength == 3) {
                // check correctness of receiver address
                if (u & TOKENBIT) {
                    // bad receiver address. valid range is 0x00..0x7F
                    reset_bus(psBus);
                    break;
                }
                psBus->sRecvMsg.uReceiver = u;
                // hello, is it me you are looking for (or broadcast-message)?
                if ((BUS_BRDCSTADR == u) || (u == (psBus->sCfg.uOwnNodeAddress & 0x007f))) {
                    // nothing more to do here.
                } else {
                    // we are not interested in this message
                    psBus->eState = eBus_ReceivingPassive;
                }

            // 5. byte: EA - Extended address 4bit sender in higher nibble, 4bit receiver in lower nibble.
            } else if (psBus->sRecvMsg.uOverallLength == 4) {
                psBus->sRecvMsg.uSender |= (((uint16_t)u & 0x00F0) << 4);
                psBus->sRecvMsg.uReceiver |= (((uint16_t)u & 0x000F) << 8);
                // TODO check receiver address again

            // receive data (5th byte till length + 3(SY+AS+LE) - 2(CRC))
            } else if (psBus->sRecvMsg.uOverallLength > 4) {
                if (psBus->sRecvMsg.uOverallLength == (psBus->sRecvMsg.uLength + 3 - 2)) {
                    psBus->sRecvMsg.uCRC = u << 8;
                    // N th byte: CRCL - Low byte of 16bit CRC

                } else if (psBus->sRecvMsg.uOverallLength == (psBus->sRecvMsg.uLength + 3 - 1)) {
                    psBus->sRecvMsg.uCRC |= u;
                    crc = crc_calc16(&psBus->sRecvMsg.auBuf[0], psBus->sRecvMsg.uLength + 3 - 2);
                    if (crc == psBus->sRecvMsg.uCRC) {
                        // message has been received correctly
                        if(BUS_BRDCSTADR != psBus->sRecvMsg.uReceiver) {
                            // Send ACK if it was not a broadcast-message.
                            send_ack(psBus);
                        }
                        psBus->msg_receive_state = eBUS_RECV_MESSAGE;
            			psBus->sRecvMsg.uOverallLength = 0;
            			break;
            		} else {
            			// invalid crc of message
            			reset_bus(psBus);
            			break;
            		}

                } else if(psBus->sRecvMsg.uOverallLength >= psBus->sRecvMsg.uLength + 3) {
                    // invalid length of message
                    reset_bus(psBus);
                    break;
                }
            }
        }

        psBus->sRecvMsg.auBuf[psBus->sRecvMsg.uOverallLength] = u;
        psBus->sRecvMsg.uOverallLength++;

        // passive receiving state, only count bytes till the end of the message
        // and then go back to eBus_Idle state.
        if (psBus->eState == eBus_ReceivingPassive) {
            if (psBus->sRecvMsg.uOverallLength >= (psBus->sRecvMsg.uLength + 3 - 1)) {
                psBus->msg_receive_state = eBUS_RECV_FOREIGN_MESSAGE;
                reset_bus(psBus);
                // wait for ACK of receiver of foreign message
                clk_timer_start(&psBus->sAckTimeout, CLOCK_MS_2_TICKS(BUS_ACKTIMEOUT));
                psBus->eState = eBus_AckWaitReceiving;
            }
        }
    } while ( FALSE );

    return bytereceived;
}

// Check if data had to be sent or otherwise send empty message.
static void initiate_sending (sBus_t* psBus)
{
    uint16_t crc;
    uint8_t* msg_buf;
    uint8_t  q_msg_len, q_pending;

    psBus->eState = eBus_Sending;
    // is there a pending message to be sent?
    q_pending = bus_q_get_pending(&psBus->tx_queue);
    // if overall-length is set, message is completely enqueued
    if (q_pending > 0) {
        q_msg_len = bus_q_get_byte(&psBus->tx_queue);

        // check if read message length matches with the number of pending bytes
        if (q_msg_len > q_pending) {
            // the queue is corrupt
            bus_q_initialize(&psBus->tx_queue);
            // send empty message
            bus_phy_send(&psBus->sPhy, psBus->auEmptyMsg, BUS_EMPTY_MSG_LEN);
        }
        psBus->sSendMsg.uOverallLength = q_msg_len;

        // copy message from queue to send buffer
        msg_buf = psBus->sSendMsg.auBuf;
        while (q_msg_len--) {
            *msg_buf++ = bus_q_get_byte(&psBus->tx_queue);
        }

        // calculate checksum and reset the retry counter
        crc = crc_calc16(&psBus->sSendMsg.auBuf[0], psBus->sSendMsg.uOverallLength);
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = crc >> 8;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = crc & 0xFF;
        psBus->sSendMsg.uRetries = BUS_MAX_MSGRETRIES;

        // initiate sending of the message
        bus_phy_send(&psBus->sPhy,
                     psBus->sSendMsg.auBuf,
                     psBus->sSendMsg.uOverallLength);
    } else {
        // send empty message
        bus_phy_send(&psBus->sPhy, psBus->auEmptyMsg, BUS_EMPTY_MSG_LEN);
    }
}

// Check if data has been sent.
static void check_message_sent (sBus_t* psBus)
{
    // data completely sent?
    if (bus_phy_sending(&psBus->sPhy) == TRUE) return;
    // some retries left, then move to waiting for ack.
    if (0 < psBus->sSendMsg.uRetries) {
        psBus->sSendMsg.uRetries--;
        psBus->eState = eBus_AckWaitSending;
    } else {
        // empty message has been sent
        psBus->eState = eBus_Idle;
    }

}

// Wait for an acknowledge byte after message has been sent
static void wait_for_ack_after_sending (sBus_t* psBus)
{
    uint8_t u;
    BOOL    ack_received = FALSE;

    do {
        if (!bus_phy_data_received(&psBus->sPhy)) {
            break; // No byte received.
        }
        bus_phy_read_byte(&psBus->sPhy, &u);

        switch (u) {
        case BUS_ACKBYTE:
            psBus->sSendMsg.uRetries = 0;
            ack_received = TRUE;
            reset_bus(psBus);
            // TODO CV / TODO RM: notify application, that ACK byte has been received.
            break;

        default:
            // not the ACK byte, reset bus and try again in the next round
            reset_bus(psBus);
            break;
        }

        // has message been acknowledged at all?
        if (psBus->sSendMsg.uRetries == 0) {
            // end sending message
            psBus->sSendMsg.uOverallLength = 0;
            if (!ack_received) {
                // TODO CV / TODO RM: notify application, that ACK byte has not
                // been received.
            }
        }
    } while(FALSE);
}

// wait until an ACK byte has been sent by the receiver of the foreign message
static void wait_for_ack_passive (sBus_t* psBus)
{
    uint8_t u;

    do {
        if (clk_timer_is_elapsed(&psBus->sAckTimeout)) {
            // timeout.
            reset_bus(psBus);
            break;
        }
        else if (!bus_phy_data_received(&psBus->sPhy) ||
                 psBus->msg_receive_state == eBUS_RECV_MESSAGE) {
            break; // No byte received or message not retrieved.
        }
        bus_phy_read_byte(&psBus->sPhy, &u);

        if (u == BUS_ACKBYTE) {
            // ACK byte received, reset bus and wait for next token.
            reset_bus(psBus);
        }
    } while(FALSE);
}

// --- Module global functions -------------------------------------------------

/**
 * Reset bus state.
 *
 * @param[in]   psBus       Handle of the bus.
 */
void bus_trp_reset (sBus_t* psBus)
{
    reset_bus(psBus);
}

/**
 * Manage sending and receiving of messages.
 *
 * @param[in]   psBus       Handle of the bus.
 *
 * @returns TRUE, if there is a pending received message, otherwise FALSE.
 */
BOOL bus_trp_send_and_receive (sBus_t* psBus)
{
	uint8_t bb = 0;

    switch (psBus->eState) {
    case eBus_GotToken:
        // initiate sending of message
    	initiate_sending(psBus);
        break;

    case eBus_Sending:
        // message is currently being sent
        check_message_sent(psBus);
        break;

    case eBus_AckWaitSending:
        wait_for_ack_after_sending(psBus);
        break;

    case eBus_AckWaitReceiving:
        wait_for_ack_passive(psBus);
        break;

    case eBus_InitWait:
        if (bus_phy_data_received(&psBus->sPhy)) {
            psBus->eState = eBus_Idle;
            psBus->eModuleState = eMod_Running;
        } else {
            // wait until first byte is ready to be received
            break;
        }
        // otherwise fall through to eBus_Idle state
        // no break
    case eBus_Idle:
    case eBus_ReceivingWait:
    case eBus_ReceivingActive:
    case eBus_ReceivingPassive:
    default:
    	// repeat receiving while queue is not empty
        while (psBus->msg_receive_state != eBUS_RECV_MESSAGE && bb<2) {
        	if (receive(psBus) == FALSE) break;
        	bb++;
        }
        break;
    }

    return (psBus->msg_receive_state == eBUS_RECV_MESSAGE);
}

/**
 * Start sending of sleep command.
 *
 * @param[in]   psBus       Handle of the bus.
 *
 * @returns TRUE, if the sleep command has been sent, otherwise FALSE.
 */
BOOL bus_trp_send_sleepcmd (sBus_t* psBus)
{
    uint16_t crc;
    uint8_t msg[8];

    msg[0] = BUS_SYNCBYTE;                  // SY - Syncronization byte: '�' = 0b10011010
    msg[1] = psBus->sCfg.uOwnNodeAddress;   // AS - Address sender on bus 7bit
    msg[2] = sizeof(msg) - 3;               // LE - Length of message from AR to CRCL
    msg[3] = 0x00;                          // AR - Address receiver 7bit (Broadcast)
    msg[4] = 0x00;                          // EA - Extended address 4bit sender in higher nibble, 4bit receiver in lower nibble.
    msg[5] = CMD_eSleep;                    // MD - Sleep-Command
    crc = crc_calc16(&msg[0], 6);
    msg[6] = crc >> 8;
    msg[7] = crc & 0xFF;

    if( bus_phy_send(&psBus->sPhy, msg, sizeof msg) ) {
        while( bus_phy_sending(&psBus->sPhy) ) {}; // Wait till message is sent completely.
        return TRUE;
    }
    return FALSE;
}

// --- Global functions --------------------------------------------------------

/**
 * Configure the bus interface and data.
 * @note Has to be called before bus_initialize() is called the first time.
 *
 * @param[in]   psBus       Handle of the bus.
 * @param[in]   uNodeAddress Address of this node.
 */
void bus_configure (sBus_t* psBus, uint16_t uNodeAddress)
{
    psBus->sCfg.uOwnNodeAddress = uNodeAddress;
    create_empty_message(psBus);
}

/**
 * Initialize Bus interface.
 *
 * @param[in]   psBus       Handle of the bus.
 * @param[in]   uUart       Number of the UART. 0=first.
 */
void bus_initialize (sBus_t* psBus, uint8_t uUart)
{
    psBus->eState = eBus_InitWait;
    psBus->sSendMsg.uRetries = 0;
    psBus->sSendMsg.uOverallLength = 0;
    psBus->eModuleState = eMod_Sleeping;
    bus_phy_initialize(&psBus->sPhy, uUart);
    bus_flush_bus(psBus);
    bus_q_initialize(&psBus->tx_queue);
}

/**
 * Flushes phy-buffer and resets bus.
 *
 * @param[in]   psBus       Handle of the bus.
 */
void bus_flush_bus (sBus_t* psBus)
{
	bus_phy_flush(&psBus->sPhy);
	reset_bus(psBus);
}

/**
 * Checks if there is a pending message to be read.
 *
 * @param[in]   psBus       Handle of the bus.
 *
 * @returns TRUE, if there is a pending message.
 */
BOOL bus_get_message (sBus_t* psBus)
{
    return bus_trp_send_and_receive(psBus);
}

/**
 * Reads a pending message.
 *
 * @param[in] 	psBus       Handle of the bus.
 * @param[out] 	puSender    Sender of the message.
 * @param[out] 	puLen       (Netto) Length of the message.
 * @param[out]  puMsg       Received message.
 *
 * @returns TRUE, if a message has been received and.
 */
BOOL bus_read_message (sBus_t*  psBus,
                       uint16_t* puSender,
                       uint8_t* puLen,
                       uint8_t* puMsg)
{
    uint8_t len = 0;

    do {
        // is there a new message pending?
        if (psBus->msg_receive_state != eBUS_RECV_MESSAGE) {
            break;
        }
        *puSender   = psBus->sRecvMsg.uSender;

        while (len < psBus->sRecvMsg.uLength - 4) {
            puMsg[len] = psBus->sRecvMsg.auBuf[5 + len];
            len ++;
        }
        // reset bus to IDLE state, so we are ready to receive the next message
        psBus->msg_receive_state = eBUS_RECV_NOTHING;
        psBus->sRecvMsg.uLength = 0;
        psBus->sRecvMsg.uOverallLength = 0;
        psBus->eState = eBus_Idle;

        *puLen      = len;
        return TRUE;
    } while ( FALSE );

    return FALSE;
}

/**
 * Send a message.
 *
 * @param[in]   psBus       Handle of the bus.
 * @param[in]   uReceiver   Receiver address of the message.
 * @param[in]   uLen        (Netto) Length of the message.
 * @param[in]   puMsg       Message to send.
 *
 * @returns TRUE, if the message has successfully been queued.
 * @note Use bus_is_idle() to check if message is successfully transmitted.
 */
BOOL bus_send_message (sBus_t*    psBus,
                       uint16_t   uReceiver,
                       uint8_t    uLen,
                       uint8_t*   puMsg)
{
    uint8_t  overall_msg_len;

    do {
        // wake-up bus
        if(eMod_Sleeping == psBus->eModuleState) {
        	send_wakeupbyte(psBus);
        	psBus->eModuleState = eMod_Running;
        	bus_flush_bus(psBus);
        }

    	// check length of message to be sent.
        if (uLen == 0 ||
            uLen > BUS_MAXMSGLEN) {
        	break;
        }

        // check if there is enough free space in the send queue
        // OVERALLLENGTH + SYNC + ADDR + LEN + RECV + EA + uLen
        if (bus_q_get_free(&psBus->tx_queue) < uLen + 6) {
            break;
        }

        // save length of message in queue
        overall_msg_len = uLen + 5; //SYNC + ADDR + LEN + RECV + EA (without CRC)
        bus_q_put_byte(&psBus->tx_queue, overall_msg_len);

        // prepare message header
        bus_q_put_byte(&psBus->tx_queue, BUS_SYNCBYTE);
        bus_q_put_byte(&psBus->tx_queue, psBus->sCfg.uOwnNodeAddress & 0x007F);
        bus_q_put_byte(&psBus->tx_queue, uLen + 4); // RECV + EA + 2byte CRC
        bus_q_put_byte(&psBus->tx_queue, uReceiver & 0x007F);
        // EA - Extended address 4bit sender in higher nibble, 4bit receiver
        // in lower nibble.
        bus_q_put_byte(&psBus->tx_queue, ((uReceiver & 0x0F00) >> 8) |
                          ((psBus->sCfg.uOwnNodeAddress & 0x0F00) >> 4));

        // copy data
        while (uLen--) {
            bus_q_put_byte(&psBus->tx_queue, *puMsg++);
        }

        return TRUE;
    } while ( FALSE );

    return FALSE;
}

/**
 * Send a ACK-message.
 *
 * @param[in]   psBus       Handle of the bus.
 *
 * @returns TRUE, if the message has successfully been queued.
 * @note Use bus_is_idle() to check if message is successfully transmitted.
 */
BOOL bus_send_ack_message (sBus_t* psBus, uint16_t uReceiver)
{
    uint16_t crc;

    do {
        // prepare message header
        psBus->sSendMsg.uOverallLength = 0;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = BUS_SYNCBYTE;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = psBus->sCfg.uOwnNodeAddress & 0x007F;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = 5;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = uReceiver & 0x007F;
        // EA - Extended address 4bit sender in higher nibble, 4bit receiver in lower nibble.
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] =
            (((uReceiver & 0x0F00) >> 8) |
            ((psBus->sCfg.uOwnNodeAddress & 0x0F00) >> 4));
        // copy data
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = CMD_eAck;
        // calculate and send CRC
        crc = crc_calc16(&psBus->sSendMsg.auBuf[0], psBus->sSendMsg.uOverallLength);
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = crc >> 8;
        psBus->sSendMsg.auBuf[psBus->sSendMsg.uOverallLength++] = crc & 0xFF;
        return TRUE;
    } while ( FALSE );
    return FALSE;
}

/**
 * Check if bus is in IDLE state.
 *
 * @param[in]   psBus       Handle of the bus.
 * @returns TRUE, if bus is IDLE.
 */
BOOL bus_is_idle (sBus_t*       psBus)
{
    return (psBus->eState == eBus_Idle);
}

/**
 * Set module into sleep mode.
 *
 * @param[in]   psBus       Handle of the bus.
 */
void bus_sleep (sBus_t*       psBus)
{
	psBus->eModuleState = eMod_Sleeping;
    // disable clock-timer, otherwise IRQ will cause immediate wakeup.
	clk_control(FALSE);

	// sleep till byte is received.
	sleep_set_mode(SLEEP_MODE_IDLE);
	sleep_activate();

	// ...sleeping... zzzZZZ

	sleep_delay_ms(1);      // wait for sys-clock becoming stable

	bus_flush_bus(psBus);   // Clean bus-buffer
	clk_control(TRUE);      // enable clock-timer

	// wait for first pending byte, then set module to running state
	psBus->eState = eBus_InitWait;
}

/** @} */
/** @} */
