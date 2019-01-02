/**
 * @addtogroup CRC16
 * @brief 16bit cyclic redundancy check.
 *
 * @{
 * @file    crc16.c
 * @brief   16bit cyclic redundancy check.
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

// --- Include section ---------------------------------------------------------

#include "prjconf.h"
#include "prjtypes.h"

#include "crc16.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifndef PRJCONF_UC_AVR
uint16_t crc_16_next_byte (uint16_t crc, uint8_t a)
{
	int i;

	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
	    if (crc & 1)
		crc = (crc >> 1) ^ 0xA001;
	    else
		crc = (crc >> 1);
	}

	return crc;
}
#endif

/**
 * Calculate CRC16 checksum.
 */
uint16_t crc_calc16(uint8_t* data, uint8_t len)
{
    uint16_t crc = CRC_START_VALUE;

    while (len--) {
    	crc = crc_16_next_byte(crc, *data++);
    }
    return crc;
}

/** @} */
