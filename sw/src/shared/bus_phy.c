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

#if defined (__AVR_ATmega8__)
	#define REGBIT_UCSRA UCSRA
	#define REGBIT_UCSRB UCSRB
	#define REGBIT_UDR	 UDR
	#define REGBIT_UBRRH UBRRH
	#define REGBIT_UBRRL UBRRL
	#define REGBIT_RXCIE RXCIE
	#define REGBIT_UDRIE UDRIE
	#define REGBIT_TXEN  TXEN
	#define REGBIT_FE    FE
	#define INTERRUPT_USART_RXC  USART_RXC_vect
	#define INTERRUPT_USART_UDRE USART_UDRE_vect
	#define INTERRUPT_UART_TRANS USART_TXC_vect
#elif defined (__AVR_ATmega88__)
	#define REGBIT_UCSRA UCSR0A
	#define REGBIT_UCSRB UCSR0B
	#define REGBIT_UDR	 UDR0
	#define REGBIT_UBRRH UBRR0H
	#define REGBIT_UBRRL UBRR0L
	#define REGBIT_RXCIE RXCIE0
	#define REGBIT_UDRIE UDRIE0
	#define REGBIT_TXEN  TXEN0
	#define REGBIT_FE    FE0
	#define INTERRUPT_USART_RXC  SIG_UART_RECV
	#define INTERRUPT_USART_UDRE SIG_UART_DATA
	#define INTERRUPT_UART_TRANS SIG_UART_TRANS
#endif

#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * BUS_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

sBusPhy_t* g_UART0Phy = NULL;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

ISR(INTERRUPT_USART_RXC)
{
	unsigned char data, fe;
	sBusRec_t	  buffer = g_UART0Phy->sRecvBuf;

	fe = (REGBIT_UCSRA & (1<<REGBIT_FE));
	// Daten auslesen, dadurch wird das Interruptflag gel�scht
	data = REGBIT_UDR;

	if(0!=fe) {
        g_UART0Phy->uflags |= e_uartrxerrflag;
        return;
	}
	if ( buffer.uWritePos == (buffer.uReadPos-1+sizeof(buffer.auBuf)) % (sizeof(buffer.auBuf)) ) return;
	buffer.auBuf[buffer.uWritePos] = data;
	buffer.uWritePos = (buffer.uWritePos+1) % sizeof(buffer.auBuf);
	if (fe) REGBIT_UCSRA &= ~(1<<REGBIT_FE); // Reset FrameError-Flag
	g_UART0Phy->uflags |= e_uartrxflag;
}

ISR(INTERRUPT_USART_UDRE)
{
    if (g_UART0Phy->uCurrentBytesToSend > 1) {
        g_UART0Phy->puSendPtr++;
        REGBIT_UDR = *g_UART0Phy->puSendPtr;
        g_UART0Phy->uCurrentBytesToSend--;
    } else {
        REGBIT_UCSRB &= ~(1<<REGBIT_UDRIE);
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
        REGBIT_UBRRH = UBRRVALH;
        REGBIT_UBRRL = UBRRVALL;
        REGBIT_UCSRB |= ((1<<REGBIT_RXCIE) | (1<<REGBIT_UDRIE) | (1<<REGBIT_TXEN));
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
        REGBIT_UDR = *psPhy->puSendPtr;   // send first byte
        REGBIT_UCSRB |= (1<<REGBIT_UDRIE);      // enable data register empty interrupt
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
		puInBuf[n] = buffer.auBuf[buffer.uReadPos];
		buffer.uReadPos = (buffer.uReadPos+1) % sizeof(buffer.auBuf);
		n++;
	}
	psPhy->uflags &= ~(e_uartrxflag);
	return n;
}

BOOL BUS__bPhyReadByte(sBusPhy_t* psPhy, uint8_t *puByte)
{   
	sBusRec_t  buffer = psPhy->sRecvBuf;

	if (buffer.uWritePos != buffer.uReadPos) {
        *puByte = buffer.auBuf[buffer.uReadPos];
        buffer.uReadPos = (buffer.uReadPos+1) % sizeof(buffer.auBuf);
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
