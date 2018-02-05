/**
 * @addtogroup MODDEF_COMMON
 * @brief Project global layout of the internal configuration storage.
 * The location of all general settings which apply to every module are defined
 * here. Mostly, depending on the processor, this configuration area resides in
 * the controller's internal eeprom.
 *
 * @{
 * @file    moddef_common.h
 * @brief   Project global layout of the internal configuration storage.
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

#ifndef _MODDEF_COMMON_H_
#define _MODDEF_COMMON_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#define MOD_LEN_APPSIZE         2
#define MOD_LEN_APPCRC          2

#define MOD_LEN_CONTROLLERID    4
#define MOD_LEN_BOARDID         2
#define MOD_LEN_BOARDREV        1
#define MOD_LEN_APPID           2
#define MOD_LEN_APPVER          3

//! Application's version items in #app_versioninfo
//! @see BOARD_IDs, APP_IDs, APP_VERSIONING
typedef enum {
    MOD_eVerCtrlSignature0 = 0, //!< 1st byte of controller's device signature.
    MOD_eVerCtrlSignature1,     //!< 2nd byte of controller's device signature.
    MOD_eVerCtrlSignature2,     //!< 3rd byte of controller's device signature.
    MOD_eVerCtrlSignature3,     //!< 4th byte of controller's device signature.
    MOD_eVerBoardIdHigh,        //!< High byte of 16bit board id.
    MOD_eVerBoardIdLow,         //!< Low byte of 16bit board id.
    MOD_eVerBoardRev,           //!< Board revision.
    MOD_eVerAppIdHigh,          //!< High byte of 16bit application id.
    MOD_eVerAppIdLow,           //!< Low byte of 16bit application id.
    MOD_eVerAppMajor,           //!< Major application's version number.
    MOD_eVerAppMinor,           //!< Minor application's version number.
    MOD_eVerAppBugfix           //!< Bugfix application's version number.
} MOD_eVersionItems;

/**
 * @defgroup BOARD_IDs Board IDs
 * @{
 * The Name of the board corresponds to a PCB in the hardware section.
 * The Board Revision 1-26 corresponds to Rev A - Rev Z
 * The Board IDs are filled ascending whenever new boards occur. The board must
 * already exist to get a board ID and therefore listed here.
 * The following board IDs are defined yet:
 * 0x0001 bus-module_atmega324  Basis board
 * 0x0002 bus-module_atmega328
 * 0x0003 bus-module_attiny1634
 * 0x0004 gateway_usb
 * 0x0005 motor_switch_3pole
 * 0x000x (not available yet - room-thermostat)
 * 0x000x (not available yet - heater-control)
 * 0x000x (not available yet - 8keys)
 * 0x000x (not available yet - io88)
 * ...
 * 0xEFFF (last common board)
 * 0xF000 (first user specific board)
 * 0xFFFE elektor_bus-node_atmega88 (only for testing, since this software is
 *                                   not productively usable)(last user specific board)
 * 0xFFFF (empty flash: board ID not defined)
 * }
 */

/**
 * @defgroup APP_IDs Application software IDs
 * @{
 * The following application IDs are defined yet:
 * 0x0001 scheduler_attiny1634
 * 0x0002 gateway232_atmega324(p)
 * 0x0003 basic_app_atmega324(p)
 * 0x0004 basic_app_atmega328(p)
 * 0x0005 basic_app_attiny1634
 * 0x0006 blind-control_atmega328(p)
 * ...
 * 0xEFFF (last common application)
 * 0xF000 (first user specific application)
 * 0xFFFE elektor_bus-node_atmega88 (only for testing, since this software is
 *                                   not productively usable)
 * 0xFFFF (empty flash: application ID not defined)
 * }
 */

