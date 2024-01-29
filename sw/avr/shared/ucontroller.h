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
#ifndef _UCONTROLLER_H_
#define _UCONTROLLER_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#if defined (__AVR_ATmega8__)
 #define REGISTER_UCSRA0        UCSRA
 #define REGISTER_UCSRB0        UCSRB
 #define REGISTER_UCSRC0        UCSRC
 #define REGISTER_UDR0          UDR
 #define REGISTER_UBRRH0        UBRRH
 #define REGISTER_UBRRL0        UBRRL
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
 #define REGISTER_UCSRA0         UCSR0A
 #define REGISTER_UCSRB0         UCSR0B
 #define REGISTER_UCSRC0         UCSR0C
 #define REGISTER_UDR0           UDR0
 #define REGISTER_UBRRH0         UBRR0H
 #define REGISTER_UBRRL0         UBRR0L
 #define REGBIT_RXCIE            RXCIE0
 #define REGBIT_UDRIE            UDRIE0
 #define REGBIT_TXCIE            TXCIE0
 #define REGBIT_RXEN             RXEN0
 #define REGBIT_TXEN             TXEN0
 #define REGBIT_FE               FE0
 #define INTERRUPT_USART_RXC0    USART_RX_vect
 #define INTERRUPT_USART_UDRE0   USART_UDRE_vect
 #define INTERRUPT_UART_TRANS0   USART_TX_vect
 #define INTERRUPT_PINCHANGE0    PCINT0_vect
 #define INTERRUPT_PINCHANGE1    PCINT1_vect
 #define INTERRUPT_PINCHANGE2    PCINT2_vect
 #define INTERRUPT_SPI           SPI_STC_vect

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

#elif defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
// UART
 #define REGISTER_UCSRA0         UCSR0A
 #define REGISTER_UCSRB0         UCSR0B
 #define REGISTER_UCSRC0         UCSR0C
 #define REGISTER_UDR0           UDR0
 #define REGISTER_UBRRH0         UBRR0H
 #define REGISTER_UBRRL0         UBRR0L
 #define REGBIT_RXCIE            RXCIE0
 #define REGBIT_UDRIE            UDRIE0
 #define REGBIT_TXCIE            TXCIE0
 #define REGBIT_RXEN             RXEN0
 #define REGBIT_TXEN             TXEN0
 #define REGBIT_FE               FE0
 #define INTERRUPT_USART_RXC0    USART_RX_vect
 #define INTERRUPT_USART_UDRE0   USART_UDRE_vect
 #define INTERRUPT_UART_TRANS0   USART_TX_vect
 #define INTERRUPT_PINCHANGE0    PCINT0_vect
 #define INTERRUPT_PINCHANGE1    PCINT1_vect
 #define INTERRUPT_PINCHANGE2    PCINT2_vect
 #define INTERRUPT_SPI           SPI_STC_vect

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
#elif defined (__AVR_ATmega328PB__)
// UART
 #define REGISTER_UCSRA0         UCSR0A
 #define REGISTER_UCSRB0         UCSR0B
 #define REGISTER_UCSRC0         UCSR0C
 #define REGISTER_UDR0           UDR0
 #define REGISTER_UBRRH0         UBRR0H
 #define REGISTER_UBRRL0         UBRR0L
 #define REGBIT_RXCIE            RXCIE0
 #define REGBIT_UDRIE            UDRIE0
 #define REGBIT_TXCIE            TXCIE0
 #define REGBIT_RXEN             RXEN0
 #define REGBIT_TXEN             TXEN0
 #define REGBIT_FE               FE0
 #define INTERRUPT_USART_RXC0    USART0_RX_vect
 #define INTERRUPT_USART_UDRE0   USART0_UDRE_vect
 #define INTERRUPT_UART_TRANS0   USART0_TX_vect
 #define INTERRUPT_PINCHANGE0    PCINT0_vect
 #define INTERRUPT_PINCHANGE1    PCINT1_vect
 #define INTERRUPT_PINCHANGE2    PCINT2_vect
 #define INTERRUPT_SPI           SPI_STC_vect

