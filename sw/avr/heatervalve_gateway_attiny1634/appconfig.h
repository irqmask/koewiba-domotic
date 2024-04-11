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

#define BUS_APPCONFIG           1
#undef BUS_SCHEDULER                    //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE       100     //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN       32      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN       64 + 2  //!< Maximum length of a message to be received.

#define UART_BAUDRATE           9600    //!< Baudrate for second UART interfacing heatervalve module.
#define APP_HAS_BLOCK_HANDLER   1       //!< Application has block start and block end handler.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_IntTempCurrent = MOD_eReg_FirstAppSpecific, //!< Internal temperature sensor measurement in 1/100 Kelvin
    APP_eReg_ResTempCurrent,			//!< Resulting temperature in 1/100 Kelvin. Depends on TemperatureMode
    APP_eReg_IntTempOffset,				//!< Offset to calibrate internal temperature sensor in 1/100 Kelvin
    APP_eReg_TempMode,           		//!< Temperature measurement mode (0=internal, 1=external, 2=average int+ext)
    APP_eReg_reserved020,
    APP_eReg_reserved021,
    APP_eReg_reserved022,
    APP_eReg_reserved023,
    APP_eReg_reserved024,
    APP_eReg_reserved025,
    APP_eReg_reserved026,
    APP_eReg_reserved027,
    APP_eReg_reserved028,
    APP_eReg_reserved029,
    APP_eReg_reserved030,
    APP_eReg_reserved031,
    APP_eReg_RemTempCurrentModuleID, 	//!< ModuleID of sender of current temperature
    APP_eReg_RemTempCurrentRegNo,       //!< Sender's register number of current temperature
    APP_eReg_RemTempSetpointModuleID,   //!< ModuleID of sender of temperature setpoint
    APP_eReg_RemTempSetpointRegNo,      //!< Sender's register number of temperature setpoint
    APP_eReg_DbgRecvModuleID,           //!< Receiver module ID for debug messages
    APP_eReg_reserved037,
    APP_eReg_Kp,                        //!< Kp regulation parameter as 8bit fix-point number. 256 = 1.0
    APP_eReg_reserved038,
    APP_eReg_reserved039,
    APP_eReg_reserved040,
    APP_eReg_reserved041,
    APP_eReg_reserved042,
    APP_eReg_reserved043,
    APP_eReg_reserved044,
    APP_eReg_reserved045,
    APP_eReg_reserved046,
    APP_eReg_reserved047,
    APP_eReg_Contact0State,
    APP_eReg_Contact1State,
    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_IntTempOffset = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_IntTempOffsetLow = APP_eCfg_IntTempOffset,
    APP_eCfg_IntTempOffsetHigh,
    APP_eCfg_TempMode,
    APP_eCfg_RemTempCurrentModuleID,
    APP_eCfg_RemTempCurrentModuleIDLow = APP_eCfg_RemTempCurrentModuleID,
    APP_eCfg_RemTempCurrentModuleIDHigh,
    APP_eCfg_RemTempCurrentRegNo,
    APP_eCfg_RemTempSetpointModuleID,
    APP_eCfg_RemTempSetpointModuleIDLow = APP_eCfg_RemTempSetpointModuleID,
    APP_eCfg_RemTempSetpointModuleIDHigh,
    APP_eCfg_RemTempSetpointRegNo,
    APP_eCfg_DebugReceiverModuleID,
    APP_eCfg_DebugReceiverModuleIDLow = APP_eCfg_DebugReceiverModuleID,
    APP_eCfg_DebugReceiverModuleIDHigh,
    APP_eCfg_Kp,
    APP_eCfg_KpLow = APP_eCfg_Kp,
    APP_eCfg_KpHigh,

    // insert application specific configuration here
    APP_eCfg_Last
} eConfiguration_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

extern bool app_block_start(uint16_t sender, uint8_t blocktype);
extern bool app_block_end(uint16_t sender, uint8_t blocktype);

extern uint16_t app_rem_temp_curr_modid;
extern uint8_t app_rem_temp_curr_regno;
extern uint16_t app_rem_temp_setp_modid;
extern uint8_t app_rem_temp_setp_regno;
extern uint16_t app_debug_receiver;
extern uint16_t app_regulation_kp;

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern void app_register_load(void);

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
