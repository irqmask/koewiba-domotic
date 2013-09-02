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
#include "uart.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------




void vDoMiscTests(void)
{
    typedef union {
        uint32_t u32Bit;
        uint16_t u16Bit;
        uint8_t auArray[4];
    } multival_t;
    multival_t val;

    UART_vPutString("\nAVR Endianess Test\n");
    UART_vPutString("32bit Value 0x12345678 in array ");
    val.u32Bit = 0x12345678;
    UART_vPutHex(val.auArray[0]);
    UART_vPutChar(' ');
    UART_vPutHex(val.auArray[1]);
    UART_vPutChar(' ');
    UART_vPutHex(val.auArray[2]);
    UART_vPutChar(' ');
    UART_vPutHex(val.auArray[3]);
    UART_vPutChar('\n');

    UART_vPutString("16bit Value 0x1234     in array ");
    val.u32Bit = 0;
    val.u16Bit = 0x1234;
    UART_vPutHex(val.auArray[0]);
    UART_vPutChar(' ');
    UART_vPutHex(val.auArray[1]);
    UART_vPutChar(' ');
    UART_vPutHex(val.auArray[2]);
    UART_vPutChar(' ');
    UART_vPutHex(val.auArray[3]);
    UART_vPutChar('\n');

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
    UART_vInit(9600);
    // enable sender and receiver
    DDRD |= (1<<PD2);
    PORTD |= (1<<PD2);
    DDRB |= (1<<PB0 | 1<<PB1);
    PORTB &=~(1<<PB0 | 1<<PB1);
    
__RESTART:
    SPI_vMasterInitBlk();
    EEP_vInit();

    sei();

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

    vDoMiscTests();

    UART_vPutString("\n\nApplication halted. Restart with key \"R\"\n");
    while (1) {
        temp = (UART_uReceive() & 0x00FF);
        if (temp == 'R') goto __RESTART; // too lazy to do a while loop with all it's braces...
    }
    return 0;
}

/** @} */
