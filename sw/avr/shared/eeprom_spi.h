/**
 * @addtogroup EEPROM_SPI
 * @brief Functions for external SPI driven EEProms.
 *
 * Initialize, read and write to external EEProm, which are connected via SPI.
 *
 * @{
 * @file    eeprom_spi.h
 * @brief   Functions for SPI driven EEProms.
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
#ifndef _EEPROM_SPI_H_
#define _EEPROM_SPI_H_

// --- Include section ---------------------------------------------------------

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

#include "prjtypes.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

#ifndef EEPROM_PCB_CONFIG
 #define EEPROM_PCB_CONFIG  1
 #define __25LC256__
 #define EEP_CS_PORT        PORTB
 #define EEP_CS_DDR         DDRB
 #define EEP_CS             PB4
#endif

#if defined (__25LC640__)
#define EEPROM_PAGESIZE   32
#define EEPROM_SIZE       0x2000
#elif defined (__25LC128__)
#define EEPROM_PAGESIZE   64
#define EEPROM_SIZE       0x4000
#elif defined (__25LC256__)
#define EEPROM_PAGESIZE   64
#define EEPROM_SIZE       0x8000
#elif defined (__25LC512__)
#define EEPROM_PAGESIZE   128
#define EEPROM_SIZE       0x10000
#endif
#define EEPROM_MAXADDRESS EEPROM_SIZE-1

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

void            eep_initialize           (void);

uint8_t         eep_check_statusregister (uint8_t        flag_mask);

uint16_t        eep_read                 (uint16_t       eep_address,
                                          uint16_t       count,
                                          uint8_t*       buffer);

uint16_t        eep_write                (uint16_t       eep_address,
                                          uint16_t       count,
                                          const uint8_t* buffer);

#endif /* _EEPROM_SPI_H_ */
/** @} */