// SPI
 #define REG_SPCR0              SPCR0
 #define REG_SPDR0              SPDR0
 #define REG_SPSR0              SPSR0
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
#elif defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__) || defined (__AVR_ATmega324PA__)
 // definition of processor specific registers
 // UART0
 #define REGISTER_UCSRA0        UCSR0A
 #define REGISTER_UCSRB0        UCSR0B
 #define REGISTER_UCSRC0        UCSR0C
 #define REGISTER_UDR0          UDR0
 #define REGISTER_UBRRH0        UBRR0H
 #define REGISTER_UBRRL0        UBRR0L
 #define REGBIT_RXCIE           RXCIE0
 #define REGBIT_UDRIE           UDRIE0
 #define REGBIT_TXCIE           TXCIE0
 #define REGBIT_RXEN            RXEN0
 #define REGBIT_TXEN            TXEN0
 #define REGBIT_FE              FE0
 #define REGBIT_UDRE            UDRE0
 #define REGBIT_USBS            USBS0
 #define REGBIT_UCSZ0           UCSZ00
 #define REGBIT_UCSZ1           UCSZ01
 #define REGBIT_UCSZ2           UCSZ02
 #define INTERRUPT_USART_RXC0   USART0_RX_vect
 #define INTERRUPT_USART_UDRE0  USART0_UDRE_vect
 #define INTERRUPT_UART_TRANS0  USART0_TX_vect

 #define REGISTER_UCSRA1        UCSR1A
 #define REGISTER_UCSRB1        UCSR1B
 #define REGISTER_UCSRC1        UCSR1C
 #define REGISTER_UDR1          UDR1
 #define REGISTER_UBRRH1        UBRR1H
 #define REGISTER_UBRRL1        UBRR1L
 #define INTERRUPT_USART_RXC1   USART1_RX_vect
 #define INTERRUPT_USART_UDRE1  USART1_UDRE_vect
 #define INTERRUPT_UART_TRANS1  USART1_TX_vect

 // Sleep-Modes
 #define INTERRUPT_PINCHANGE0   PCINT0_vect
 #define INTERRUPT_PINCHANGE1   PCINT1_vect
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
 #define REG_SPCR0              SPCR0
 #define REG_SPDR0              SPDR0
 #define REG_SPSR0              SPSR0
 // SPCR
 #define REGBIT_SPE0            SPE0
 #define REGBIT_MSTR0           MSTR0
 // SPSR
 #define REGBIT_SPIF0           SPIF0
 #define REGBIT_SPI2X0          SPI2X0
 #endif // AVRSTUDIO6

 // Signature byte addresses.
 #define ADDR_SIGNATURE_BYTE0   0
 #define ADDR_SIGNATURE_BYTE1   2
 #define ADDR_SIGNATURE_BYTE2   4

#elif defined (__AVR_ATtiny1634__)
 // UCSR0C is used twice (in uart- and spi-mode) and BIT_1 has different names (uart: UCPHA0   spi: UCSZ00).
 // This is not included in "avr/iotn1634" so it has to be defined at this point.
 #define UCPHA0 UCSZ00
 #define UCPHA1 UCSZ10

