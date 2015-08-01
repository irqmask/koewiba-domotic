/**
 * @addtogroup TEST_EEP
 *
 * @{
 * @file    main_eeptest.c
 * @brief   EEPRom test program.
 * Standalone application which test functionality of SPI and foremost EEProm
 * routines. It gives console output on UART at 9600baud 8N1.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "eeprom_spi.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

//! Calculation of UART baudrate
#define UART_UBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*16L)-1)
//! Chosen baudrate for this application
#define UART_BAUD_RATE 9600

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------



static char cVal2Hex(uint8_t uValue)
{
    if (uValue <= 9) return '0' + uValue;
    if (uValue <= 15) return 'A'- 0x0A + uValue;
    return '-';
}

void UART_vInit ( void )
{
    // 9600 Baud 8N1
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // UART TX und RX einschalten
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Asynchron 8N1

    UBRR0H = (uint8_t)(UART_UBRR_CALC( UART_BAUD_RATE, F_CPU ) >> 8);
    UBRR0L = (uint8_t)UART_UBRR_CALC( UART_BAUD_RATE, F_CPU );
}

void UART_vPutChar ( char cChar )
{
    if ( cChar == '\n' )
        UART_vPutChar('\r');

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = cChar;
}

void UART_vPutHex (uint8_t uValue)
{
    UART_vPutChar(cVal2Hex(uValue >> 4));
    UART_vPutChar(cVal2Hex(uValue & 0x0F));
}

void UART_vPutString ( const char *pcChar )
{
    while ( *pcChar != '\0' ) {
        UART_vPutChar(*pcChar++);
    }
}

static char UART_uGetChar ( void )
{
    // is char available?
    if ( !(UCSR0A & (1 << RXC0)) ) {
        // return NUL character if nothing has been received
        return '\0';
    }
    // return received char
    return UDR0;
}

static void UART_vHexDump ( const uint8_t *puArray, uint8_t uSize)
{
    uint8_t ii;

    for (ii=0; ii<uSize; ii++) {
        UART_vPutHex(puArray[ii]);
        if (ii>1 && (ii+1)%16==0) {
            UART_vPutChar('\n');
        } else {
            UART_vPutChar(' ');
        }
    }
    if (ii%16) UART_vPutChar('\n');
}

void vSPIBeforeTransmit(uint8_t uUArg)
{
    PORTB &= ~(1<<PB3);
}

void vSPIAfterTransmit(uint8_t uUArg)
{
     PORTB |= (1<<PB3);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main ( void )
{
    uint8_t ii, temp;
    uint16_t raddr = 0, written;

    // testpattern
    const uint8_t arrayii[] = {0xDE,0xAD,0xBE,0xEF,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    uint8_t inversearrayii[sizeof(arrayii)] = {0x00, 0x01, 0x02, 0x04, 0x08};

    // disable all interrupts
    cli();

    // initialize UART, SPI and EEProm
    UART_vInit();
    // enable sender and receiver
    DDRD |= (1<<PD2);
    PORTD |= (1<<PD2);

__RESTART:
    UART_vPutString("\n\nSPI Test Application\n");
    UART_vPutString("-------------------------\n");
    SPI_vMasterInit();
    DDRB |=  (1<<PB3);
    sei();

    ii = 0;
    while (UART_uGetChar() != 'C') {
        inversearrayii[0] = (1<<ii);
        SPI_uTransmit(inversearrayii, NULL, 4, vSPIBeforeTransmit, vSPIAfterTransmit, 0);
        UART_vPutChar('.');
        while(SPI_bIsBusy());
        _delay_ms(1);
        ii++;
        ii%=8;
    }

    SPI_vMasterInitBlk();
    EEP_vInit();

    UART_vPutString("\n\nEEProm Test Application\n");
    UART_vPutString("-------------------------\n");

    // 1st test: Read EEPRom for the first time
    // ------------------------------------------------------------------------

    UART_vPutString("\n1. Read current EEProm content:\n");
    for (raddr=0; raddr<sizeof(arrayii); raddr++) {
        EEP_uRead(raddr,1,&temp);
        UART_vPutHex(temp);
        if (raddr>1 && (raddr+1)%16==0) {
            UART_vPutChar('\n');
        } else {
            UART_vPutChar(' ');
        }
    }
    if (raddr%16) UART_vPutChar('\n');

    // 2nd test: Write testpattern into the EEPRom.
    // ------------------------------------------------------------------------
    UART_vPutString("\n2. Fill EEProm 1st time:\n");
    UART_vHexDump(&arrayii[0], sizeof(arrayii));
    written = EEP_uWrite(0, sizeof(arrayii),&arrayii[0]);
    UART_vPutString("...done.\n");


    // 3rd test: Read EEPRom again
    // ------------------------------------------------------------------------
    UART_vPutString("\n3. Read current EEProm content after 1st write:\n");
    for (raddr=0; raddr<sizeof(arrayii); raddr++) {
        EEP_uRead(raddr,1,&temp);
        UART_vPutHex(temp);
        if (raddr>1 && (raddr+1)%16==0) {
            UART_vPutChar('\n');
        } else {
            UART_vPutChar(' ');
        }
    }
    if (raddr%16) UART_vPutChar('\n');


    // 4th test: Write inverse pattern to EEPRom.
    // ------------------------------------------------------------------------
    UART_vPutString("\n4. Fill EEProm 2nd time with inverse pattern:\n");
    // create inverse testpattern
    for (ii=0; ii<sizeof(arrayii); ii++) {
        inversearrayii[ii] = ~arrayii[ii];
    }
    UART_vHexDump(&inversearrayii[0], sizeof(inversearrayii));
    written = EEP_uWrite(0, sizeof(inversearrayii),&inversearrayii[0]);
    UART_vPutString("...done.\n");

    // 5th test: Read EEPRom again
    // ------------------------------------------------------------------------
    UART_vPutString("\n5. Read current EEProm content after 2nd write:\n");
    for (raddr=0; raddr<sizeof(inversearrayii); raddr++) {
        EEP_uRead(raddr,1,&temp);
        UART_vPutHex(temp);
        if (raddr>1 && (raddr+1)%16==0) {
            UART_vPutChar('\n');
        } else {
            UART_vPutChar(' ');
        }
    }
    if (raddr%16) UART_vPutChar('\n');

    UART_vPutString("\n\nApplication halted. Restart with key \"R\"\n");
    while (1) {
        temp = UART_uGetChar();
        if (temp == 'R') goto __RESTART; // too lazy to do a while loop with all it's braces...
    }
    return 0;
}

/** @} */
