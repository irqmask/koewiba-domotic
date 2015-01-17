/*
 * serialcomm.c
 *
 *  Created on: 28.12.2014
 *      Author: robert mueller
 *
 *   IMPORTANT: Dependent on the controller used for serialcomm the receive-queue is not as huge
 *   as the computers serial buffer is. Hence it is important to reduce the block-size of bytes
 *   that will be send by the computers serial interface.
 *   Please choose 64 Byte and set flow control to 'hardware', otherwise it will not work properly.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "pcbconfig.h"
#include "serialcomm.h"
#include "ucontroller.h"
#include "queue.h"



// --- Definitions -------------------------------------------------------------
#define SERIAL_BAUDRATE 57600

#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * SERIAL_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------
static sSerPhy_t *g_UART1Phy = NULL;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Interrupts --------------------------------------------------------------
/**
 * Received byte interrupt 1.
 */
ISR(INTERRUPT_USART_RXC1)
{
    uint8_t    data, fe;
    queue_t    *q = &g_UART1Phy->sRecvQ;

    fe = (REGISTER_UCSRA1 & (1<<REGBIT_FE));
    // read data to clear interrupt flag
    data = REGISTER_UDR1;

    // framing error ?
    if (0 != fe) {
        g_UART1Phy->uFlags |= e_scommrxerrflag;
        return;
    }
    if (q->writepos == (q->readpos-1+sizeof(q->data)) % (sizeof(q->data))) {
        return;
    }

    if ((sizeof(q->data)/2) > (sizeof(q->data) - 1 + q->readpos - q->writepos) % sizeof(q->data))
    {   // if queue's free space is less than 32 byte, set flag for sending Xoff.
        set_cts_pin();
        if (q->writepos == (q->readpos-1+sizeof(q->data)) % (sizeof(q->data))) return;
    }

    q->data[q->writepos] = data;
    q->writepos = (q->writepos+1) % sizeof(q->data);
    if (fe) REGISTER_UCSRA1 &= ~(1<<REGBIT_FE); // Reset FrameError-Flag
    g_UART1Phy->uFlags |= e_scommrxflag;
}

/**
 * Data register empty interrupt 1.
 */
ISR(INTERRUPT_USART_UDRE1)
{
    queue_t    *q = &g_UART1Phy->sSendQ;

    // If queue is not empty ...
    if(0 < (sizeof(q->data) + q->writepos - q->readpos) % sizeof(q->data))
    {   // send next byte
        REGISTER_UDR1 = q->data[q->readpos++];
        q->readpos %= sizeof(q->data);

    }
    else // ... disable interrupt.
    {
        REGISTER_UCSRB1 &= ~(1<<REGBIT_UDRIE);
    }


}

/**
 * Transmit complete interrupt
 */
ISR(INTERRUPT_UART_TRANS1)
{
    g_UART1Phy->uFlags &= ~e_scommtxflag;
}



// --- Local functions ---------------------------------------------------------

/**
 * Initializes USART1 and the in-/outgoing queue.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns:            nothing
 */
void scomm_initialize_uart1(sSerPhy_t *psPhy)
{
    g_UART1Phy = psPhy; //assign the phy-handle for accessing it within the interrupt.
    // initialize queues
    q_initialize(&psPhy->sRecvQ);
    q_initialize(&psPhy->sSendQ);
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
BOOL serial_phy_initiate_sending(sSerPhy_t *psPhy)
{
    queue_t *q = &psPhy->sSendQ;
    if(0 == q_get_pending(q)) return FALSE;
    /* Wait for empty transmit buffer */
    while ( !( REGISTER_UCSRA1 & (1<<REGBIT_UDRE)) ) {};

    REGISTER_UDR1    = q_get_byte(q);     // send first byte
    REGISTER_UCSRB1 |= (1<<REGBIT_UDRIE); // enable data register empty interrupt

    return TRUE;
}


/**
 * Checks if data has been received.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns TRUE:   at least one byte is waiting in receive buffer.
 */
BOOL serial_phy_data_received(sSerPhy_t* psPhy)
{
    return (0 != (psPhy->uFlags & e_scommrxflag));
}


/**
 * Checks if a message has been received.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns TRUE:       the entire message is received.
 *          FALSE:      we have to wait for further bytes.
 */
BOOL serial_phy_msg_received(sSerPhy_t* psPhy)
{
    // Nothing (new) received, then return.
    if (TRUE != serial_phy_data_received(psPhy)) return FALSE;

    // If CTS is true, reset the Received-Flag, otherwise it is necessary to poll this function periodical
    // to obtain information about the queue regaining sufficient space again.
    if     ( !cts_state() ) psPhy->uFlags &= ~e_scommrxflag; // erstmal RX-Flag zurücksetzen.
    else if((3*sizeof(psPhy->sRecvQ.data)/4) < q_get_free(&psPhy->sRecvQ) ) {
        // set 'ClearToSend' true
        clear_cts_pin();
    }

    // There are data in the queue, but the length byte (byte 3) is not received yet -> wait for further bytes!
    if (3 > q_get_pending(&psPhy->sRecvQ))                                      return FALSE;
    // The message length is common now. If the whole message is already received return true.
    else if (q_read_byte(&psPhy->sRecvQ, 2) <= q_get_pending(&psPhy->sRecvQ)-3) return TRUE;
    // otherwise wait for message entirely received.
    return FALSE;
}

