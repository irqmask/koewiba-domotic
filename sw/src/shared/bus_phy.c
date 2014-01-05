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
#include "ucontroller.h"
#include "bus_intern.h"

// --- Switches ----------------------------------------------------------------

#define sw_recvlisten FALSE

// --- Definitions -------------------------------------------------------------

//#define UART_PORT               PORTD
//#define UART_INPORT             PIND
//#define UART_DDR                DDRD
//#define UART_DRIVER             0b00000100
//#define UART_RECVSTOP           0b00001000


#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * BUS_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

/*
inline void IR_DataRegisterEmpty_Enable(void)
{
	REGISTER_UCSRB |=  (1<<REGBIT_UDRIE);
}
inline void IR_DataRegisterEmpty_Disable(void)
{
	REGISTER_UCSRB &= ~(1<<REGBIT_UDRIE);
}
inline void IR_TransmitComplete_Enable(void)
{
	REGISTER_UCSRB |=  (1<<REGBIT_TXCIE);
}
inline void IR_TransmitComplete_Disable(void)
{
	REGISTER_UCSRB &= ~(1<<REGBIT_TXCIE);
}
inline void IR_ReceiveComplete_Enable(void)
{
	REGISTER_UCSRB |=  (1<<REGBIT_RXCIE);
}
inline void IR_ReceiveComplete_Disable(void)
{
	REGISTER_UCSRB &= ~(1<<REGBIT_RXCIE);
}
inline void IR_OutputCompareMatchA_Enable(void)
{
	REGBIT_TIMSK |=  (1<<REGBIT_OCIEA);
}
inline void IR_OutputCompareMatchA_Disable(void)
{
	REGBIT_TIMSK &= ~(1<<REGBIT_OCIEA);
}
#if defined (__AVR_ATmega88__)
inline void IR_PinChange2_Enable(void)
{
	PCICR  |=  (1<<PCIE2);
}
inline void IR_PinChange2_Disable(void)
{
	PCICR  &= ~(1<<PCIE2);
}
inline void IR_PinChange1_Enable(void)
{
	PCICR  |=  (1<<PCIE1);
}
inline void IR_PinChange1_Disable(void)
{
	PCICR  &= ~(1<<PCIE1);
}
inline void IR_PinChange0_Enable(void)
{
	PCICR  |=  (1<<PCIE0);
}
inline void IR_PinChange0_Disable(void)
{
	PCICR  &= ~(1<<PCIE0);
}
#endif
*/
// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

sBusPhy_t* g_UART0Phy = NULL;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Received byte interrupt.
 */
ISR(INTERRUPT_USART_RXC)
{
    uint8_t     data, fe;
    sBusRec_t   *buffer = &g_UART0Phy->sRecvBuf;

    fe = (REGISTER_UCSRA & (1<<REGBIT_FE));
    // read data to clear interrupt flag
    data = REGISTER_UDR;

    // framing error ?
    if(0 != fe) {
        g_UART0Phy->uFlags |= e_uartrxerrflag;
        return;
    }
    if (buffer->uWritePos == (buffer->uReadPos-1+sizeof(buffer->auBuf)) % (sizeof(buffer->auBuf))) return;
    buffer->auBuf[buffer->uWritePos] = data;
    buffer->uWritePos = (buffer->uWritePos+1) % sizeof(buffer->auBuf);
    if (fe) REGISTER_UCSRA &= ~(1<<REGBIT_FE); // Reset FrameError-Flag
    g_UART0Phy->uFlags |= e_uartrxflag;
}

/**
 * Data register empty interrupt.
 */
ISR(INTERRUPT_USART_UDRE)
{
	if (g_UART0Phy->uCurrentBytesToSend > 1) {
    	REGISTER_UDR = *++g_UART0Phy->puSendPtr;
        g_UART0Phy->uCurrentBytesToSend--;
    } else {
        REGISTER_UCSRB &= ~(1<<REGBIT_UDRIE);
        g_UART0Phy->uCurrentBytesToSend = 0;
    }
}

/**
 * Transmit complete interrupt
 */
ISR(INTERRUPT_UART_TRANS)
{
    BUS__vPhyActivateSender(g_UART0Phy, FALSE);
    BUS__vPhyActivateReceiver(g_UART0Phy, TRUE);
    g_UART0Phy->uFlags &= ~e_uarttxflag;
}

// --- Module global functions -------------------------------------------------

/**
 * Initialize data and hardware of bus's physical layer.
 *
 * @param[in] psPhy		Handle of bus physical layer.
 * @param[in] uUart		Number of the UART. 0=first.
 */
