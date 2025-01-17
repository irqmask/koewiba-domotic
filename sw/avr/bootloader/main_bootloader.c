/**
 * @addtogroup BOOTLOADER
 *
 * @{
 * @file    main_bootloader.c
 * @brief   Common bootloader code for atmega328, atmega324 and attiny1634.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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
// --- Include section ---------------------------------------------------------
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

// include
#include "moddef_common.h"
#include "prjtypes.h"
#include "ucontroller.h"

// shared
#include "bootloader.h"
#include "crc16.h"
#include "eeprom_spi.h"
#include "spi.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

#ifdef DEBUG_LED_OUTPUT
 #include <util/delay.h>
#endif

// --- Definitions -------------------------------------------------------------

#ifdef DEBUG_LED_OUTPUT
 #define DBG_INIT()         DBG_DDR |= DBG_ALL_MASK;  \
                            DBG_PORT &= ~DBG_ALL_MASK; \
                            _delay_ms(1000);
 #define DBG_SET_PIN0()     DBG_PORT |= (1<<DBG_0_PIN); \
                            _delay_ms(500);
 #define DBG_SET_PIN1()     DBG_PORT |= (1<<DBG_1_PIN); \
                            _delay_ms(500);
 #define DBG_SET_PIN2()     DBG_PORT |= (1<<DBG_2_PIN); \
                            _delay_ms(500);
 #define DBG_SET_PIN3()     DBG_PORT |= (1<<DBG_3_PIN); \
                            _delay_ms(500);
 #define DBG_SET_PIN4()     DBG_PORT |= (1<<DBG_4_PIN); \
                            _delay_ms(500);
 #define DBG_SET_PIN5()     DBG_PORT |= (1<<DBG_5_PIN); \
                            _delay_ms(500);
 #define DBG_ALL_ON()       DBG_PORT |= DBG_ALL_MASK; \
                            _delay_ms(300);
 #define DBG_ALL_OFF()      DBG_PORT &= ~DBG_ALL_MASK; \
                            _delay_ms(300);
#else
 #define DBG_INIT()
 #define DBG_SET_PIN0()
 #define DBG_SET_PIN1()
 #define DBG_SET_PIN2()
 #define DBG_SET_PIN3()
 #define DBG_SET_PIN4()
 #define DBG_SET_PIN5()
 #define DBG_ALL_ON()
 #define DBG_ALL_OFF()
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

//! Needed to get structured access to the internal eeprom.
uint8_t g_reg_internal_eep[MOD_eCfg_FirstAppSpecific] EEMEM;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Disable watchdog before main starts.
 */
void init_wdt (void) __attribute__((naked)) __attribute__((section(".init3")));
void init_wdt(void)
{
    MCUSR = 0;
    wdt_disable();
}

bool check_crc (uint16_t* length)
{
    uint32_t ii;
    uint16_t crc = 0, crcexpected = 0;
    uint8_t byte;

    *length = 0;

    // get length of the application
    eep_read(MOD_eExtEEPAddr_AppSize, MOD_LEN_APPSIZE, (uint8_t*)length);
    if (*length > EEPROM_SIZE) return false;

    // read expected CRC value
    eep_read(MOD_eExtEEPAddr_AppCrc, MOD_LEN_APPCRC, (uint8_t*)&crcexpected);

    // calculate CRC of EEProm content
    crc = CRC_START_VALUE;
    for (ii=0; ii<(*length); ii++) {
        eep_read(MOD_eExtEEPAddr_AppStart + ii, 1, &byte);
        crc = crc_16_next_byte(crc, byte);
    }

    if (crc != crcexpected) return false;

    return true;
}

bool check_controller_id (void)
{
    uint8_t sigbyte, eepcontent = 0;

    eep_read(BLD_eExtEEPAddr_CtrlID + 0, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE0);
    if (sigbyte != eepcontent) return false;

    eep_read(BLD_eExtEEPAddr_CtrlID + 1, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE1);
    if (sigbyte != eepcontent) return false;

    eep_read(BLD_eExtEEPAddr_CtrlID + 2, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE2);
    if (sigbyte != eepcontent) return false;

    return true;
}

bool check_board_type (void)
{
    return true;
}

bool has_app_id_changed (void)
{
    return true;
}

bool has_app_version_changed (void)
{
    return true;
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

    void (*start) ( void ) = (void*) 0x0000; // function pointer to application code

    DBG_INIT();

    // disable all interrupts
    cli();

#ifndef __AVR_ATtiny1634__
    register uint8_t temp, tempSREG;
    // move interrupt vector table to bootloader section
    tempSREG = SREG;
    temp = MCUCR;
    MCUCR = temp | (1 << IVCE);     // needed to unlock IVSEL
    MCUCR = temp | (1 << IVSEL);    // set IVSEL (within 4 cycles)
    SREG = tempSREG;
#endif

    bld_status = 0;

    // initialize external SPI eeprom
    spi_master_init_blk();
    eep_initialize();

    // check internal EEProm if a new application has to be flashed
    bld_status = eeprom_read_byte(&g_reg_internal_eep[MOD_eCfg_BldFlag]);
    bld_status &= ~((1 << eBldFlagCRCMismatch) |
    		        (1 << eBldFlagControllerTypeMismatch) |
                    (1 << eBldFlagBoardTypeMismatch) |
                    (1 << eBldFlagUnused));


    if (bld_status & (1<<eBldFlagAppProgram)) {
        bld_status &= ~(1<<eBldFlagAppProgram);
        DBG_SET_PIN0();
        do {
            // check external EEProm
            if (!check_crc(&length)) {
                bld_status |= (1 << eBldFlagCRCMismatch);
                break;
            }
            DBG_SET_PIN1();

            // check controller
            if (!check_controller_id()) {
                bld_status |= (1 << eBldFlagControllerTypeMismatch);
                DBG_SET_PIN5();
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

            DBG_SET_PIN2();

            // flash new application
            program_flash(0, length);

            DBG_SET_PIN3();

            bld_status |= (1 << eBldFlagNewSWProgrammed);
#ifndef __AVR_ATtiny1634__
            // re-enable RWW-section again. We need this if we want to jump back
            // to the application after bootloading.
            boot_rww_enable ();
#endif
        } while ( false );
        eeprom_write_byte(&g_reg_internal_eep[MOD_eCfg_BldFlag], bld_status);
    }
    DBG_SET_PIN4();

    DBG_ALL_OFF();
    DBG_ALL_ON();
    DBG_ALL_OFF();
    DBG_ALL_ON();
    DBG_ALL_OFF();
    DBG_ALL_ON();
    DBG_ALL_OFF();

    // restore interrupt vector table
    cli();
#ifndef __AVR_ATtiny1634__
    temp = MCUCR;
    MCUCR = temp | (1<<IVCE);
    MCUCR = temp & ~(1<<IVSEL);
#endif
    // start application
    start();
    return 0;
}

/** @} */
