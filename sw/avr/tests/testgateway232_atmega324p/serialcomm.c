/**
 * @addtogroup BUSGATEWAY
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

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "pcbconfig.h"
#include "serialcomm.h"
#include "ucontroller.h"
#include "queue.h"

// TODO remove after debug
#include "led_debug.h"

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

// --- Local functions ---------------------------------------------------------

/**
 * Initializes USART1 and the in-/outgoing queue.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns:            nothing
 */
void scomm_initialize_uart1 (scomm_phy_t* phy)
{
    g_uart1phy = phy; //assign the phy-handle for accessing it within the interrupt.
    // initialize queues
    q_initialize(&phy->recvQ, phy->recv_q_data, sizeof(phy->recv_q_data));
    q_initialize(&phy->sendQ, phy->send_q_data, sizeof(phy->send_q_data));
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
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns TRUE: sending successfully initiated, otherwise FALSE.
 */
BOOL serial_phy_initiate_sending (scomm_phy_t* phy)
{
    queue_t *q = &phy->sendQ;

    if (0 == q_get_pending(q)) return FALSE;
    // Wait for empty transmit buffer
    while ( !( REGISTER_UCSRA1 & (1<<REGBIT_UDRE)) ) {};

    REGISTER_UDR1    = q_get_byte(q);     // send first byte
    REGISTER_UCSRB1 |= (1<<REGBIT_UDRIE); // enable data register empty interrupt

    return TRUE;
}

/** */
void serial_phy_check_q_level (scomm_phy_t* phy)
{
    queue_t *q = &phy->recvQ;

    if (RECV_Q_MIN_CTS_THD >= ((q->size + q->writepos - q->readpos) % q->size)) {
        // if queue's free space is bigger than RECV_Q_MIN_CTS_THD byte,
        // signal host computer that we are ready to receive more bytes.
        clear_cts_pin();
    }
}
