/**
 * @addtogroup EEPROM_SPI
 * @brief Functions for external SPI driven EEProms.
 *
 * Initialize, read and write to external EEProm, which are connected via SPI.
 *
 * @{
 * @file    eeprom_spi.c
 * @brief   Functions for external SPI driven EEProms.
 *
 * @author  Robert Müller
 * @author  Christian Verhalen
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
 // --- Include section ---------------------------------------------------------

#include "eeprom_spi.h"

#include <avr/io.h>
#include <util/delay.h>

#include "prjtypes.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize the EEProm.
 * Configure chip select pin and deactivate chip select.
 */
void eep_initialize (void)
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
uint8_t eep_check_statusregister (uint8_t flag_mask)
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
uint16_t eep_read (uint16_t eep_address, uint16_t count, uint8_t* buffer)
{
    uint16_t read = 0;
    uint8_t addressH = (eep_address >> 8);
    uint8_t addressL = (eep_address & 0x00FF);

    if (eep_address + count > EEPROM_SIZE) return 0;

    uint8_t timeout = 0xFF;
    while (eep_check_statusregister(eEEP_WIP) != 0 && (timeout-- > 0))  _delay_us(5);
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

/**
 * Writes count bytes from buffer into EEPROM beginning at address
 * eep_address and returns the number of data, that was written.
 * 
 * @param[in]     eep_address   Address in EEProm to begin reading.
 * @param[in]     count         Number of bytes to transmit.
 * @param[in]     buffer        Pointer to transmit buffer.
 * @returns     Number of transmitted bytes.
 */
uint16_t eep_write (uint16_t eep_address, uint16_t count, const uint8_t* buffer)
{
    uint8_t attempt = 3;
    uint16_t pos = 0, byteCnt = count;

    if (eep_address + count > EEPROM_SIZE) return 0;

    while (0 < attempt--) {
        uint8_t addressH = (0xFF00 & eep_address) >> 8;
        uint8_t addressL = (0x00FF & eep_address);

        uint8_t timeout = 0xFF;
        while (eep_check_statusregister(eEEP_WIP) != 0 && (timeout-- > 0)) _delay_us(5);
        if (timeout == 0) continue;

        EEP_SPI_CS_ACTIVATE;
        spi_transmit_blk(eEEP_WREN);
        EEP_SPI_CS_DEACTIVATE;

        if (eep_check_statusregister(eEEP_WEL)) {
            EEP_SPI_CS_ACTIVATE;
            attempt = 0;
            spi_transmit_blk(eEEP_WRITE);
            spi_transmit_blk(addressH);
            spi_transmit_blk(addressL);
            while (byteCnt--) {
                spi_transmit_blk(buffer[pos++]);
                eep_address++;
                // Page-Check:
                if ((0 == eep_address % EEPROM_PAGESIZE) && (0 < byteCnt)) {
                    attempt = 3;
                    break;
                }
            }
            EEP_SPI_CS_DEACTIVATE;
        }
    }
    return pos;
}

/** @} */
