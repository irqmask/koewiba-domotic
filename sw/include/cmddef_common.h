/**
 * @addtogroup CMDDEF_COMMON
 * @brief List of common command bytes.
 *
 * @{
 * @file    cmddef_common.h
 * @brief   List of common command bytes.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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

#ifndef _CMDDEF_COMMON_H_
#define _CMDDEF_COMMON_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! Common defined commands.
typedef enum {
    eCMD_STATE_TYPELESS         = 0x00, //!< Send typeless state data.
    eCMD_STATE_BITFIELDS        = 0x01, //!< Send bitfield state data.
    eCMD_STATE_8BIT             = 0x02, //!< Send 8bit state data.
    eCMD_STATE_16BIT            = 0x03, //!< Send 16bit state data.
    eCMD_STATE_32BIT            = 0x04, //!< Send 32bit state data.
    eCMD_STATE_DATE_TIME        = 0x05, //!< Send date/time data.

    eCMD_REQUEST_REG            = 0x10, //!< Request register value.
    eCMD_SET_REG_8BIT           = 0x12, //!< Set 8bit register value.
    eCMD_SET_REG_16BIT          = 0x13, //!< Set 16bit register value.
    eCMD_SET_REG_32BIT          = 0x14, //!< Set 32bit register value.

    eCMD_BLOCK_START            = 0x20,
    eCMD_BLOCK_DATA             = 0x21,
    eCMD_BLOCK_END              = 0x22,
    eCMD_BLOCK_INFO             = 0x23,
    eCMD_BLOCK_RESET            = 0x24,
    
    eCMD_FIRST_APP_SPECIFIC     = 0x40, //!< First application specific command.

    eCMD_ACK                    = 0xF0, //!< Acknowledge message.
    eCMD_NAK                    = 0xF2, //!< Not-Acknowledge message.
    eCMD_SLEEP                  = 0xF5, //!< Send module into sleep mode.
    eCMD_RESET                  = 0xFE, //!< Reset module.
} cmd_common_t;

//! destinations for block data.
typedef enum {
    eSTORAGE_NONE           = 0,
    eSTORAGE_EEPROM_INT     = 1,
    eSTORAGE_EEPROM_EXT     = 2,
    eSTORAGE_DISPLAY        = 3,    // data for display memory
    eSTORAGE_SCOMM          = 4     // data stream output to serial interface
} blkdata_destination_t;


// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _CMDDEF_COMMON_H_ */
/** @} */
