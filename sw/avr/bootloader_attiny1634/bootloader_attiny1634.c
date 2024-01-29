/**
 * @addtogroup BOOTLOADER
 *
 * @{
 * @file    bootloader_attiny1634.c
 * @brief   Bootloader entry point for attiny1634.
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
#include "led_debug.h"
#include "spi.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

#ifdef DEBUG_LED_OUTPUT
 #include <util/delay.h>
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void microdelay(uint8_t loops)
{
    while (loops--) {
        asm("nop");
        asm("nop");
        asm("nop");
    }
}

#ifdef USE_DEBUG_LEDS
static void millidelay(uint8_t loops)
{
    while (loops--) {
        microdelay(100);
        microdelay(100);
        microdelay(100);
        microdelay(100);
        microdelay(100);
        microdelay(100);
        microdelay(100);
        microdelay(100);
        microdelay(100);
        microdelay(100);
    }
}

static void led_blink_debug(uint8_t pulses)
{
    while (pulses--) {
        LED_STATUS_ON;
        millidelay(250);
        LED_STATUS_OFF;
        millidelay(250);
    }
    millidelay(255);
    millidelay(255);
}
#else
 #define led_blink_debug(x)
#endif

/**
 * Read a byte from internal EEProm
 * @param[in] address within internal EEProm
 * @returns read data from EEProm
 */
static uint8_t bld_int_eep_read(uint16_t address)
{
    while (EECR & (1<<EEPE));
    EEAR = address;
    EECR |= (1<<EERE);
    return EEDR;
}

/**
 * Write a byte into internal EEProm
 * @param[in]   address Address within internal EEProm
 * @param[in]   value   Data byte to write
 */
static void bld_int_eep_write(uint16_t address, uint8_t value)
{
    while (EECR & (1<<EEPE));
    // set programming mode
    EECR = (0<<EEPM1) | (0<<EEPM0);
    EEAR = address;
    EEDR = value;
    EECR |= (1<<EEMPE);
    EECR |= (1<<EEPE);
}

/**
 * Initialize the EEProm.
 * Configure chip select pin and deactivate chip select.
 */
static void bld_ext_eep_initialize (void)
{
    EEP_CS_DDR |= (1 << EEP_CS);
    EEP_SPI_CS_DEACTIVATE;
}

/**
 * Returns the masked Flag-Bit(s) if flag_mask is not 0. Otherwise the whole status
 * register will be returned.
 *
 * @param[in] flag_mask Mask of flags to be checked
 * @returns Checked flags.
 */
static uint8_t bld_ext_eep_check_statusregister (uint8_t flag_mask)
{
    uint8_t retval;

    EEP_SPI_CS_ACTIVATE;
    spi_transmit_blk(eEEP_RDSR);
    retval = spi_transmit_blk(0);
    if (flag_mask) {
        retval &= flag_mask;
    }
    EEP_SPI_CS_DEACTIVATE;
    return retval;
}

/**
 * Reads count bytes from EEPROM beginning at address eep_address into
 * buffer and returns the number of data, that was read.
 *
 * @param[in]     eep_address   Address in EEProm to begin reading.
 * @param[in]     count         Number of bytes to read.
 * @param[out]    buffer        Pointer to transmit buffer.
 * @returns     Number of read bytes.
 */
static uint16_t bld_ext_eep_read (uint16_t eep_address, uint16_t count, uint8_t* buffer)
{
    uint16_t read = 0;
    uint8_t addressH = (eep_address >> 8);
    uint8_t addressL = (eep_address & 0x00FF);

    if (eep_address + count > EEPROM_SIZE) return 0;

    uint8_t timeout = 0xFF;
    while (bld_ext_eep_check_statusregister(eEEP_WIP) != 0 && (timeout-- > 0)) microdelay(5);
    if (timeout == 0) return 0;

    EEP_SPI_CS_ACTIVATE;
    spi_transmit_blk(eEEP_READ);
    spi_transmit_blk(addressH);
    spi_transmit_blk(addressL);
    for (read = 0; read < count; read++) {
        buffer[read] = spi_transmit_blk(0);
    }
    EEP_SPI_CS_DEACTIVATE;
    return read;
}

