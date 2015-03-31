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

 // --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include "eeprom_spi.h"
#include "prjtypes.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

#define EEP_SPI_CS_ACTIVATE   EEP_CS_PORT &= ~(1<<EEP_CS)
#define EEP_SPI_CS_DEACTIVATE EEP_CS_PORT |=  (1<<EEP_CS)

// --- Type definitions --------------------------------------------------------

//! Flags of the EEProm's status register
enum eep_flags {
    eEEP_WIP    = 0x01, //!< Write-In-Process
    eEEP_WEL    = 0x02, //!< Write-Enable-Latch
    eEEP_BP0    = 0x04, //!< Block-Protection 0
    eEEP_BP1    = 0x08, //!< Block-Protection 1
    eEEP_WPEN   = 0x80  //!< Write-Protection-Enabled
} eep_flags_t;

//! Commands of the EEPRom
enum eep_commands {
    eEEP_WRSR   = 0x01, //!< Write STATUS register
    eEEP_WRITE  = 0x02, //!< Write data to memory array beginning at selected
                        //!< address.
    eEEP_READ   = 0x03, //!< Read data from memory array beginning at selected
                        //!< address.
    eEEP_WRDI   = 0x04, //!< Reset the write enable latch (disable write
                        //!< operations).
    eEEP_RDSR   = 0x05, //!< Read STATUS register
    eEEP_WREN   = 0x06  //!< Set the write enable latch (enable write operations)
} eep_commands_t;

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
    spi0_transmit_blk(eEEP_RDSR);
    spi0_transmit_blk(0);
    if (flag_mask) retval = (flag_mask & SPI0_DATA_REG);
    else retval = SPI0_DATA_REG;
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
    uint8_t addressH = (0xFF00 & eep_address) >> 8;
    uint8_t addressL = (0x00FF & eep_address);

    if (eep_address + count > EEPROM_SIZE) return 0;

    while (eep_check_statusregister(eEEP_WIP) != 0);

    EEP_SPI_CS_ACTIVATE;
    spi0_transmit_blk(eEEP_READ);
    spi0_transmit_blk(addressH);
    spi0_transmit_blk(addressL);
    for (read = 0; read < count; read++) {
        spi0_transmit_blk(0);
        buffer[read] = SPI0_DATA_REG;
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

        while (eep_check_statusregister(eEEP_WIP) != 0);

        EEP_SPI_CS_ACTIVATE;
        spi0_transmit_blk(eEEP_WREN);
        EEP_SPI_CS_DEACTIVATE;

        if (eep_check_statusregister(eEEP_WEL)) {
            EEP_SPI_CS_ACTIVATE;
            attempt = 0;
            spi0_transmit_blk(eEEP_WRITE);
            spi0_transmit_blk(addressH);
            spi0_transmit_blk(addressL);
            while (byteCnt--) {
                spi0_transmit_blk(buffer[pos++]);
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
