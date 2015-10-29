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

#ifndef _CRC16_H_
#define _CRC16_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define CRC_START_VALUE 0

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

uint16_t crc_16_start (void);

uint16_t crc_16_next_byte (uint16_t old_crc, uint8_t new_byte);

uint16_t crc_calc16 (uint8_t* data, uint8_t len);

#endif /* _CRC16_H_ */
/** @} */
