/**
 * @addtogroup CRC16
 * @brief 16bit cyclic redundancy check.
 *
 * @{
 * @file    crc16.h
 * @brief   16bit cyclic redundancy check.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _CRC16_KWB_H_
#define _CRC16_KWB_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"
#ifdef PRJCONF_UC_AVR
#include <util/crc16.h>
#endif
#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define CRC_START_VALUE 0xFFFF

#ifdef PRJCONF_UC_AVR
#define crc_16_next_byte _crc16_update
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifndef PRJCONF_UC_AVR
uint16_t crc_16_next_byte (uint16_t old_crc, uint8_t new_byte);
#endif

uint16_t crc_calc16 (uint8_t* data, uint8_t len);

#endif /* _CRC16_KWB_H_ */
/** @} */
