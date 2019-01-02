/**
 * @addtogroup ROOMTHERMOSTAT
 * @addtogroup APPVERSION
 * @brief Version information of "roomthermostat" project.
 *
 * @{
 * @file    appversion.c
 * @brief   Version information of "roomthermostat" project.
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

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// Note: The linker sorts the variables upside down. app_controller_id begins at 
//       the lowest address and app_version begins at the highest address.
const unsigned char app_version[MOD_LEN_APPVER] __attribute__((section(".versioninfo"))) 
                        = {6,0x07};

const unsigned char app_id[MOD_LEN_APPID] __attribute__((section(".versioninfo"))) 
                        = {4,0x05};

const unsigned char app_board_rev[MOD_LEN_BOARDREV] __attribute__((section(".versioninfo"))) 
                        = {3};

const unsigned char app_board_id[MOD_LEN_BOARDID] __attribute__((section(".versioninfo"))) 
                        = {1,0x02};

const unsigned char app_controller_id[MOD_LEN_CONTROLLERID] __attribute__((section(".versioninfo"))) 
                        = {SIGNATURE_0,SIGNATURE_1,SIGNATURE_2,0x00};
                    
// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
