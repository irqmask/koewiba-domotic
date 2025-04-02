/**
 * @addtogroup BUS_GATEWAY
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "testscheduler" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "testscheduler" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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
#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG       1
#define BUS_TX_QUEUE_SIZE   100
#define BUS_MAXSENDMSGLEN   64 + 3   //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 3  //!< Maximum length of a message to be received.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_Input1 = MOD_eReg_FirstAppSpecific, //!< State of Input 1
	APP_eReg_Input2,			//!< State of input 2
	APP_eReg_Input3,			//!< State of input 3
	APP_eReg_Input4,			//!< State of input 4

    // insert application specific registers here
    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_Unused00 = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_Unused00Low = APP_eCfg_Unused00,
    APP_eCfg_Unused00High,
     // insert application specific configuration here
    APP_eCfg_Last
} eConfiguration_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
