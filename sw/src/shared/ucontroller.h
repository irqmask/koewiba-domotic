/**
 * @addtogroup ARCH
 * @brief Define project specific types definitions.
 *
 * @{
 * @file    ucontroller.h
 * @brief   Define project specific types definitions..
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------

#ifndef UCONTROLLER_H_
#define UCONTROLLER_H_

// --- Include section ---------------------------------------------------------


// --- Definitions -------------------------------------------------------------

#if defined (__AVR_ATmega8__)
    #define REGISTER_UCSRA UCSRA
    #define REGISTER_UCSRB UCSRB
    #define REGISTER_UDR   UDR
    #define REGISTER_UBRRH UBRRH
    #define REGISTER_UBRRL UBRRL
    #define REGBIT_RXCIE   RXCIE
    #define REGBIT_UDRIE   UDRIE
    #define REGBIT_TXCIE   TXCIE
    #define REGBIT_RXEN    RXEN
    #define REGBIT_TXEN    TXEN
    #define REGBIT_FE      FE
    #define INTERRUPT_USART_RXC  USART_RXC_vect
    #define INTERRUPT_USART_UDRE USART_UDRE_vect
    #define INTERRUPT_UART_TRANS USART_TXC_vect
#elif defined (__AVR_ATmega88__)
    #define REGISTER_UCSRA   UCSR0A
    #define REGISTER_UCSRB   UCSR0B
    #define REGISTER_UDR     UDR0
    #define REGISTER_UBRRH   UBRR0H
    #define REGISTER_UBRRL   UBRR0L
    #define REGBIT_RXCIE   RXCIE0
    #define REGBIT_UDRIE   UDRIE0
    #define REGBIT_TXCIE   TXCIE0
    #define REGBIT_RXEN    RXEN0
    #define REGBIT_TXEN    TXEN0
    #define REGBIT_FE      FE0
    #define INTERRUPT_USART_RXC  USART_RX_vect
    #define INTERRUPT_USART_UDRE USART_UDRE_vect
    #define INTERRUPT_UART_TRANS USART_TX_vect
#endif


// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------


#endif /* UCONTROLLER_H_ */
/** @} */
