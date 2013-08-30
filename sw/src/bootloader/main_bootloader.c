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
#include "modcfg_common.h"
#include "prjtypes.h"
#include "ucontroller.h"

// shared
#include "eeprom_spi.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

// Hilfsmakro zur UBRR-Berechnung ("Formel" laut Datenblatt)
#define UART_UBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*16L)-1)
#define UART_BAUD_RATE 9600

#define BLD_LEN_CONTROLLERID    4
#define BLD_LEN_BOARDID         2
#define BLD_LEN_BOARDREV        1
#define BLD_LEN_APPID           2
#define BLD_LEN_APPVER          2

// --- Type definitions --------------------------------------------------------

typedef enum {
    BLD_eExtEEPAddr_CtrlID      = MOD_eExtEEPAddr_AppStart + STARTADDR_APPLICATION,
    BLD_eExtEEPAddr_BoardID     = BLD_eExtEEPAddr_CtrlID + BLD_LEN_CONTROLLERID,
    BLD_eExtEEPAddr_BoardRev    = BLD_eExtEEPAddr_BoardID + BLD_LEN_BOARDID,
    BLD_eExtEEPAddr_AppID       = BLD_eExtEEPAddr_BoardRev + BLD_LEN_BOARDREV,
    BLD_eExtEEPAddr_AppVer      = BLD_eExtEEPAddr_AppID + BLD_LEN_APPID,
} BLD_eExtEEPAddr_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

uint8_t BLD_uStatus;                    //!< Status byte of the bootloader

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void UART_vInit ( void )
{
    // 9600 Baud 8N1
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // UART TX und RX einschalten
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Asynchron 8N1

    UBRR0H = (uint8_t)(UART_UBRR_CALC( UART_BAUD_RATE, F_CPU ) >> 8);
    UBRR0L = (uint8_t)UART_UBRR_CALC( UART_BAUD_RATE, F_CPU );
}

static void UART_vPutChar ( char cChar )
{
    if ( cChar == '\n' )
        UART_vPutChar('\r');

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = cChar;
}

static char cVal2Hex(uint8_t uValue)
{
    if (uValue <= 9) return '0' + uValue;
    if (uValue <= 15) return 'A'- 0x0A + uValue;
    return '-';
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

BOOL bCheckCRC (uint32_t* puLength)
{
    uint32_t ii;
    uint16_t crc = 0, crcexpected = 0;
    uint8_t byte;

    *puLength = 0;

    // get length of the application
    EEP_uRead(MOD_eExtEEPAddr_AppSize, 4, (uint8_t*)puLength);
    if (*puLength > EEPROM_SIZE) return FALSE;

    // read expected CRC value
    EEP_uRead(MOD_eExtEEPAddr_AppCrc, 2, (uint8_t*)&crcexpected);

    // calculate CRC of EEProm content
    for (ii=0; ii<*puLength; ii++) {
        EEP_uRead(MOD_eExtEEPAddr_AppStart, 1, &byte);
        crc = _crc16_update(crc, byte);
    }
    if (crc != crcexpected) return FALSE;

    return TRUE;
}

BOOL bCheckControllerID (void)
{
    uint8_t sigbyte, eepcontent = 0;

    EEP_uRead(BLD_eExtEEPAddr_CtrlID + 0, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(0);
    UART_vPutHex(sigbyte);
    //if (sigbyte != eepcontent) return FALSE;

    EEP_uRead(BLD_eExtEEPAddr_CtrlID + 1, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(1);
    UART_vPutHex(sigbyte);
    //if (sigbyte != eepcontent) return FALSE;

    EEP_uRead(BLD_eExtEEPAddr_CtrlID + 2, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(2);
    UART_vPutHex(sigbyte);
    //if (sigbyte != eepcontent) return FALSE;

    return TRUE;
}

BOOL bCheckBoardType (void)
{
    return TRUE;
}

BOOL bHasAppIDChanged (void)
{
    return TRUE;
}

BOOL bHasAppVersionChanged (void)
{
    return TRUE;
}

void vProgramFlash (uint32_t uAddr, uint32_t uLength)
{
    uint16_t ii, ww;
    uint8_t temp;

    eeprom_busy_wait ();

    do {

        boot_page_erase (uAddr);
        boot_spm_busy_wait ();      /* Wait until the memory is erased. */

        // only copy full 16bit words to flash memory
        if (uLength % 2) uLength--;

        for (ii=0; ii<SPM_PAGESIZE; ii+=2) {
            // read a word and convert to litte endian
            EEP_uRead(MOD_eExtEEPAddr_AppStart + uAddr + ii, 1, &temp);
            ww = temp;
            EEP_uRead(MOD_eExtEEPAddr_AppStart + uAddr + ii + 1, 1, &temp);
            ww |= temp << 8;
            boot_page_fill (uAddr + ii, ww);

            uLength -= 2;
            if (uLength == 0) break;
        }
        boot_page_write (uAddr);     /* Store buffer in flash page.      */
        boot_spm_busy_wait();       /* Wait until the memory is written.*/
        uAddr += SPM_PAGESIZE;
    } while (uLength > 0);
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

    BLD_uStatus = 0;

    // initialize UART and stdout stream
    UART_vInit();
    SPI_vMasterInitBlk();
    EEP_vInit();

    // enable sender and receiver
    DDRD |= (1<<PD2);
    PORTD |= (1<<PD2);

    // initialize LEDs and buttons
    DDRB |= (1<<PB0 | 1<<PB1);              // output
    PORTB &= ~(1<<PB0 | 1<<PB1);            // output off

    DDRC &= ~(1<<PC5 | 1<<PC6 | 1<<PC7);    // input
    PORTC |= (1<<PC5 | 1<<PC6 | 1<<PC7);    // pull-up resistors

    // check internal EEProm if a new application has to be flashed
    BLD_uStatus = eeprom_read_byte((uint8_t*)MOD_eCfg_BldFlag);
    if (BLD_uStatus & (1<<eBldFlagAppProgram)) {
        do {
            // check external EEProm
            if (!bCheckCRC(&length)) {
                BLD_uStatus |= (1 << eBldFlagCRCMismatch);
                UART_vPutString("CRCFailed");
                break;
            }

            // check controller
            if (!bCheckControllerID()) {
                BLD_uStatus |= (1 << eBldFlagControllerTypeMismatch);
                break;
            }

            // check board type
            if (!bCheckBoardType()) {
                BLD_uStatus |= (1 << eBldFlagBoardTypeMismatch);
                break;
            }

            // check application ID
            if (bHasAppIDChanged()) {
                BLD_uStatus |= (1 << eBldFlagAppIDChanged);
            }

            // check application version
            if (bHasAppVersionChanged()) {
                BLD_uStatus |= (1 << eBldFlagAppVersionChanged);
            }

            // flash new application
            vProgramFlash(0, length);
            BLD_uStatus |= (1 << eBldFlagNewSWProgrammed);
            // Reenable RWW-section again. We need this if we want to jump back
            // to the application after bootloading.
            boot_rww_enable ();
            BLD_uStatus &= ~(1<<eBldFlagAppProgram);
            eeprom_write_byte((uint8_t*)MOD_eCfg_BldFlag, BLD_uStatus);
        } while ( FALSE );

    }

    while (1);
    // start application
    start();

    return 0;
}

/** @} */
