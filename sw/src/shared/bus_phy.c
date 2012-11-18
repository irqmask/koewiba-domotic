/**
 * @addtogroup BUS
 * @addtogroup BUS_PHY
 * @brief Physical layer of bus protocol.
 *
 * @{
 * @file    bus_phy.c
 * @brief   Physical layer of bus protocol.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bus_intern.h"

// --- Definitions -------------------------------------------------------------

#define UART_PORT               PORTD
#define UART_INPORT             PIND
#define UART_DDR                DDRD
#define UART_DRIVER             0b00000100
#define UART_RECVSTOP           0b00001000

#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * BUS_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

sBusPhy_t* g_UART0Phy = NULL;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

ISR(SIG_USART_RECV)
{
	unsigned char data, fe;
	sBusRec_t	  buffer = g_UART0Phy->sRecvBuf;

	fe = (UCSR0A & (1<<FE0));
	// Daten auslesen, dadurch wird das Interruptflag gelöscht
	data = UDR0;

	if(0!=fe) {
        g_UART0Phy->uflags |= e_uartrxerrflag;
        return;
	}
	if ( buffer.uWritePos == (buffer.uReadPos-1+sizeof(buffer.buf))%(sizeof(buffer.buf)) ) return;
	buffer.buf[buffer.uWritePos] = data;
	buffer.uWritePos = (buffer.uWritePos+1)%sizeof(buffer.buf);
	if (fe) UCSR0A &= ~(1<<FE0); // FrameError-Flag zurücksetzen
	g_UART0Phy->uflags |= e_uartrxflag;
}

ISR(SIG_USART_DATA)
{
    if (g_UART0Phy->uCurrentBytesToSend > 1) {
        g_UART0Phy->puSendPtr++;
        UDR0 = *g_UART0Phy->puSendPtr;
        g_UART0Phy->uCurrentBytesToSend--;
    } else {
        UCSR0B &= ~(1<<UDRIE0);
        g_UART0Phy->uCurrentBytesToSend = 0;
    }
}

// --- Module global functions -------------------------------------------------

void BUS__vPhyInitialize(sBusPhy_t* psPhy, uint8_t uUart)
{
    psPhy->uCurrentBytesToSend = 0;
    psPhy->uUart = uUart;
    
    // configure UART hardware
    if (psPhy->uUart == 0) {
        // initialize UART
    #if defined (__AVR_ATmega8__)
        UBRRH = UBRRVALH;
        UBRRL = UBRRVALL;
        UCSRB |= ((1<<RXCIE) | (1<<RXEN) | (1<<TXEN));
    #elif defined (__AVR_ATmega88__)
        UBRR0H = UBRRVALH;
        UBRR0L = UBRRVALL;
        UCSR0B |= ((1<<RXCIE0) | (1<<UDRIE0) | (1<<TXEN0));
    #endif
        UART_DDR |= (UART_DRIVER | UART_RECVSTOP);
        g_UART0Phy = psPhy;
    }
    
    // sender is initial off, receiver is always on.
    BUS__vPhyActivateSender(psPhy, FALSE);
    BUS__vPhyActivateReceiver(psPhy, TRUE);
}

void BUS__vPhyActivateSender(sBusPhy_t* psPhy, BOOL bActivate)
{
    if (bActivate) {
        UART_PORT |= UART_DRIVER;
    }
    else {
        UART_PORT &= ~UART_DRIVER;
    }
}

void BUS__vPhyActivateReceiver(sBusPhy_t* psPhy, BOOL bActivate)
{
    if (bActivate) {
        UART_PORT &= ~UART_RECVSTOP;
    }
    else {
        UART_PORT |= UART_RECVSTOP;
    }
}

BOOL BUS__bPhySend(sBusPhy_t* psPhy, const uint8_t* puMsg, uint8_t uLen)
{
    if (psPhy->uCurrentBytesToSend != 0) {
        return FALSE;
    }
    psPhy->uCurrentBytesToSend = uLen;
    psPhy->puSendPtr = puMsg;
    if (psPhy->uUart == 0) {
        UDR0 = *psPhy->puSendPtr;   // send first byte
        UCSR0B |= (1<<UDRIE0);      // enable data register empty interrupt
    }
    return TRUE;
}

BOOL BUS__bPhyDataReceived(sBusPhy_t* psPhy)
{
	return (0 != (psPhy->uflags & e_uartrxflag));
}

uint8_t BUS__uPhyRead(sBusPhy_t* psPhy, uint8_t *puInBuf)
{
	uint8_t    n = 0;
	sBusRec_t  buffer = psPhy->sRecvBuf;

	while(buffer.uWritePos != buffer.uReadPos)
	{
		puInBuf[n] = buffer.buf[buffer.uReadPos];
		buffer.uReadPos = (buffer.uReadPos+1) % sizeof(buffer.buf);
		n++;
	}
	psPhy->uflags &= ~(e_uartrxflag);
	return n;
}

BOOL BUS__bPhyReadByte(sBusPhy_t* psPhy, uint8_t *puByte)
{   
	sBusRec_t  buffer = psPhy->sRecvBuf;

	if (buffer.uWritePos != buffer.uReadPos) {
        *puByte = buffer.buf[buffer.uReadPos];
        buffer.uReadPos = (buffer.uReadPos+1) % sizeof(buffer.buf);
        if (buffer.uWritePos == buffer.uReadPos) {
            psPhy->uflags &= ~(e_uartrxflag);
        }
        return TRUE;
	}
	return FALSE;
}

// --- Global functions --------------------------------------------------------

/** @} */
/** @} */
