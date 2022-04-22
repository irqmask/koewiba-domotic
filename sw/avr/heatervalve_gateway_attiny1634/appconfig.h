/**
 * @addtogroup HEATERVALVE_GATEWAY_ATTINY1634
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "heatervalve_gateway_attiny1634" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "heatervalve_gateway_attiny1634" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
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

#define BUS_APPCONFIG   1
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_RemTempCurrentModuleID = MOD_eReg_FirstAppSpecific,    //!< ModuleID of sender of current temperature
    APP_eReg_RemTempCurrentRegNo,       //!< Sender's register number of current temperature
    APP_eReg_RemTempSetpointModuleID,   //!< ModuleID of sender of temperature setpoint
    APP_eReg_RemTempSetpointRegNo,      //!< Sender's register number of temperature setpoint

    // insert application specific registers here
    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_RemTempCurrentModuleID = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_RemTempCurrentModuleIDLow = APP_eCfg_RemTempCurrentModuleID,
    APP_eCfg_RemTempCurrentModuleIDHigh,
    APP_eCfg_RemTempCurrentRegNo,
    APP_eCfg_RemTempSetpointModuleID,
    APP_eCfg_RemTempSetpointModuleIDLow = APP_eCfg_RemTempSetpointModuleID,
    APP_eCfg_RemTempSetpointModuleIDHigh,
    APP_eCfg_RemTempSetpointRegNo,

    // insert application specific configuration here
    APP_eCfg_Last
} eConfiguration_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

extern uint16_t app_rem_temp_curr_modid;
extern uint8_t app_rem_temp_curr_regno;
extern uint16_t app_rem_temp_setp_modid;
extern uint8_t app_rem_temp_setp_regno;

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern void app_register_load(void);

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
