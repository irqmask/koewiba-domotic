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

#if defined (__AVR_ATmega8__)	\
 || defined (__AVR_ATmega88__)
	#define CLOCKTIMERCONTROLREGISTERA TCCR1A
	#define CLOCK_TCCRB TCCR1B
	#define CLOCKSELECT0 CS10
	#define CLOCKSELECT1 CS11
	#define CLOCKSELECT2 CS12
	#define CLOCK_OCRA_H OCR1AH
	#define CLOCK_OCRA_L OCR1AL
	#define CLOCKOUTPUTCOMPAREB_H OCR1BH
	#define CLOCKOUTPUTCOMPAREB_L OCR1BL
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------
extern void IR_DataRegisterEmpty_Enable(void);
extern void IR_DataRegisterEmpty_Disable(void);
extern void IR_TransmitComplete_Enable(void);
extern void IR_TransmitComplete_Disable(void);
extern void IR_ReceiveComplete_Enable(void);
extern void IR_ReceiveComplete_Disable(void);
extern void IR_OutputCompareMatchA_Enable(void);
extern void IR_OutputCompareMatchA_Disable(void);


#endif /* UCONTROLLER_H_ */
/** @} */