/**
 * @defgroup APP_VERSIONING Application versioning
 * {
 * The application version consists of major, minor and bugfix number (M,m,b).
 * Each of it is 8bit wide. Versions starting with 0.x.x are considered as
 * pre-production development versions used to give already the developer the
 * possibility to identify versions in an early stage of development.
 *
 * The major version number is changed, when a major change happened. This could
 * be a change in the interface, other design philosophy e.g. touch instead of
 * buttons, another bus command interface, which is not backwards compatible.
 *
 * The minor version number is changed when features are added, but the
 * application remains backwards compatible in terms of interfaces. E.g.
 * additional bus commands are supported, a newer hardware with new features is
 * supported (e.g. more IOs) but the software runs also without changes in the
 * old hardware.
 *
 * The bugfix version is increased, whenever a bug is fixed ;-) or minor features
 * are added.
 * }
 */

// --- Type definitions --------------------------------------------------------

//! Common register storage layout which is used by all applications.
typedef enum {
    MOD_eReg_Undefined,
    MOD_eReg_ModuleID,                  //!< Module ID as it appears on the bus.
    MOD_eReg_BldFlag,                   //!< Storage for bootloader flags for
                                        //!< next reboot will be reset by the
                                        //!< bootloader.

    MOD_eReg_FirstAppSpecific   = 16,
    MOD_eReg_LastAppSpecific    = 229,
    MOD_eReg_DeviceSignature0   = 230,  //!< 1st byte of device signature.
    MOD_eReg_DeviceSignature1   = 231,  //!< 2nd byte of device signature.
    MOD_eReg_DeviceSignature2   = 232,  //!< 3rd byte of device signature.
    MOD_eReg_DeviceSignature3   = 233,  //!< 4th byte of device signature.
    MOD_eReg_BoardID            = 234,  //!< Needed Board ID.
    MOD_eReg_BoardRev           = 235,  //!< Needed Board revision (at least)
    MOD_eReg_AppID              = 236,  //!< Application ID.
    MOD_eReg_AppVersionMajor    = 237,  //!< Application version major.
    MOD_eReg_AppVersionMinor    = 238,  //!< Application version minor.
    MOD_eReg_AppVersionBugfix   = 239   //!< Application version bug-fix.
} MOD_eRegCommon_t;

//! Common configuration storage layout which is used by all applications.
//! Mostly, depending on the processor, this configuration area resides in the
//! controller's internal eeprom.
typedef enum {
    MOD_eCfg_ModuleID,                  //!< Module ID as it appears on the bus.
    MOD_eCfg_ModuleIDLo = MOD_eCfg_ModuleID,  //!< Module ID low-byte as it
                                        //!< appears on the bus.
    MOD_eCfg_ModuleIDHigh,              //!< Module high-byte ID as it appears
                                        //!< on the bus.
    MOD_eCfg_BldFlag,                   //!< Storage for bootloader flags for
                                        //!< next reboot will be reset by the
                                        //!< bootloader.
    MOD_eCfg_FirstAppSpecific   = 16,
} MOD_eCfgCommon_t;

//! Layout of external EEProm
typedef enum {
    MOD_eExtEEPAddr_AppSize     = 0,
    MOD_eExtEEPAddr_AppCrc      = MOD_eExtEEPAddr_AppSize + MOD_LEN_APPSIZE,
    MOD_eExtEEPAddr_AppStart    = MOD_eExtEEPAddr_AppCrc + MOD_LEN_APPCRC,
} MOD_eExtEEPAddr_t;

//! Flags of bootloader as they appear in #MOD_eCfg_BldFlag.
typedef enum {
    eBldFlagNewSWProgrammed,
    eBldFlagCRCMismatch,
    eBldFlagControllerTypeMismatch,
    eBldFlagBoardTypeMismatch,
    eBldFlagAppIDChanged,
    eBldFlagAppVersionChanged,
    eBldFlagUnused,
    eBldFlagAppProgram
} eBldStatusFlags_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _MODDEF_COMMON_H_ */
/** @} */