void BUS__vPhyInitialize(sBusPhy_t* psPhy, uint8_t uUart)
{
    psPhy->uCurrentBytesToSend = 0;
    psPhy->uUart = uUart;
    psPhy->uFlags = 0;

    // configure UART hardware
    if (psPhy->uUart == 0) {
        g_UART0Phy = psPhy;
        // initialize UART
        REGISTER_UBRRH = UBRRVALH;
        REGISTER_UBRRL = UBRRVALL;
        REGISTER_UCSRB |= ((1<<REGBIT_RXCIE) | (0<<REGBIT_UDRIE) | (1<<REGBIT_TXCIE) | (1<<REGBIT_RXEN) | (1<<REGBIT_TXEN));
        BUS_DDR_ENASND |= (1<<BUS_ENASND);
#ifndef TXRXEN_COMBINED
        BUS_DDR_DISRCV |= (1<<BUS_DISRCV);
#endif
    }

    // sender is initial off, receiver is always on.
    BUS__vPhyActivateSender(psPhy, FALSE);
    BUS__vPhyActivateReceiver(psPhy, TRUE);
}

/**
 * Activate or deactivate bus's sender hardware (driver).
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate
 * @returns TRUE: activate sender, FALSE: deactivate sender.
 */
void BUS__vPhyActivateSender(sBusPhy_t* psPhy, BOOL bActivate)
{
    if (bActivate) {
        if(!sw_recvlisten) BUS__vPhyActivateReceiver(psPhy, FALSE);
        BUS_PORT_ENASND |= (1<<BUS_ENASND);
    }
    else {
        BUS_PORT_ENASND &= ~(1<<BUS_ENASND);
        BUS__vPhyActivateReceiver(psPhy, TRUE);
    }
}

/**
 * Activate or deactivate bus's receiver hardware.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 * @param[in] bActivate
 * @returns TRUE: activate receiver, FALSE: deactivate receiver.
 */
void BUS__vPhyActivateReceiver(sBusPhy_t* psPhy, BOOL bActivate)
{
#ifndef TXRXEN_COMBINED
    if (bActivate) {
        BUS_PORT_DISRCV &= ~(1<<BUS_DISRCV);
    }
    else {
        BUS_PORT_DISRCV |= (1<<BUS_DISRCV);
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
 * @returns TRUE: sending successfully initiated, otherwise FALSE.
 */
BOOL BUS__bPhySend(sBusPhy_t* psPhy, const uint8_t* puMsg, uint8_t uLen)
{
    if (psPhy->uCurrentBytesToSend != 0) {
        return FALSE;
    }
    psPhy->uCurrentBytesToSend = uLen;
    psPhy->uFlags |= e_uarttxflag;
    psPhy->puSendPtr = puMsg;
    if (psPhy->uUart == 0) {
        BUS__vPhyActivateSender(psPhy, TRUE);
        REGISTER_UDR = *psPhy->puSendPtr;   // send first byte
        REGISTER_UCSRB |=  (1<<REGBIT_UDRIE); // enable data register empty interrupt
    }
    return TRUE;
}

/**
 * Checks if data is currently beeing sent.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns TRUE:       sending in progress.
 */
BOOL BUS__bPhySending(sBusPhy_t* psPhy)
{
    return ((psPhy->uFlags & e_uarttxflag) || (0 < psPhy->uCurrentBytesToSend));
}

/**
 * Checks if data has been received.
 *
 * @param[in] psPhy     Handle of bus physical layer.
 *
 * @returns TRUE: at least one byte is waiting in receive buffer.
 */
BOOL BUS__bPhyDataReceived(sBusPhy_t* psPhy)
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
uint8_t BUS__uPhyRead(sBusPhy_t* psPhy, uint8_t *puInBuf)
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
 * @returns TRUE if a byte has been received, otherwise false.
 */
BOOL BUS__bPhyReadByte(sBusPhy_t* psPhy, uint8_t *puByte)
{
    sBusRec_t  *buffer = &psPhy->sRecvBuf;

    if (buffer->uWritePos == buffer->uReadPos) {
		psPhy->uFlags &= ~(e_uartrxflag);
		return FALSE;
    }
	*puByte = buffer->auBuf[buffer->uReadPos];
	buffer->uReadPos = (buffer->uReadPos+1) % sizeof(buffer->auBuf);
	// check if there are still bytes pending in queue
	if (buffer->uWritePos == buffer->uReadPos) {
		psPhy->uFlags &= ~(e_uartrxflag);
	}
	return TRUE;
}

/**
 * Flush all data from physical layer's input buffer.
 *
 * @param[in] psPhy
 * Handle of bus physical layer.
 */
void BUS__uPhyFlush(sBusPhy_t* psPhy)
{
    uint8_t    n = 0;
    sBusRec_t  *buffer = &psPhy->sRecvBuf;

    for(n=0; n<sizeof(buffer->auBuf); ++n){
    	buffer->auBuf[n] = 0;
    }
    buffer->uReadPos  = 0;
    buffer->uWritePos = 0;
    psPhy->uFlags &= ~(e_uartrxflag);
}

// --- Global functions --------------------------------------------------------


/*
void BUS__vDebugSend(uint8_t *data, uint8_t len)
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
