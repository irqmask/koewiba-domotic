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

void do_misc_tests (void)
{
    typedef union
    {
        uint32_t u32Bit;
        uint16_t u16Bit;
        uint8_t auArray[4];
    } multival_t;
    multival_t val;

    uart_put_string_blk("\nAVR Endianess Test\n");
    uart_put_string_blk("32bit Value 0x12345678 in array ");
    val.u32Bit = 0x12345678;
    uart_put_hex8_blk(val.auArray[0]);
    uart_put_char_blk(' ');
    uart_put_hex8_blk(val.auArray[1]);
    uart_put_char_blk(' ');
    uart_put_hex8_blk(val.auArray[2]);
    uart_put_char_blk(' ');
    uart_put_hex8_blk(val.auArray[3]);
    uart_put_char_blk('\n');

    uart_put_string_blk("16bit Value 0x1234     in array ");
    val.u32Bit = 0;
    val.u16Bit = 0x1234;
    uart_put_hex8_blk(val.auArray[0]);
    uart_put_char_blk(' ');
    uart_put_hex8_blk(val.auArray[1]);
    uart_put_char_blk(' ');
    uart_put_hex8_blk(val.auArray[2]);
    uart_put_char_blk(' ');
    uart_put_hex8_blk(val.auArray[3]);
    uart_put_char_blk('\n');

}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (void)
{
    uint8_t ii, temp, read_ok = 0;
    uint16_t raddr = 0, written;

    // testpattern
    const uint8_t arrayii[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16 };
    uint8_t inversearrayii[sizeof(arrayii)] = { 0x00, 0x01, 0x02, 0x04, 0x08 };

    // disable all interrupts
    cli();

    // initialize UART, SPI and EEProm
    uart_init_blk(9600);

__RESTART:
#ifdef __AVR_ATtiny1634__
    // enable sender
    DDRA |= (1 << PA6);
    PORTA |= (1 << PA6);
#endif
    uart_put_string_blk("\n\nEEProm Test Application\n");
    uart_put_string_blk("-------------------------\n");

    spi_master_init_blk();
    uart_put_string_blk("SPI Init done.\n");
    eep_initialize();
    uart_put_string_blk("EEP Init done.\n");
    sei();


    // 1st test: Read EEPRom for the first time
    // ------------------------------------------------------------------------

    uart_put_string_blk("\n1. Read current EEProm content:\n");
    for (raddr = 0; raddr < sizeof(arrayii); raddr++) {
        eep_read(raddr, 1, &temp);
        uart_put_hex8_blk(temp);
        if (raddr > 1 && (raddr + 1) % 16 == 0) {
            uart_put_char_blk('\n');
        } else {
            uart_put_char_blk(' ');
        }
    }
    if (raddr % 16) uart_put_char_blk('\n');

    // 2nd test: Write testpattern into the EEPRom.
    // ------------------------------------------------------------------------
    uart_put_string_blk("\n2. Fill EEProm 1st time:\n");
    uart_hex_dump_blk(&arrayii[0], sizeof(arrayii));
    written = eep_write(0, sizeof(arrayii), &arrayii[0]);
    uart_put_string_blk("...done.\n");

    // 3rd test: Read EEPRom again
    // ------------------------------------------------------------------------
    read_ok = 1;
    uart_put_string_blk("\n3. Read current EEProm content after 1st write:\n");
    for (raddr = 0; raddr < sizeof(arrayii); raddr++) {
        if (temp != arrayii[raddr]) read_ok = 0;
        eep_read(raddr, 1, &temp);
        uart_put_hex8_blk(temp);
        if (raddr > 1 && (raddr + 1) % 16 == 0) {
            uart_put_char_blk('\n');
        } else {
            uart_put_char_blk(' ');
        }
    }
    if (raddr % 16) uart_put_char_blk('\n');
    if (read_ok == 1) uart_put_string_blk("...done.\n");
    else uart_put_string_blk("..failed.\n");

    // 4th test: Write inverse pattern to EEPRom.
    // ------------------------------------------------------------------------
    uart_put_string_blk("\n4. Fill EEProm 2nd time with inverse pattern:\n");
    // create inverse testpattern
    for (ii = 0; ii < sizeof(arrayii); ii++) {
        inversearrayii[ii] = ~arrayii[ii];
    }
    uart_hex_dump_blk(&inversearrayii[0], sizeof(inversearrayii));
    written = eep_write(0, sizeof(inversearrayii), &inversearrayii[0]);
    uart_put_string_blk("...done.\n");

    // 5th test: Read EEPRom again
    // ------------------------------------------------------------------------
    read_ok = 1;
    uart_put_string_blk("\n5. Read current EEProm content after 2nd write:\n");
    for (raddr = 0; raddr < sizeof(inversearrayii); raddr++) {
        eep_read(raddr, 1, &temp);
        uart_put_hex8_blk(temp);
        if (temp != inversearrayii[raddr]) read_ok = 0;
        if (raddr > 1 && (raddr + 1) % 16 == 0) {
            uart_put_char_blk('\n');
        } else {
            uart_put_char_blk(' ');
        }
    }
    if (raddr % 16) uart_put_char_blk('\n');
    if (read_ok == 1) uart_put_string_blk("...done.\n");
    else uart_put_string_blk("..failed.\n");


    do_misc_tests();

    uart_put_string_blk("\n\nApplication halted. Restart with key \"R\"\n");
    while (1) {
#ifdef __AVR_ATtiny1634__
    // enable receiver / disable sender
    PORTA &= ~(1 << PA6);
#endif

        temp = (uart_get_char_blk() & 0x00FF);
        if (temp == 'R') goto __RESTART;
        // too lazy to do a while loop with all it's braces...
    }
    return 0;
}

/** @} */
