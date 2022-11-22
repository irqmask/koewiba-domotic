/**
 * @addtogroup BLINDCONTROL_5_ATMEGA328
 * @addtogroup APPVERSION
 * @brief Version information of "blindcontrol_5_atmega328p" project.
 *
 * @{
 * @file    appversion.c
 * @brief   Version information of "blindcontrol_5_atmega328p" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
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

#include <avr/io.h>
#include "moddef_common.h"
#include "version.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

//! Application's version information
//! @note The linker sorts the variables upside down. app_controller_id begins at
//!       the lowest address and app_version begins at the highest address.
//! @see BOARD_IDs, APP_IDs, APP_VERSIONING
const unsigned char app_versioninfo[MOD_VERSIONINFO_LEN] __attribute__((section(".versioninfo")))
                        = {SIGNATURE_0,SIGNATURE_1,SIGNATURE_2,0x00,
                           0x00,0x08,   // board ID            (high byte, low byte)
                           2,           // board revision
                           0x00,0x0B,   // application ID      (high byte, low byte)
                           VERSION_MAJOR, VERSION_MINOR, VERSION_BUGFIX, // application version (major, minor, bugfix)
                            (VERSION_HASH & 0x000000FF),        // version hash
                           ((VERSION_HASH & 0x0000FF00) >> 8),
                           ((VERSION_HASH & 0x00FF0000) >> 16),
                           ((VERSION_HASH & 0xFF000000) >> 24)
};

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
