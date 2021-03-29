/**
 * @addtogroup BUS
 * @addtogroup BUS_PHY
 * @brief Physical layer of bus protocol.
 * This module contains the hardware dependent functions which are used by
 * bus_transport.c and bus_scheduler.c.
 *
 * @{
 * @file    bus_phy.c
 * @brief   Physical layer of bus protocol.
 * @author  Christian Verhalen
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
#include "ucontroller.h"
#include "bus_intern.h"
#include "pcbconfig.h"
#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * BUS_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

sBusPhy_t* g_UART0Phy = NULL;
#if defined (__AVR_ATtiny1634__) && defined (BUS_HUBMODE)
sBusPhy_t* g_UART1Phy = NULL;
#endif

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Received byte interrupt 0.
 */
ISR(INTERRUPT_USART_RXC0)
{
    uint8_t     data, fe;
    sBusRec_t   *buffer = &g_UART0Phy->sRecvBuf;

    fe = (REGISTER_UCSRA0 & (1<<REGBIT_FE));
    // read data to clear interrupt flag
    data = REGISTER_UDR0;

    // framing error ?
    if(0 != fe) {
        g_UART0Phy->uFlags |= e_uartrxerrflag;
        return;
    }
    if (buffer->uWritePos == (buffer->uReadPos-1+sizeof(buffer->auBuf)) % (sizeof(buffer->auBuf))) return;
    buffer->auBuf[buffer->uWritePos] = data;
    buffer->uWritePos = (buffer->uWritePos+1) % sizeof(buffer->auBuf);
    if (fe) REGISTER_UCSRA0 &= ~(1<<REGBIT_FE); // Reset FrameError-Flag
    g_UART0Phy->uFlags |= e_uartrxflag;
}

/**
 * Data register empty interrupt 0.
 */
ISR(INTERRUPT_USART_UDRE0)
{
	if (g_UART0Phy->uCurrentBytesToSend > 1) {
    	REGISTER_UDR0 = *++g_UART0Phy->puSendPtr;
        g_UART0Phy->uCurrentBytesToSend--;
    } else {
        REGISTER_UCSRB0 &= ~(1<<REGBIT_UDRIE);
        g_UART0Phy->uCurrentBytesToSend = 0;
    }
}

/**
 * Transmit complete interrupt
 */
ISR(INTERRUPT_UART_TRANS0)
{
    bus_phy_activate_sender(g_UART0Phy, false);
#ifdef TXRXEN0_SEPERATE
    bus_phy_activate_receiver(g_UART0Phy, true);
#endif
    g_UART0Phy->uFlags &= ~e_uarttxflag;

}

#if defined (__AVR_ATtiny1634__) && defined (BUS_HUBMODE)
/**
 * Received byte interrupt 1.
 */
ISR(INTERRUPT_USART_RXC1)
{
    uint8_t     data, fe;
    sBusRec_t   *buffer = &g_UART1Phy->sRecvBuf;

    fe = (REGISTER_UCSRA1 & (1<<REGBIT_FE));
    // read data to clear interrupt flag
    data = REGISTER_UDR1;

    // framing error ?
    if (0 != fe) {
        g_UART1Phy->uFlags |= e_uartrxerrflag;
        return;
    }
    if (buffer->uWritePos == (buffer->uReadPos-1+sizeof(buffer->auBuf)) % (sizeof(buffer->auBuf))) return;
    buffer->auBuf[buffer->uWritePos] = data;
    buffer->uWritePos = (buffer->uWritePos+1) % sizeof(buffer->auBuf);
    if (fe) REGISTER_UCSRA1 &= ~(1<<REGBIT_FE); // Reset FrameError-Flag
    g_UART1Phy->uFlags |= e_uartrxflag;
}

/**
 * Data register empty interrupt 0.
 */
ISR(INTERRUPT_USART_UDRE1)
{
	if (g_UART1Phy->uCurrentBytesToSend > 1) {
    	REGISTER_UDR = *++g_UART1Phy->puSendPtr;
        g_UART1Phy->uCurrentBytesToSend--;
    } else {
        REGISTER_UCSRB &= ~(1<<REGBIT_UDRIE);
        g_UART1Phy->uCurrentBytesToSend = 0;
    }
}

/**
 * Transmit complete interrupt
 */
