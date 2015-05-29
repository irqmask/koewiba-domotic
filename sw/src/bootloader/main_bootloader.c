/**
 * @addtogroup BOOTLOADER
 *
 * @{
 * @file    main_bootloader.c
 * @brief   Bootloader TODO CV
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include <util/delay.h>

// include
#include "moddef_common.h"
#include "prjtypes.h"
#include "ucontroller.h"

// shared
#include "eeprom_spi.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

// Hilfsmakro zur UBRR-Berechnung ("Formel" laut Datenblatt)
#define UART_UBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*16L)-1)
#define UART_BAUD_RATE 9600

// --- Type definitions --------------------------------------------------------

typedef enum {
    BLD_eExtEEPAddr_CtrlID      = MOD_eExtEEPAddr_AppStart + STARTADDR_VERSIONINFO,
    BLD_eExtEEPAddr_BoardID     = BLD_eExtEEPAddr_CtrlID + MOD_LEN_CONTROLLERID,
    BLD_eExtEEPAddr_BoardRev    = BLD_eExtEEPAddr_BoardID + MOD_LEN_BOARDID,
    BLD_eExtEEPAddr_AppID       = BLD_eExtEEPAddr_BoardRev + MOD_LEN_BOARDREV,
    BLD_eExtEEPAddr_AppVer      = BLD_eExtEEPAddr_AppID + MOD_LEN_APPID,
} bld_ext_eep_addr_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

uint8_t bld_status;                     //!< Status byte of the bootloader

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void uart_init ( void )
{
    // 9600 Baud 8N1
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // UART TX und RX einschalten
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Asynchron 8N1

    UBRR0H = (uint8_t)(UART_UBRR_CALC( UART_BAUD_RATE, F_CPU ) >> 8);
    UBRR0L = (uint8_t)UART_UBRR_CALC( UART_BAUD_RATE, F_CPU );
}

static void uart_put_char ( char cchar )
{
    if ( cchar == '\n' )
        uart_put_char('\r');

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = cchar;
}

static char val2hex(uint8_t value)
{
    if (value <= 9) return '0' + value;
    if (value <= 15) return 'A'- 0x0A + value;
    return '-';
}

void uart_put_hex (uint8_t value)
{
    uart_put_char(val2hex(value >> 4));
    uart_put_char(val2hex(value & 0x0F));
}

void uart_put_string ( const char *string )
{
    while ( *string != '\0' ) {
        uart_put_char(*string++);
    }
}

static void uart_hex_dump ( const uint8_t *array, uint8_t size)
{
    uint8_t ii;

    for (ii=0; ii<size; ii++) {
        uart_put_hex(array[ii]);
        if (ii>1 && (ii+1)%16==0) {
            uart_put_char('\n');
        } else {
            uart_put_char(' ');
        }
    }
    if (ii%16) uart_put_char('\n');
}

BOOL check_crc (uint32_t* length)
{
    uint32_t ii;
    uint16_t crc = 0, crcexpected = 0;
    uint8_t byte;

    *length = 0;

    // get length of the application
    eep_read(MOD_eExtEEPAddr_AppSize, 4, (uint8_t*)length);
    if (*length > EEPROM_SIZE) return FALSE;

    // read expected CRC value
    eep_read(MOD_eExtEEPAddr_AppCrc, 2, (uint8_t*)&crcexpected);

    // calculate CRC of EEProm content
    for (ii=0; ii<*length; ii++) {
        eep_read(MOD_eExtEEPAddr_AppStart, 1, &byte);
        crc = _crc16_update(crc, byte);
    }
    if (crc != crcexpected) return FALSE;

    return TRUE;
}

BOOL check_controller_id (void)
{
    uint8_t sigbyte, eepcontent = 0;

    eep_read(BLD_eExtEEPAddr_CtrlID + 0, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE0);
    uart_put_hex(sigbyte);
    //if (sigbyte != eepcontent) return FALSE;

    eep_read(BLD_eExtEEPAddr_CtrlID + 1, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE1);
    uart_put_hex(sigbyte);
    //if (sigbyte != eepcontent) return FALSE;

    eep_read(BLD_eExtEEPAddr_CtrlID + 2, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE2);
    uart_put_hex(sigbyte);
    //if (sigbyte != eepcontent) return FALSE;

    return TRUE;
}

BOOL check_board_type (void)
{
    return TRUE;
}

BOOL has_app_id_changed (void)
{
    return TRUE;
}

BOOL has_app_version_changed (void)
{
    return TRUE;
}

void program_flash (uint32_t address, uint32_t length)
{
    uint16_t ii, ww;
    uint8_t temp;

    eeprom_busy_wait ();

    do {

        boot_page_erase (address);
        boot_spm_busy_wait ();      /* Wait until the memory is erased. */

        // only copy full 16bit words to flash memory
        if (length % 2) length--;

        for (ii=0; ii<SPM_PAGESIZE; ii+=2) {
            // read a word and convert to litte endian
            eep_read(MOD_eExtEEPAddr_AppStart + address + ii, 1, &temp);
            ww = temp;
            eep_read(MOD_eExtEEPAddr_AppStart + address + ii + 1, 1, &temp);
            ww |= temp << 8;
            boot_page_fill (address + ii, ww);

            length -= 2;
            if (length == 0) break;
        }
        boot_page_write (address);     /* Store buffer in flash page.      */
        boot_spm_busy_wait();       /* Wait until the memory is written.*/
        address += SPM_PAGESIZE;
    } while (length > 0);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main ( void )
{
    uint32_t length = 0;
    register uint8_t temp, tempSREG;
    void (*start) ( void ) = (void*) 0x0000; // function pointer to application code

    // disable all interrupts
    cli();

    // move interrupt vector table to bootloader section
    tempSREG = SREG;
    temp = MCUCR;
    MCUCR = temp | (1 << IVCE); // needed to unlock IVSEL
    MCUCR = temp | (1 << IVSEL); // set IVSEL (within 4 cycles)
    SREG = tempSREG;

    bld_status = 0;

    // initialize UART and stdout stream
    uart_init();
    spi_master_init_blk();
    eep_initialize();

    // enable sender and receiver
    DDRD |= (1<<PD2);
    PORTD |= (1<<PD2);

    // initialize LEDs and buttons
    DDRB |= (1<<PB0 | 1<<PB1);              // output
    PORTB &= ~(1<<PB0 | 1<<PB1);            // output off

    // TODO remove after test
    check_controller_id();

    // check internal EEProm if a new application has to be flashed
    bld_status = eeprom_read_byte((uint8_t*)MOD_eReg_BldFlag);
    if (bld_status & (1<<eBldFlagAppProgram)) {
        do {
            // check external EEProm
            if (!check_crc(&length)) {
                bld_status |= (1 << eBldFlagCRCMismatch);
                uart_put_string("CRCFailed");
                break;
            }

            // check controller
            if (!check_controller_id()) {
                bld_status |= (1 << eBldFlagControllerTypeMismatch);
                break;
            }

            // check board type
            if (!check_board_type()) {
                bld_status |= (1 << eBldFlagBoardTypeMismatch);
                break;
            }

            // check application ID
            if (has_app_id_changed()) {
                bld_status |= (1 << eBldFlagAppIDChanged);
            }

            // check application version
            if (has_app_version_changed()) {
                bld_status |= (1 << eBldFlagAppVersionChanged);
            }

            // flash new application
            program_flash(0, length);
            bld_status |= (1 << eBldFlagNewSWProgrammed);
            // Reenable RWW-section again. We need this if we want to jump back
            // to the application after bootloading.
            boot_rww_enable ();
            bld_status &= ~(1<<eBldFlagAppProgram);
            eeprom_write_byte((uint8_t*)MOD_eReg_BldFlag, bld_status);
        } while ( FALSE );

    }

    // start application
    start();

    return 0;
}

/** @} */
