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

#define EEP_CS_PORT  PORTB
#define EEP_CS_DDR   DDRB
#define EEP_CS       PB4

#define EEP_SPI_CS_ACTIVATE   EEP_CS_PORT &= ~(1<<EEP_CS)
#define EEP_SPI_CS_DEACTIVATE EEP_CS_PORT |=  (1<<EEP_CS)

// --- Type definitions --------------------------------------------------------

//! Flags of the EEProm's status register
enum eEEPFlags {
    eEEP_WIP    = 0x01, //!< Write-In-Process
    eEEP_WEL    = 0x02, //!< Write-Enable-Latch
    eEEP_BP0    = 0x04, //!< Block-Protection 0
    eEEP_BP1    = 0x08, //!< Block-Protection 1
    eEEP_WPEN   = 0x80  //!< Write-Protection-Enabled
} eEEPFlags_t;

//! Commands of the EEPRom
enum eEEPCommands {
    eEEP_WRSR   = 0x01, //!< Write STATUS register
    eEEP_WRITE  = 0x02, //!< Write data to memory array beginning at selected
                        //!< address.
    eEEP_READ   = 0x03, //!< Read data from memory array beginning at selected
                        //!< address.
    eEEP_WRDI   = 0x04, //!< Reset the write enable latch (disable write
                        //!< operations).
    eEEP_RDSR   = 0x05, //!< Read STATUS register
    eEEP_WREN   = 0x06  //!< Set the write enable latch (enable write operations)
} eEEPCommands_t;

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
void            EEP_vInit           (void)
{
    EEP_CS_DDR |= (1<<EEP_CS);
    EEP_SPI_CS_DEACTIVATE;
}

/**
 * Returns the masked Flag-Bit(s) if uFlagMask is not 0. Otherwise the whole status
 * register will be returned.
 */
uint8_t         EEP_uCheckStatusReg (uint8_t                uFlagMask)
{
    uint8_t retval;

    EEP_SPI_CS_ACTIVATE;
    SPI_uTransmitBlk(eEEP_RDSR);
    SPI_uTransmitBlk(0);
    if ( uFlagMask )
        retval = (uFlagMask & SPDR);
    else
        retval = SPDR;
    EEP_SPI_CS_DEACTIVATE;
    return retval;
}

/**
 * Reads uCount bytes from EEPROM beginning at address uEEPAddress into
 * puBuffer and returns the number of data, that was read.
 */
uint16_t        EEP_uRead           (uint16_t               uEEPAddress,
                                     uint16_t               uCount,
                                     uint8_t*               puBuffer)
{
    uint16_t read = 0;
    uint8_t addressH = (0xFF00 & uEEPAddress) >> 8;
    uint8_t addressL = (0x00FF & uEEPAddress);

    if ( uEEPAddress + uCount > EEPROM_SIZE )
        return 0;

    while (EEP_uCheckStatusReg(eEEP_WIP) != 0);

    EEP_SPI_CS_ACTIVATE;
    SPI_uTransmitBlk(eEEP_READ);
    SPI_uTransmitBlk(addressH);
    SPI_uTransmitBlk(addressL);
    for ( read = 0; read < uCount; read++ ) {
        SPI_uTransmitBlk(0);
        puBuffer[read] = SPDR;
    }
    EEP_SPI_CS_DEACTIVATE;
    return read;
}

/**
 * Writes uCount bytes from puBuffer into EEPROM beginning at address
 * uEEPAddress and returns the number of data, that was written.
 */
uint16_t        EEP_uWrite          (uint16_t               uEEPAddress,
                                     uint16_t               uCount,
                                     const uint8_t*         puBuffer)
{
    uint8_t attempt = 3;
    uint16_t pos = 0, byteCnt = uCount;

    if ( uEEPAddress + uCount > EEPROM_SIZE )
        return 0;

    while ( 0 < attempt-- ) {
        uint8_t addressH = (0xFF00 & uEEPAddress) >> 8;
        uint8_t addressL = (0x00FF & uEEPAddress);

        while ( EEP_uCheckStatusReg(eEEP_WIP) != 0);

        EEP_SPI_CS_ACTIVATE;
        SPI_uTransmitBlk(eEEP_WREN);
        EEP_SPI_CS_DEACTIVATE;

        if ( EEP_uCheckStatusReg(eEEP_WEL) ) {
            EEP_SPI_CS_ACTIVATE;
            attempt = 0;
            SPI_uTransmitBlk(eEEP_WRITE);
            SPI_uTransmitBlk(addressH);
            SPI_uTransmitBlk(addressL);
            while ( byteCnt-- ) {
                SPI_uTransmitBlk(puBuffer[pos++]);
                uEEPAddress++;
                // Page-Check:
                if ( (0 == uEEPAddress % EEPROM_PAGESIZE) && (0 < byteCnt) ) {
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
