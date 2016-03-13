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
#include <util/delay.h>

// include
#include "moddef_common.h"
#include "prjtypes.h"
#include "ucontroller.h"

// shared
#include "crc16.h"
#include "eeprom_spi.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum {
	BLD_eExtEEPAddr_CtrlID      = MOD_eExtEEPAddr_AppStart + STARTADDR_VERSIONINFO,
    BLD_eExtEEPAddr_BoardID     = BLD_eExtEEPAddr_CtrlID + MOD_LEN_CONTROLLERID,
    BLD_eExtEEPAddr_BoardRev    = BLD_eExtEEPAddr_BoardID + MOD_LEN_BOARDID,
    BLD_eExtEEPAddr_AppID       = BLD_eExtEEPAddr_BoardRev + MOD_LEN_BOARDREV,
    BLD_eExtEEPAddr_AppVer      = BLD_eExtEEPAddr_AppID + MOD_LEN_APPID,
} bld_ext_eep_addr_t;

// --- Local variables ---------------------------------------------------------

//! Needed to get structured access to the internal eeprom.
uint8_t g_reg_internal_eep[MOD_eCfg_FirstAppSpecific] EEMEM;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

BOOL check_crc (uint16_t* length)
{
    uint32_t ii;
    uint16_t crc = 0, crcexpected = 0;
    uint8_t byte;

    *length = 0;

    // get length of the application
    eep_read(MOD_eExtEEPAddr_AppSize, MOD_LEN_APPSIZE, (uint8_t*)length);
    if (*length > EEPROM_SIZE) return FALSE;

    // read expected CRC value
    eep_read(MOD_eExtEEPAddr_AppCrc, MOD_LEN_APPCRC, (uint8_t*)&crcexpected);

    // calculate CRC of EEProm content
    crc = CRC_START_VALUE;
    for (ii=0; ii<(*length); ii++) {
        eep_read(MOD_eExtEEPAddr_AppStart + ii, 1, &byte);
        crc = crc_16_next_byte(crc, byte);
    }

    if (crc != crcexpected) return FALSE;

    return TRUE;
}

BOOL check_controller_id (void)
{
    uint8_t sigbyte, eepcontent = 0;

    eep_read(BLD_eExtEEPAddr_CtrlID + 0, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE0);
    if (sigbyte != eepcontent) return FALSE;

    eep_read(BLD_eExtEEPAddr_CtrlID + 1, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE1);
    if (sigbyte != eepcontent) return FALSE;

    eep_read(BLD_eExtEEPAddr_CtrlID + 2, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE2);
    if (sigbyte != eepcontent) return FALSE;

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

void program_flash (uint16_t address, uint16_t length)
{
    uint16_t ii, ww;
    uint8_t temp;

    // wait until eventually running write cycles of internal eeprom are
    // finished.
    eeprom_busy_wait ();

    do {
        // erase flash page and wait until the memory is erased.
        boot_page_erase (address);
        boot_spm_busy_wait ();

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
        // store buffer in flash page and wait until the memory is written.
        boot_page_write (address);
        boot_spm_busy_wait();
        // one flash page has been written
        address += SPM_PAGESIZE;
    } while (length > 0);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main ( void )
{
	uint8_t     bld_status;
    uint16_t    length = 0;
    register uint8_t temp, tempSREG;
    void (*start) ( void ) = (void*) 0x0000; // function pointer to application code

    //TODO remove after debug
    DDRC = 0x3F;
    PORTC = 0;
    _delay_ms(1000);

    // disable all interrupts
    cli();

    // move interrupt vector table to bootloader section
    tempSREG = SREG;
    temp = MCUCR;
    MCUCR = temp | (1 << IVCE);     // needed to unlock IVSEL
    MCUCR = temp | (1 << IVSEL);    // set IVSEL (within 4 cycles)
    SREG = tempSREG;

    bld_status = 0;

    // initialize external SPI eeprom
    spi_master_init_blk();
    eep_initialize();

    // check internal EEProm if a new application has to be flashed
    bld_status = eeprom_read_byte(&g_reg_internal_eep[MOD_eCfg_BldFlag]);
    bld_status &= ~((1<<eBldFlagCRCMismatch) |
    		        (1 << eBldFlagControllerTypeMismatch) |
    		        (1 << eBldFlagBoardTypeMismatch));


    if (bld_status & (1<<eBldFlagAppProgram)) {
        bld_status &= ~(1<<eBldFlagAppProgram);
        //TODO remove after debug
    	PORTC |= (1<<0);
        _delay_ms(1000);
        do {
            // check external EEProm
            if (!check_crc(&length)) {
                bld_status |= (1 << eBldFlagCRCMismatch);
                break;
            }
            //TODO remove after debug
        	PORTC |= (1<<1);
        	_delay_ms(1000);

            // check controller
            if (!check_controller_id()) {
                bld_status |= (1 << eBldFlagControllerTypeMismatch);
                //TODO remove after debug
                PORTC |= (1<<5);
                _delay_ms(1000);
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

            //TODO remove after debug
            PORTC |= (1<<2);
            _delay_ms(1000);
            // flash new application
            program_flash(0, length);
            //TODO remove after debug
            PORTC |= (1<<3);
            _delay_ms(1000);

            bld_status |= (1 << eBldFlagNewSWProgrammed);
            // re-enable RWW-section again. We need this if we want to jump back
            // to the application after bootloading.
            boot_rww_enable ();
        } while ( FALSE );
        eeprom_write_byte(&g_reg_internal_eep[MOD_eCfg_BldFlag], bld_status);
    }
    // debug code. TODO remove when bootloader code is finally stable
    /*PORTC |= (1<<4);
    _delay_ms(1000);
    PORTC &= 0xC0;
    _delay_ms(500);
    PORTC |= 0x3F;
    _delay_ms(500);
    PORTC &= 0xC0;
    _delay_ms(500);
    PORTC |= 0x3F;
    _delay_ms(500);
    PORTC &= 0xC0;
    _delay_ms(500);
    PORTC |= 0x3F;
    _delay_ms(500);
    PORTC &= 0xC0;
    _delay_ms(500);*/

    // restore interrupt vector table
    cli();
    temp = MCUCR;
    MCUCR = temp | (1<<IVCE);
    MCUCR = temp & ~(1<<IVSEL);

    // start application
    start();
    return 0;
}

/** @} */