ISR(INTERRUPT_UART_TRANS1)
{
    bus_phy_activate_sender(g_UART1Phy, false);
    bus_phy_activate_receiver(g_UART1Phy, true);
    g_UART1Phy->uFlags &= ~e_uarttxflag;
}
#endif

// --- Module global functions -------------------------------------------------

/**
 * Initialize data and hardware of bus's physical layer.
 *
 * @param[in] psPhy		Handle of bus physical layer.
 * @param[in] uUart		Number of the UART. 0=first.
 */
void bus_phy_initialize(sBusPhy_t* psPhy, uint8_t uUart)
{
    psPhy->uCurrentBytesToSend = 0;
    psPhy->uUart = uUart;
    psPhy->uFlags = 0;

    // configure UART hardware
    if (psPhy->uUart == 0) {
        g_UART0Phy = psPhy;
        // initialize UART
        REGISTER_UBRRH0  = UBRRVALH;
        REGISTER_UBRRL0  = UBRRVALL;
        REGISTER_UCSRB0 |= ((1<<REGBIT_RXCIE) | (0<<REGBIT_UDRIE) | (1<<REGBIT_TXCIE) | (1<<REGBIT_RXEN) | (1<<REGBIT_TXEN));
        BUS_DDR_ENASND0 |= (1<<BUS_ENASND0);
    #ifdef TXRXEN0_SEPERATE
            BUS_DDR_DISRCV0 |= (1<<BUS_DISRCV0);
    #endif
     }
#if defined (__AVR_ATtiny1634__) && defined (BUS_HUBMODE)
    else if (psPhy->uUart == 1) {
        g_UART0Phy = psPhy;
        // initialize UART
        REGISTER_UBRRH1  = UBRRVALH;
        REGISTER_UBRRL1  = UBRRVALL;
        REGISTER_UCSRB1 |= ((1<<REGBIT_RXCIE) | (0<<REGBIT_UDRIE) | (1<<REGBIT_TXCIE) | (1<<REGBIT_RXEN) | (1<<REGBIT_TXEN));
        BUS_DDR_ENASND1 |= (1<<BUS_ENASND1);
    #ifdef TXRXEN1_SEPERATE
            BUS_DDR_DISRCV1 |= (1<<BUS_DISRCV1);
    #endif
    }
#endif

    // sender is initial off, receiver is on.
    bus_phy_activate_sender(psPhy, false);
#ifdef TXRXEN0_SEPERATE
    bus_phy_activate_receiver(psPhy, true);
#endif
}

/**
 * Activate or deactivate bus's sender hardware (driver).
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate
 * @returns true: activate sender, false: deactivate sender.
 */
void bus_phy_activate_sender(sBusPhy_t* psPhy, bool bActivate)
{
    bus_phy_activate_receiver(psPhy, !bActivate);
    if (0==psPhy->uUart) {
        if (bActivate)  BUS_PORT_ENASND0 |=  (1<<BUS_ENASND0);
        else            BUS_PORT_ENASND0 &= ~(1<<BUS_ENASND0);
    }
#ifdef BUS_PORT_ENASND1
    else if(1==psPhy->uUart) {
        if (bActivate)  BUS_PORT_ENASND1 |=  (1<<BUS_ENASND1);
        else            BUS_PORT_ENASND1 &= ~(1<<BUS_ENASND1);
    }
#endif
}

/**
 * Activate or de-activate bus's receiver hardware.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate
 * @returns true: activate receiver, false: deactivate receiver.
 */
void bus_phy_activate_receiver(sBusPhy_t* psPhy, bool bActivate)
{
#ifdef TXRXEN0_SEPERATE
    if (0 == psPhy->uUart) {
        if (bActivate) BUS_PORT_DISRCV0 &= ~(1<<BUS_DISRCV0);
        else           BUS_PORT_DISRCV0 |=  (1<<BUS_DISRCV0);
    }
#endif
#ifdef TXRXEN1_SEPERATE
    if (1 == psPhy->uUart) {
        if (bActivate) BUS_PORT_DISRCV1 &= ~(1<<BUS_DISRCV1);
        else           BUS_PORT_DISRCV1 |=  (1<<BUS_DISRCV1);
    }
#endif
}

/**
 * Send given number of data on the bus.
 * Sending is initiated, if the previous sending process finished.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] puMsg     Pointer to message to be sent.
 * @param[in] uLen      Length in bytes of mesage to be sent.
 *
 * @returns true: sending successfully initiated, otherwise false.
 */
