/**
 * @addtogroup CRC16
 * @brief 16bit cyclic redundancy check.
 *
 * TODO: Detailed description of module.
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

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

uint16_t CRC_uCalc16(uint8_t* puData, uint8_t uLen);

#endif /* _CRC16_H_ */
/** @} */