// the following "functions" are inline-assembly makros and don't need to be
// moved into the bootloader section explicitely, since they will be
// automatically, when called from function already in the section.
// * boot_signature_byte_get()
// * boot_spm_busy_wait(), boot_page_erase(), boot_page_fill(), boot_page_write()
// * eeprom_busy_wait(),
static bool check_crc (uint16_t* length)
{
    uint32_t ii;
    uint16_t crc = 0, crcexpected = 0;
    uint8_t byte;

    *length = 0;

    // get length of the application
    bld_ext_eep_read(MOD_eExtEEPAddr_AppSize, MOD_LEN_APPSIZE, (uint8_t*)length);
    if (*length > EEPROM_SIZE) return false;

    // read expected CRC value
    bld_ext_eep_read(MOD_eExtEEPAddr_AppCrc, MOD_LEN_APPCRC, (uint8_t*)&crcexpected);

    // calculate CRC of EEProm content
    crc = CRC_START_VALUE;
    for (ii=0; ii<(*length); ii++) {
        bld_ext_eep_read(MOD_eExtEEPAddr_AppStart + ii, 1, &byte);
        crc = crc_16_next_byte(crc, byte);
    }

    if (crc != crcexpected) return false;

    return true;
}

static bool check_controller_id (void)
{
    uint8_t sigbyte, eepcontent = 0;

    bld_ext_eep_read(BLD_eExtEEPAddr_CtrlID + 0, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE0);
    if (sigbyte != eepcontent) return false;

    bld_ext_eep_read(BLD_eExtEEPAddr_CtrlID + 1, 1, &eepcontent);
    sigbyte = boot_signature_byte_get(ADDR_SIGNATURE_BYTE1);
    if (sigbyte != eepcontent) return false;

    bld_ext_eep_read(BLD_eExtEEPAddr_CtrlID + 2, 1, &eepcontent);
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
    uint16_t page, ii, ww;
    uint8_t temp;

    // wait until eventually running write cycles of internal eeprom are
    // finished.
    eeprom_busy_wait ();
    page = address / SPM_PAGESIZE;
    // erase first page since it is not neccessarily the first of four pages
    boot_page_erase (address);
    boot_spm_busy_wait ();
    do {
        // erase flash page and wait until the memory is erased.
        // ATtiny1634 has a 4page erase command, meaning always four pages are erased at once.
        // Thus we must only erase every fourth page.
        if ((page % 4 ) == 0) {
            boot_page_erase (address);
            boot_spm_busy_wait ();
        }

        // only copy full 16bit words to flash memory
        if (length % 2) length--;

        for (ii=0; ii<SPM_PAGESIZE; ii+=2) {
            // read a word and convert to litte endian
            bld_ext_eep_read(MOD_eExtEEPAddr_AppStart + address + ii, 1, &temp);
            ww = temp;
            bld_ext_eep_read(MOD_eExtEEPAddr_AppStart + address + ii + 1, 1, &temp);
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
        page++;
    } while (length > 0 && address < BLD_ENTRYPOINT);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void bld_entry_point(void) __attribute__((section(".bootld_attiny1634_entry")));
void bld_entry_point(void)
{
	uint8_t     bld_status;
    uint16_t    length = 0;

    // disable all interrupts
    cli();

    MCUSR = 0;
    wdt_disable();

#ifdef USE_DEBUG_LEDS
    LED_STATUS_DDR |= (1<<LED_STATUS);
    LED_ERROR_DDR |= (1<<LED_ERROR);
    LED_STATUS_OFF;
    LED_ERROR_OFF;
#endif

    bld_status = 0;

    // initialize external SPI eeprom
    spi_master_init_blk();
    bld_ext_eep_initialize();

    // check internal EEProm if a new application has to be flashed
    bld_status = bld_int_eep_read(MOD_eCfg_BldFlag);
    bld_status &= ~((1 << eBldFlagCRCMismatch) |
    		        (1 << eBldFlagControllerTypeMismatch) |
                    (1 << eBldFlagBoardTypeMismatch) |
                    (1 << eBldFlagUnused));
    led_blink_debug(1);

    if (bld_status & (1<<eBldFlagAppProgram)) {
        bld_status &= ~(1<<eBldFlagAppProgram);
        led_blink_debug(2);
        do {
            // check external EEProm
            if (!check_crc(&length)) {
                bld_status |= (1 << eBldFlagCRCMismatch);
                led_blink_debug(4);
                break;
            }

            // check controller
            if (!check_controller_id()) {
                bld_status |= (1 << eBldFlagControllerTypeMismatch);
                led_blink_debug(5);
                break;
            }

            // check board type
            if (!check_board_type()) {
                bld_status |= (1 << eBldFlagBoardTypeMismatch);
                led_blink_debug(6);
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

            led_blink_debug(3);

            bld_status |= (1 << eBldFlagNewSWProgrammed);
        } while ( false );
        bld_int_eep_write(MOD_eCfg_BldFlag, bld_status);
        wdt_enable(WDTO_60MS);
        while(1) {
            led_blink_debug(4);
        }
    }
    led_blink_debug(1);
}

/** @} */
