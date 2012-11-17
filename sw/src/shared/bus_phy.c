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

#define NOW 0

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

// --- Global functions --------------------------------------------------------

void BUS__vPhyInitialize(sBusPhy_t* psPhy, uint8_t uUart)
{
    psPhy->uCurrentBytesToSend = 0;
    psPhy->uUart = uUart;
    
    // configure UART hardware
    if (psPhy->uUart == 0) {
        // initialize UART
    #if defined (__AVR_ATmega8__)
        sjd
        UBRRH = UBRRVALH;
        UBRRL = UBRRVALL;
        UCSRB |= ((1<<RXCIE) | (1<<UDRIE) | (1<<RXEN) | (1<<TXEN));
    #elif defined (__AVR_ATmega88__)
        UBRR0H = UBRRVALH;
        UBRR0L = UBRRVALL;
        UCSR0B |= ((1<<RXCIE0) | (1<<UDRIE0) | (1<<TXEN0));
//        UCSR0B |= 0b10011000;
    #endif
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

/** @} */
/** @} */
