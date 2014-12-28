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
#define __25LC256__
#define EEP_CS_PORT  PORTB
#define EEP_CS_DDR   DDRB
#define EEP_CS       PB4
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

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            eep_initialize           (void);

uint8_t         eep_check_statusregister (uint8_t        uFlagMask);

uint16_t        eep_read                 (uint16_t       uEEPAddress,
                                          uint16_t       uCount,
                                          uint8_t*       puBuffer);

uint16_t        eep_write                (uint16_t       uEEPAddress,
                                          uint16_t       uCount,
                                          const uint8_t* puBuffer);

#endif /* _EEPROM_SPI_H_ */
/** @} */
