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
/*
 * Copyright (C) 2019  christian <irqmask@web.de>
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

uint16_t crc_calc16 (const uint8_t* data, uint8_t len);

#endif /* _CRC16_KWB_H_ */
/** @} */