// Signature byte addresses. TODO check for this processor!
 #define ADDR_SIGNATURE_BYTE0   0
 #define ADDR_SIGNATURE_BYTE1   2
 #define ADDR_SIGNATURE_BYTE2   4
 #define SIGRD 5 // workaround for missing define in avr-headers

 // definition of processor specific registers
 // UART0
 #define REGISTER_UCSRA         UCSR0A
 #define REGISTER_UCSRB         UCSR0B
 #define REGISTER_UCSRC         UCSR0C
 #define REGISTER_UCSRA0        UCSR0A
 #define REGISTER_UCSRB0        UCSR0B
 #define REGISTER_UCSRC0        UCSR0C
 #define REGISTER_UCSRA1        UCSR1A
 #define REGISTER_UCSRB1        UCSR1B
 #define REGISTER_UCSRC1        UCSR1C
 #define REGISTER_UDR           UDR0
 #define REGISTER_UDR0          UDR0
 #define REGISTER_UDR1          UDR1
 #define REGISTER_UBRRH         UBRR0H
 #define REGISTER_UBRRL         UBRR0L
 #define REGISTER_UBRRH0        UBRR0H
 #define REGISTER_UBRRL0        UBRR0L
 #define REGISTER_UBRRH1        UBRR1H
 #define REGISTER_UBRRL1        UBRR1L
 #define REGBIT_RXCIE           RXCIE0
 #define REGBIT_RXCIE0          RXCIE0
 #define REGBIT_RXCIE1          RXCIE1
 #define REGBIT_UDRE            UDRE0
 #define REGBIT_UDRE0           UDRE0
 #define REGBIT_UDRE1           UDRE1
 #define REGBIT_UDRIE           UDRIE0
 #define REGBIT_UDRIE0          UDRIE0
 #define REGBIT_UDRIE1          UDRIE1
 #define REGBIT_TXCIE           TXCIE0
 #define REGBIT_TXCIE0          TXCIE0
 #define REGBIT_TXCIE1          TXCIE1
 #define REGBIT_RXEN            RXEN0
 #define REGBIT_RXEN0           RXEN0
 #define REGBIT_RXEN1           RXEN1
 #define REGBIT_TXEN            TXEN0
 #define REGBIT_TXEN0           TXEN0
 #define REGBIT_TXEN1           TXEN1
 #define REGBIT_FE              FE0
 #define REGBIT_FE0             FE0
 #define REGBIT_FE1             FE1
 #define REGBIT_UDRE            UDRE0
 #define REGBIT_USBS            USBS0
 #define REGBIT_UCSZ2           UCSZ02
 #define REGBIT_UCSZ1           UCSZ01
 #define REGBIT_UCSZ0           UCSZ00
 #define INTERRUPT_USART_RXC    USART0_RX_vect
 #define INTERRUPT_USART_RXC0   USART0_RX_vect
 #define INTERRUPT_USART_RXC1   USART1_RX_vect
 #define INTERRUPT_USART_UDRE   USART0_UDRE_vect
 #define INTERRUPT_USART_UDRE0  USART0_UDRE_vect
 #define INTERRUPT_USART_UDRE1  USART1_UDRE_vect
 #define INTERRUPT_UART_TRANS   USART0_TX_vect
 #define INTERRUPT_UART_TRANS0  USART0_TX_vect
 #define INTERRUPT_UART_TRANS1  USART1_TX_vect
 // Sleepmodes
 #define INTERRUPT_PINCHANGE0   PCINT0_vect
 #define INTERRUPT_PINCHANGE1   PCINT1_vect
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
 #define REG_SPDR0              UDR0
 #define REG_SPDR1              UDR1
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
    defined (__AVR_ATmega328__)  || \
    defined (__AVR_ATmega328P__)  || \
	defined (__AVR_ATmega328PB__)  || \
    defined (__AVR_ATmega324P__) || \
    defined (__AVR_ATmega324A__) || \
    defined (__AVR_ATmega324PA__) || \
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

 #define REG_TIMER2_TCCRA       TCCR2A
 #define REG_TIMER2_TCCRB       TCCR2B
 #define REG_TIMER2_TCNT        TCNT2
 #define REG_TIMER2_OCRA        OCR2A
 #define REG_TIMER2_OCRB        OCR2B
 #define REG_TIMER2_OCRB_H      OCR2BH
 #define REG_TIMER2_OCRB_L      OCR2BL
 #define REGBIT_TIMER2_WGM0     WGM20
 #define REGBIT_TIMER2_WGM1     WGM21
 #define REGBIT_TIMER2_WGM2     WGM22
 #define REGBIT_TIMER2_WGM3     WGM23
 #define REGBIT_TIMER2_CS0      CS20
 #define REGBIT_TIMER2_CS1      CS21
 #define REGBIT_TIMER2_CS2      CS22
 #define INTERRUPT_TIMER2_COMPA TIMER2_COMPA_vect
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _UCONTROLLER_H_ */
/** @} */
