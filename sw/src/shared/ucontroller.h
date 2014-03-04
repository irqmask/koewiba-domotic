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

 // Timer 0,1 and 2
 #define REG_TIMER0_IRQMSK      TIMSK
 #define REG_TIMER1_IRQMSK      TIMSK
 #define REG_TIMER2_IRQMSK      TIMSK
 #define REGBIT_TIMER0_TOIE     TOIE0
 #define REGBIT_TIMER0_OCIEA    OCIE0A
 #define REGBIT_TIMER0_OCIEB    OCIE0B
 #define REGBIT_TIMER1_TOIE     TOIE1
 #define REGBIT_TIMER1_OCIEA    OCIE1A
 #define REGBIT_TIMER1_OCIEB    OCIE1B
 #define REGBIT_TIMER2_TOIE     TOIE2
 #define REGBIT_TIMER2_OCIEA    OCIE2A
 #define REGBIT_TIMER2_OCIEB    OCIE2B
 
 // Signature byte addresses. TODO check for this processor!
 #define ADDR_SIGNATURE_BYTE0   0
 #define ADDR_SIGNATURE_BYTE1   2
 #define ADDR_SIGNATURE_BYTE2   4

#elif defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88P__)
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
 
 // Timer 0,1 and 2
 #define REG_TIMER0_IRQMSK      TIMSK0
 #define REG_TIMER1_IRQMSK      TIMSK1
 #define REG_TIMER2_IRQMSK      TIMSK2
 #define REGBIT_TIMER0_TOIE     TOIE0
 #define REGBIT_TIMER0_OCIEA    OCIE0A
 #define REGBIT_TIMER0_OCIEB    OCIE0B
 #define REGBIT_TIMER1_TOIE     TOIE1
 #define REGBIT_TIMER1_OCIEA    OCIE1A
 #define REGBIT_TIMER1_OCIEB    OCIE1B
 #define REGBIT_TIMER2_TOIE     TOIE2
 #define REGBIT_TIMER2_OCIEA    OCIE2A
 #define REGBIT_TIMER2_OCIEB    OCIE2B

 // Signature byte addresses. TODO check for this processor!
 #define ADDR_SIGNATURE_BYTE0   0
 #define ADDR_SIGNATURE_BYTE1   2
 #define ADDR_SIGNATURE_BYTE2   4
 #define SIGRD 5 // workaround for missing define in avr-headers

#elif defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__)
 // definition of prozessor depending global settings
 // TODO CV: this part can be ported maybe to a makefile include
 #define STARTADDR_APPVECTORS   0x0000
 #define STARTADDR_APPLICATION  0x003E
 #define STARTADDR_BOOTLOADER   0x1700
 #define MAXSIZE_APPLICATION    (STARTADDR_BOOTLOADER - STARTADDR_APPVECTORS)
 #define MAXSIZE_BOOTLOADER     0x0400

 // definition of processor specific registers
 // UART0
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
 
 // Sleepmodes
 #define INTERRUPT_PINCHANGE2   PCINT2_vect
 
 // SPI0
 #define INTERRUPT_SPI          SPI_STC_vect
 
 // Timer 0,1 and 2
 #define REG_TIMER0_IRQMSK      TIMSK0
 #define REG_TIMER1_IRQMSK      TIMSK1
 #define REG_TIMER2_IRQMSK      TIMSK2
 #define REGBIT_TIMER0_TOIE     TOIE0
 #define REGBIT_TIMER0_OCIEA    OCIE0A
 #define REGBIT_TIMER0_OCIEB    OCIE0B
 #define REGBIT_TIMER1_TOIE     TOIE1
 #define REGBIT_TIMER1_OCIEA    OCIE1A
 #define REGBIT_TIMER1_OCIEB    OCIE1B
 #define REGBIT_TIMER2_TOIE     TOIE2
 #define REGBIT_TIMER2_OCIEA    OCIE2A
 #define REGBIT_TIMER2_OCIEB    OCIE2B

 #ifdef AVRSTUDIO6
 // SPI0
 #define REG_SPCR0              SPCR0
 #define REG_SPDR0              SPDR0
 #define REG_SPSR0              SPSR0
 // SPCR
 #define REGBIT_SPE0            SPE0
 #define REGBIT_MSTR0           MSTR0
 // SPSR
 #define REGBIT_SPIF0           SPIF0
 #define REGBIT_SPI2X0          SPI2X0
 #else 
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
 #endif // AVRSTUDIO6

 // Signature byte addresses.
 #define ADDR_SIGNATURE_BYTE0   0
 #define ADDR_SIGNATURE_BYTE1   2
 #define ADDR_SIGNATURE_BYTE2   4
 