bool bus_phy_send(sBusPhy_t* psPhy, const uint8_t* puMsg, uint8_t uLen)
{
    if (psPhy->uCurrentBytesToSend != 0) {
        return false;
    }
    psPhy->uCurrentBytesToSend = uLen;
    psPhy->uFlags |= e_uarttxflag;
    psPhy->puSendPtr = puMsg;
    if (psPhy->uUart == 0) {
        bus_phy_activate_sender(psPhy, true);
        REGISTER_UDR0 = *psPhy->puSendPtr;   // send first byte
        REGISTER_UCSRB0 |=  (1<<REGBIT_UDRIE); // enable data register empty interrupt
    }
    return true;
}

/**
 * Checks if data is currently beeing sent.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns true:       sending in progress.
 */
bool bus_phy_sending(sBusPhy_t* psPhy)
{
    return ((psPhy->uFlags & e_uarttxflag) || (0 < psPhy->uCurrentBytesToSend));
}

/**
 * Checks if data has been received.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns true: at least one byte is waiting in receive buffer.
 */
bool bus_phy_data_received(sBusPhy_t* psPhy)
{
    return (0 != (psPhy->uFlags & e_uartrxflag));
}

/**
 * Read all data from physical layer's input buffer.
 *
 * @param[in] psPhy
 * Handle of bus physical layer.
 * @param[out] puInBuf
 * Pointer to buffer where received data is copied to. It has at least to be as
 * big as the internal input buffer in physical layer.
 *
 * @returns Number of bytes read.
 */
uint8_t bus_phy_read(sBusPhy_t* psPhy, uint8_t *puInBuf)
{
    uint8_t    n = 0;
    sBusRec_t  *buffer = &psPhy->sRecvBuf;

    while(buffer->uWritePos != buffer->uReadPos) {
        puInBuf[n] = buffer->auBuf[buffer->uReadPos];
        buffer->uReadPos = (buffer->uReadPos+1) % sizeof(buffer->auBuf);
        n++;
    }
    psPhy->uFlags &= ~(e_uartrxflag);
    return n;
}

/**
 * Read a byte from the received bytes queue.
 *
 * @param[in] psPhy     Handle to bus's phsical layer
 * @param[out] puByte   Received byte. *puByte remains unchange if no byte has been received.
 *
 * @returns true if a byte has been received, otherwise false.
 */
bool bus_phy_read_byte(sBusPhy_t* psPhy, uint8_t *puByte)
{
    sBusRec_t  *buffer = &psPhy->sRecvBuf;

    if (buffer->uWritePos == buffer->uReadPos) {
		psPhy->uFlags &= ~(e_uartrxflag);
		return false;
    }
	*puByte = buffer->auBuf[buffer->uReadPos];
	buffer->uReadPos = (buffer->uReadPos+1) % sizeof(buffer->auBuf);
	// check if there are still bytes pending in queue
	if (buffer->uWritePos == buffer->uReadPos) {
		psPhy->uFlags &= ~(e_uartrxflag);
	}
	return true;
}

/**
 * Flush all data from physical layer's input buffer.
 *
 * @param[in] psPhy
 * Handle of bus physical layer.
 */
void bus_phy_flush(sBusPhy_t* psPhy)
{
    uint8_t    n = 0;
    sBusRec_t  *buffer = &psPhy->sRecvBuf;

    for (n=0; n<sizeof(buffer->auBuf); ++n){
    	buffer->auBuf[n] = 0;
    }
    buffer->uReadPos  = 0;
    buffer->uWritePos = 0;
    psPhy->uFlags &= ~(e_uartrxflag);
}

// --- Global functions --------------------------------------------------------

/*
void bus_debug_send(uint8_t *data, uint8_t len)
{
	uint8_t ii;
	REGISTER_UCSRB &= ~(1<<REGBIT_TXCIE);
	UART_PORT |= UART_RECVSTOP;
	UART_PORT |= UART_DRIVER;
	for(ii=0;ii<len;ii++) {
		REGISTER_UDR = data[ii];   // send byte
		while ( !( UCSRA & (1<<UDRE)) ) {};
	}
	while ( !( UCSRA & (1<<TXC)) ) {};
	UCSRA |= (1<<TXC);
	UART_PORT &= ~UART_DRIVER;
	UART_PORT &= ~UART_RECVSTOP;
	REGISTER_UCSRB |= (1<<REGBIT_TXCIE);
}
*/

/** @} */
/** @} */
