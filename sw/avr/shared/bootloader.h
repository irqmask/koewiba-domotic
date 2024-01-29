/**
 * @addtogroup BOOTLOADER
 *
 * @{
 * @file    bootloader.h
 * @brief   Common Bootloader defines and types.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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
#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include <avr/eeprom.h>
// include
#include "prjtypes.h"
#include "moddef_common.h"
// avr/shared
#include "bootloader.h"

// --- Definitions -------------------------------------------------------------

#ifdef __AVR_ATtiny1634__
// APP_ENTRYPOINT not defined on purpose for ATtiny1634
// bootloader is called from mainapp
#define BLD_ENTRYPOINT 0x3800   /// Entry point of bootloader
#else
#define APP_ENTRYPOINT 0x0000   /// Entry point of application
// BLD_ENTRYPOINT not defined on purpose for ATtiny1634
#endif

#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PB__)
 #define STARTADDR_VERSIONINFO  0x0068
#elif defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__) || defined (__AVR_ATmega324PA__)
 #define STARTADDR_VERSIONINFO  0x007C
#elif defined (__AVR_ATtiny1634__)
 #define STARTADDR_VERSIONINFO  0x0070
#endif

// --- Type definitions --------------------------------------------------------

typedef enum {
	BLD_eExtEEPAddr_CtrlID      = MOD_eExtEEPAddr_AppStart + STARTADDR_VERSIONINFO,
    BLD_eExtEEPAddr_BoardID     = BLD_eExtEEPAddr_CtrlID + MOD_LEN_CONTROLLERID,
    BLD_eExtEEPAddr_BoardRev    = BLD_eExtEEPAddr_BoardID + MOD_LEN_BOARDID,
    BLD_eExtEEPAddr_AppID       = BLD_eExtEEPAddr_BoardRev + MOD_LEN_BOARDREV,
    BLD_eExtEEPAddr_AppVer      = BLD_eExtEEPAddr_AppID + MOD_LEN_APPID,
} bld_ext_eep_addr_t;

// --- Local variables ---------------------------------------------------------

//! Needed to get structured access to the internal eeprom.
extern uint8_t g_reg_internal_eep[MOD_eCfg_FirstAppSpecific] EEMEM;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifndef __AVR_ATtiny1634__
void bld_entrypoint(void);
#endif

#endif
/** @} */
