/**
 * @addtogroup CRC16
 * @brief 16bit cyclic redundancy check.
 *
 * @{
 * @file    crc16.c
 * @brief   16bit cyclic redundancy check.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

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