#elif defined (__AVR_ATtiny1634__)
 // definition of prozessor depending global settings
 // TODO CV: this part can be ported maybe to a makefile include
 #define STARTADDR_APPVECTORS   0x0000
 #define STARTADDR_APPLICATION  0x003E
 #define STARTADDR_BOOTLOADER   0x1700
 #define MAXSIZE_APPLICATION    (STARTADDR_BOOTLOADER - STARTADDR_APPVECTORS)
 #define MAXSIZE_BOOTLOADER     0x0400

 // definition of processor specific registers
 // UART0
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

 // Sleepmodes
 #define INTERRUPT_PINCHANGE2   PCINT2_vect
 
 // Timer 0,1 and 2
 #define REG_TIMER0_IRQMSK      TIMSK
 #define REG_TIMER1_IRQMSK      TIMSK
 #define REG_TIMER2_IRQMSK      TIMSK
 #define REGBIT_TIMER0_OCIEA    OCIE0A
 #define REGBIT_TIMER0_OCIEB    OCIE0B
 #define REGBIT_TIMER1_OCIEA    OCIE1A
 #define REGBIT_TIMER1_OCIEB    OCIE1B
 #define REGBIT_TIMER2_OCIEA    OCIE2A
 #define REGBIT_TIMER2_OCIEB    OCIE2B

// SPI
 #define REG_SPCR0              SPCR
 #define REG_SPDR0              UDR0
 #define REG_SPSR0              SPSR
 // SPCR
 #define REGBIT_SPE0            SPE
 #define REGBIT_MSTR0           MSTR
 // SPSR
 #define REGBIT_SPIF0           SPIF
 #define REGBIT_SPI2X0          SPI2X
#endif

#if defined (__AVR_ATmega8__)    || \
    defined (__AVR_ATmega88__)   || \
    defined (__AVR_ATmega88A__)  || \
    defined (__AVR_ATmega88P__)  || \
    defined (__AVR_ATmega324P__) || \
    defined (__AVR_ATmega324A__) || \
    defined (__AVR_ATtiny1634__)
 #define REG_TIMER0_TCCRA       TCCR0A
 #define REG_TIMER0_TCCRB       TCCR0B
 #define REG_TIMER0_TCNT        TCNT0
 #define REG_TIMER0_OCRA        OCR0A
 #define REG_TIMER0_OCRB        OCR0B
 #define REGBIT_TIMER0_WGM0     WGM00
 #define REGBIT_TIMER0_WGM1     WGM01
 #define REGBIT_TIMER0_WGM2     WGM02
 #define REGBIT_TIMER0_CS0      CS00
 #define REGBIT_TIMER0_CS1      CS01
 #define REGBIT_TIMER0_CS2      CS02
 #define INTERRUPT_TIMER0_COMPA TIMER0_COMPA_vect

 #define REG_TIMER1_TCCRA       TCCR1A
 #define REG_TIMER1_TCCRB       TCCR1B
 #define REG_TIMER1_TCNT        TCNT1
 #define REG_TIMER1_OCRA        OCR1A
 #define REG_TIMER1_OCRA_H      OCR1AH
 #define REG_TIMER1_OCRA_L      OCR1AL
 #define REG_TIMER1_OCRB        OCR1B
 #define REG_TIMER1_OCRB_H      OCR1BH
 #define REG_TIMER1_OCRB_L      OCR1BL
 #define REGBIT_TIMER1_WGM0     WGM10
 #define REGBIT_TIMER1_WGM1     WGM11
 #define REGBIT_TIMER1_WGM2     WGM12
 #define REGBIT_TIMER1_WGM3     WGM13
 #define REGBIT_TIMER1_CS0      CS10
 #define REGBIT_TIMER1_CS1      CS11
 #define REGBIT_TIMER1_CS2      CS12
 #define INTERRUPT_TIMER1_COMPA TIMER1_COMPA_vect
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

extern void IR_PinChange1_Enable(void);
extern void IR_PinChange1_Disable(void);
extern void IR_PinChange0_Enable(void);
extern void IR_PinChange0_Disable(void);

#endif /* _UCONTROLLER_H_ */
/** @} */
