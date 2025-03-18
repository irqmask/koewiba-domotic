/**
 * @addtogroup BUS_GATEWAY
 * @brief Serial line driver for receiving and sending of messages.
 *
 * IMPORTANT: Dependent on the controller used for serialcomm the receive-queue
 * is not as huge as the computers serial buffer is. Hence it is important to
 * reduce the block-size of bytes that will be send by the computers serial
 * interface. Please choose 64 Byte and set flow control to 'hardware',
 * otherwise it will not work properly.
 *
 * @{
 * @file    serialcomm.c
 * @brief   Serial line driver for receiving and sending of messages.
 *
 * @author  Robert Mueller, Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "pcbconfig.h"

#include "led_debug.h"
#include "serialcomm.h"
#include "ucontroller.h"
#include "queue.h"

// --- Definitions -------------------------------------------------------------

#define SERIAL_BAUDRATE 57600

#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * SERIAL_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

#define RECV_Q_MAX_CTS_THD 80
#define RECV_Q_MIN_CTS_THD (64+3)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// uart1 phy data
static scomm_phy_t  *g_uart1phy = NULL;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Interrupts --------------------------------------------------------------

/**
 * Received byte interrupt 1.
 */
ISR (INTERRUPT_USART_RXC1)
{
    uint8_t    received_char, fe;
    queue_t    *q = &g_uart1phy->recvQ;

    // check for framing error
    fe = (REGISTER_UCSRA1 & (1<<REGBIT_FE));
    // read data to clear interrupt flag
    received_char = REGISTER_UDR1;

    // frame error ?
    if (0 != fe) {
        REGISTER_UCSRA1 &= ~(1<<REGBIT_FE); // Reset frame error flag
        // write invalid char to queue, to mark frame error
        q->data[q->writepos++] = 'x';
        q->writepos %= q->size;
        LED_ERROR_ON;
    } else {
        // queue full?
        if (q->writepos == (q->readpos-1+q->size) % q->size) {
            g_uart1phy->flags |= eSCOMM_QFULL_FL;
            set_cts_pin();
            return;
        }
        // queue almost full? if yes, set CTS pin to prevent further filling
        if (RECV_Q_MAX_CTS_THD < ((q->size + q->writepos - q->readpos) % q->size)) {
            // if queue's free space is less than 32 byte, set flag for sending Xoff.
            set_cts_pin();
        }
        // store new byte in queue
        q->data[q->writepos++] = received_char;
        q->writepos %= q->size;
    }
}

/**
 * Data register empty interrupt 1.
 */
ISR (INTERRUPT_USART_UDRE1)
{
    queue_t    *q = &g_uart1phy->sendQ;

    // If queue is not empty ...
    if(0 < (q->size + q->writepos - q->readpos) % q->size) {
        // send next byte
        REGISTER_UDR1 = q->data[q->readpos++];
        q->readpos %= q->size;
    }
    else { // ... disable interrupt.
        REGISTER_UCSRB1 &= ~(1<<REGBIT_UDRIE);
    }
}

/**
 * Transmit complete interrupt
 */
ISR (INTERRUPT_UART_TRANS1)
{
    g_uart1phy->flags &= ~eSCOMM_TX_FL;
}

// --- Module local functions ---------------------------------------------------------

void serial_convert_and_enqueue_byte (queue_t *q, uint8_t byte)
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

// --- Global functions ---------------------------------------------------------

/**
 * Initializes USART1 and the in-/outgoing queue.
 *
 * @param[in] serial     Handle of bus physical layer.
 *
 * @returns:            nothing
 */
void scomm_initialize_uart1 (scomm_phy_t* serial)
{
    g_uart1phy = serial; //assign the phy-handle for accessing it within the interrupt.
    // initialize queues
    q_initialize(&serial->recvQ, serial->recv_q_data, sizeof(serial->recv_q_data));
    q_initialize(&serial->sendQ, serial->send_q_data, sizeof(serial->send_q_data));
    // initialize UART
    REGISTER_UBRRH1  = UBRRVALH;
    REGISTER_UBRRL1  = UBRRVALL;
    REGISTER_UCSRB1 |= ((1<<REGBIT_RXCIE) | (0<<REGBIT_UDRIE) | (1<<REGBIT_TXCIE) | (1<<REGBIT_RXEN) | (1<<REGBIT_TXEN));
    REGISTER_UCSRC1 |= ((0<<REGBIT_USBS ) | (0<<REGBIT_UCSZ2) | (1<<REGBIT_UCSZ1) | (1<<REGBIT_UCSZ0)); // 8N1;
    clear_cts_pin();
}

/**
 * Send given number of data on the bus.
 * Sending is initiated, if the previous sending process finished.
 *
 * @param[in] serial     Handle of serial interface.
 *
 * @returns TRUE: sending successfully initiated, otherwise FALSE.
 */
bool serial_phy_initiate_sending (scomm_phy_t* serial)
{
    queue_t *q = &serial->sendQ;

    if (0 == q_get_pending(q)) return false;
    // Wait for empty transmit buffer
    while ( !( REGISTER_UCSRA1 & (1<<REGBIT_UDRE)) ) {};

    REGISTER_UDR1    = q_get_byte(q);     // send first byte
    REGISTER_UCSRB1 |= (1<<REGBIT_UDRIE); // enable data register empty interrupt

    return true;
}

/**
 * @param[in] serial     Handle of serial interface.
 */
void serial_phy_check_q_level (scomm_phy_t* serial)
{
    queue_t *q = &serial->recvQ;

    if (RECV_Q_MIN_CTS_THD >= ((q->size + q->writepos - q->readpos) % q->size)) {
        // if queue's free space is bigger than RECV_Q_MIN_CTS_THD byte,
        // signal host computer that we are ready to receive more bytes.
        clear_cts_pin();
    }
}

/**
 * Send a message on the serial line.
 *
 * @param[in]   serial  Handle to serial line.
 * @param[in]   sender  Sender of the message.
 * @param[in]   receiver Receiver of the message.
 * @param[in]  len     Length of message.
 * @param[in]  msg     Message payload.
 *
 * @returns true if a message was sent successfully, otherwise false.
 */
bool serial_send_message (scomm_phy_t *serial, uint16_t sender, uint16_t receiver, uint8_t length, const uint8_t *msg)
{
    serial_convert_and_enqueue_byte(&serial->sendQ, (uint8_t)((sender & 0xFF00)>>8));
    serial_convert_and_enqueue_byte(&serial->sendQ, (uint8_t)( sender & 0x00FF));
    serial_convert_and_enqueue_byte(&serial->sendQ, (uint8_t)((receiver & 0xFF00)>>8));
    serial_convert_and_enqueue_byte(&serial->sendQ, (uint8_t)( receiver & 0x00FF));
    serial_convert_and_enqueue_byte(&serial->sendQ, length);
    for (uint8_t i=0; i<length; i++) {
        serial_convert_and_enqueue_byte(&serial->sendQ, msg[i]);
    }
    q_put_byte(&serial->sendQ, '\n');
    serial_phy_initiate_sending(serial);

    return true;
}
