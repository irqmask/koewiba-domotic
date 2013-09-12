/**
 * @addtogroup ARCH
 * @brief Processor and architecture specific symbols and type definitions.
 *
 * @{
 * @file    ucontroller.h
 * @brief   Processor and architecture specific symbols and type definitions.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------

#ifndef _UCONTROLLER_H_
#define _UCONTROLLER_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#if defined (__AVR_ATmega8__)
 #define REGISTER_UCSRA         UCSRA
 #define REGISTER_UCSRB         UCSRB
 #define REGISTER_UCSRC         UCSRC
 #define REGISTER_UDR           UDR
 #define REGISTER_UBRRH         UBRRH
 #define REGISTER_UBRRL         UBRRL
 #define REGBIT_RXCIE           RXCIE
 #define REGBIT_UDRIE           UDRIE
 #define REGBIT_TXCIE           TXCIE
 #define REGBIT_RXEN            RXEN
 #define REGBIT_TXEN            TXEN
 #define REGBIT_FE              FE
 #define INTERRUPT_USART_RXC    USART_RXC_vect
 #define INTERRUPT_USART_UDRE   USART_UDRE_vect
 #define INTERRUPT_UART_TRANS   USART_TXC_vect
 #define INTERRUPT_SPI          SPI_STC_vect

 // SPI
 #define REG_SPCR0              SPCR
 #define REG_SPDR0              SPDR
 #define REG_SPSR0              SPSR
 // SPCR
 #define REGBIT_SPE0            SPE
 #define REGBIT_MSTR0           MSTR
 // SPSR
 #define REGBIT_SPIF0           SPIF
 #define REGBIT_SPI2X0          SPI2X

#elif defined (__AVR_ATmega88__)
// UART
 #define REGISTER_UCSRA         UCSR0A
 #define REGISTER_UCSRB         UCSR0B
 #define REGISTER_UCSRC         UCSR0C
 #define REGISTER_UDR           UDR0
 #define REGISTER_UBRRH         UBRR0H
 #define REGISTER_UBRRL         UBRR0L
 #define REGBIT_RXCIE           RXCIE0
 #define REGBIT_UDRIE           UDRIE0
 #define REGBIT_TXCIE           TXCIE0
 #define REGBIT_RXEN            RXEN0
 #define REGBIT_TXEN            TXEN0
 #define REGBIT_FE              FE0
 #define INTERRUPT_USART_RXC    USART_RX_vect
 #define INTERRUPT_USART_UDRE   USART_UDRE_vect
 #define INTERRUPT_UART_TRANS   USART_TX_vect
 #define INTERRUPT_PINCHANGE2   PCINT2_vect
 #define INTERRUPT_SPI          SPI_STC_vect

// SPI
 #define REG_SPCR0              SPCR
 #define REG_SPDR0              SPDR
 #define REG_SPSR0              SPSR
 // SPCR
 #define REGBIT_SPE0            SPE
 #define REGBIT_MSTR0           MSTR
 // SPSR
 #define REGBIT_SPIF0           SPIF
 #define REGBIT_SPI2X0          SPI2X
 
#elif defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__)
 // definition of prozessor depending global settings
 // TODO CV: this part can be ported maybe to a makefile include
 #define STARTADDR_APPVECTORS   0x0000
 #define STARTADDR_APPLICATION  0x003E
 #define STARTADDR_BOOTLOADER   0x1700
 #define MAXSIZE_APPLICATION    (STARTADDR_BOOTLOADER - STARTADDR_APPVECTORS)
 #define MAXSIZE_BOOTLOADER     0x0400

 // definition of processor specific registers
 #define REGISTER_UCSRA         UCSR0A
 #define REGISTER_UCSRB         UCSR0B
 #define REGISTER_UCSRC         UCSR0C
 #define REGISTER_UDR           UDR0
 #define REGISTER_UBRRH         UBRR0H
 #define REGISTER_UBRRL         UBRR0L
 #define REGBIT_RXCIE           RXCIE0
 #define REGBIT_UDRIE           UDRIE0
 #define REGBIT_TXCIE           TXCIE0
 #define REGBIT_RXEN            RXEN0
 #define REGBIT_TXEN            TXEN0
 #define REGBIT_FE              FE0
 #define INTERRUPT_USART_RXC    USART0_RX_vect
 #define INTERRUPT_USART_UDRE   USART0_UDRE_vect
 #define INTERRUPT_UART_TRANS   USART0_TX_vect
 #define INTERRUPT_PINCHANGE2   PCINT2_vect
 #define INTERRUPT_SPI          SPI_STC_vect

 // SPI
 #define REG_SPCR0              SPCR0
 #define REG_SPDR0              SPDR0
 #define REG_SPSR0              SPSR0
 // SPCR
 #define REGBIT_SPE0            SPE0
 #define REGBIT_MSTR0           MSTR0
 // SPSR
 #define REGBIT_SPIF0           SPIF0
 #define REGBIT_SPI2X0          SPI2X0

#endif

#if defined (__AVR_ATmega8__) || defined (__AVR_ATmega88__)
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
extern void IR_PinChange2_Enable(void);
extern void IR_PinChange2_Disable(void);
extern void IR_PinChange1_Enable(void);
extern void IR_PinChange1_Disable(void);
extern void IR_PinChange0_Enable(void);
extern void IR_PinChange0_Disable(void);

#endif /* _UCONTROLLER_H_ */
/** @} */
