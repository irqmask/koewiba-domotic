/**
 * @addtogroup ROOMTHERMOSTAT
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "roomthermostat" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "roomthermostat" project.
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
#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG      1
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    APP_eReg_DesiredTempDay1 = MOD_eReg_FirstAppSpecific, //!< Register mapping: remote address 0 (16bit)
    APP_eReg_DesiredTempNight1,
    APP_eReg_DesiredTempDay2,
    APP_eReg_DesiredTempNight2,
    APP_eReg_DesiredTempAway,
    APP_eReg_DesiredTempWindowOpened,
    APP_eReg_CurrentDesiredTemperature,
    APP_eReg_CurrentTemperature,

    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_RemoteAddr00 = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_RemoteAddr00Low = APP_eCfg_RemoteAddr00,
    APP_eCfg_RemoteAddr00High,
    APP_eCfg_RemoteAddr01,
    APP_eCfg_RemoteAddr01Low = APP_eCfg_RemoteAddr01,
    APP_eCfg_RemoteAddr01High,
    APP_eCfg_RemoteAddr02,
    APP_eCfg_RemoteAddr02Low = APP_eCfg_RemoteAddr02,
    APP_eCfg_RemoteAddr02High,
    APP_eCfg_RemoteAddr03,
    APP_eCfg_RemoteAddr03Low = APP_eCfg_RemoteAddr03,
    APP_eCfg_RemoteAddr03High,
    APP_eCfg_RemoteAddr04,
    APP_eCfg_RemoteAddr04Low = APP_eCfg_RemoteAddr04,
    APP_eCfg_RemoteAddr04High,
    APP_eCfg_RemoteAddr05,
    APP_eCfg_RemoteAddr05Low = APP_eCfg_RemoteAddr05,
    APP_eCfg_RemoteAddr05High,
    APP_eCfg_RemoteAddr06,
    APP_eCfg_RemoteAddr06Low = APP_eCfg_RemoteAddr06,
    APP_eCfg_RemoteAddr06High,
    APP_eCfg_RemoteAddr07,
    APP_eCfg_RemoteAddr07Low = APP_eCfg_RemoteAddr07,
    APP_eCfg_RemoteAddr07High,
    APP_eCfg_RemoteAddr08,
    APP_eCfg_RemoteAddr08Low = APP_eCfg_RemoteAddr08,
    APP_eCfg_RemoteAddr08High,
    APP_eCfg_RemoteAddr09,
    APP_eCfg_RemoteAddr09Low = APP_eCfg_RemoteAddr09,
    APP_eCfg_RemoteAddr09High,
    APP_eCfg_RemoteAddr10,
    APP_eCfg_RemoteAddr10Low = APP_eCfg_RemoteAddr10,
    APP_eCfg_RemoteAddr10High,
    APP_eCfg_RemoteAddr11,
    APP_eCfg_RemoteAddr11Low = APP_eCfg_RemoteAddr11,
    APP_eCfg_RemoteAddr11High,
    APP_eCfg_RemoteAddr12,
    APP_eCfg_RemoteAddr12Low = APP_eCfg_RemoteAddr12,
    APP_eCfg_RemoteAddr12High,
    APP_eCfg_RemoteAddr13,
    APP_eCfg_RemoteAddr13Low = APP_eCfg_RemoteAddr13,
    APP_eCfg_RemoteAddr13High,
    APP_eCfg_RemoteAddr14,
    APP_eCfg_RemoteAddr14Low = APP_eCfg_RemoteAddr14,
    APP_eCfg_RemoteAddr14High,
    APP_eCfg_RemoteAddr15,
    APP_eCfg_RemoteAddr15Low = APP_eCfg_RemoteAddr15,
    APP_eCfg_RemoteAddr15High,
    APP_eCfg_RemoteAddr16,
    APP_eCfg_RemoteAddr16Low = APP_eCfg_RemoteAddr16,
    APP_eCfg_RemoteAddr16High,
    APP_eCfg_RemoteAddr17,
    APP_eCfg_RemoteAddr17Low = APP_eCfg_RemoteAddr17,
    APP_eCfg_RemoteAddr17High,
    APP_eCfg_RemoteAddr18,
    APP_eCfg_RemoteAddr18Low = APP_eCfg_RemoteAddr18,
    APP_eCfg_RemoteAddr18High,
    APP_eCfg_RemoteAddr19,
    APP_eCfg_RemoteAddr19Low = APP_eCfg_RemoteAddr19,
    APP_eCfg_RemoteAddr19High,
    APP_eCfg_RemoteAddr20,
    APP_eCfg_RemoteAddr20Low = APP_eCfg_RemoteAddr20,
    APP_eCfg_RemoteAddr20High,
    APP_eCfg_RemoteAddr21,
    APP_eCfg_RemoteAddr21Low = APP_eCfg_RemoteAddr21,
    APP_eCfg_RemoteAddr21High,
    APP_eCfg_RemoteAddr22,
    APP_eCfg_RemoteAddr22Low = APP_eCfg_RemoteAddr22,
    APP_eCfg_RemoteAddr22High,
    APP_eCfg_RemoteAddr23,
    APP_eCfg_RemoteAddr23Low = APP_eCfg_RemoteAddr23,
    APP_eCfg_RemoteAddr23High,
    APP_eCfg_RemoteAddr24,
    APP_eCfg_RemoteAddr24Low = APP_eCfg_RemoteAddr24,
    APP_eCfg_RemoteAddr24High,
    APP_eCfg_RemoteAddr25,
    APP_eCfg_RemoteAddr25Low = APP_eCfg_RemoteAddr25,
    APP_eCfg_RemoteAddr25High,
    APP_eCfg_RemoteAddr26,
    APP_eCfg_RemoteAddr26Low = APP_eCfg_RemoteAddr26,
    APP_eCfg_RemoteAddr26High,
    APP_eCfg_RemoteAddr27,
    APP_eCfg_RemoteAddr27Low = APP_eCfg_RemoteAddr27,
    APP_eCfg_RemoteAddr27High,
    APP_eCfg_RemoteAddr28,
    APP_eCfg_RemoteAddr28Low = APP_eCfg_RemoteAddr28,
    APP_eCfg_RemoteAddr28High,
    APP_eCfg_RemoteAddr29,
    APP_eCfg_RemoteAddr29Low = APP_eCfg_RemoteAddr29,
    APP_eCfg_RemoteAddr29High,
    APP_eCfg_RemoteAddr30,
    APP_eCfg_RemoteAddr30Low = APP_eCfg_RemoteAddr30,
    APP_eCfg_RemoteAddr30High,
    APP_eCfg_RemoteAddr31,
    APP_eCfg_RemoteAddr31Low = APP_eCfg_RemoteAddr31,
    APP_eCfg_RemoteAddr31High,
    APP_eCfg_RemoteReg00,
    APP_eCfg_RemoteReg01,
    APP_eCfg_RemoteReg02,
    APP_eCfg_RemoteReg03,
    APP_eCfg_RemoteReg04,
    APP_eCfg_RemoteReg05,
    APP_eCfg_RemoteReg06,
    APP_eCfg_RemoteReg07,
    APP_eCfg_RemoteReg08,
    APP_eCfg_RemoteReg09,
    APP_eCfg_RemoteReg10,
    APP_eCfg_RemoteReg11,
    APP_eCfg_RemoteReg12,
    APP_eCfg_RemoteReg13,
    APP_eCfg_RemoteReg14,
    APP_eCfg_RemoteReg15,
    APP_eCfg_RemoteReg16,
    APP_eCfg_RemoteReg17,
    APP_eCfg_RemoteReg18,
    APP_eCfg_RemoteReg19,
    APP_eCfg_RemoteReg20,
    APP_eCfg_RemoteReg21,
    APP_eCfg_RemoteReg22,
    APP_eCfg_RemoteReg23,
    APP_eCfg_RemoteReg24,
    APP_eCfg_RemoteReg25,
    APP_eCfg_RemoteReg26,
    APP_eCfg_RemoteReg27,
    APP_eCfg_RemoteReg28,
    APP_eCfg_RemoteReg29,
    APP_eCfg_RemoteReg30,
    APP_eCfg_RemoteReg31,
    APP_eCfg_TargetReg00,
    APP_eCfg_TargetReg01,
    APP_eCfg_TargetReg02,
    APP_eCfg_TargetReg03,
    APP_eCfg_TargetReg04,
    APP_eCfg_TargetReg05,
    APP_eCfg_TargetReg06,
    APP_eCfg_TargetReg07,
    APP_eCfg_TargetReg08,
    APP_eCfg_TargetReg09,
    APP_eCfg_TargetReg10,
    APP_eCfg_TargetReg11,
    APP_eCfg_TargetReg12,
    APP_eCfg_TargetReg13,
    APP_eCfg_TargetReg14,
    APP_eCfg_TargetReg15,
    APP_eCfg_TargetReg16,
    APP_eCfg_TargetReg17,
    APP_eCfg_TargetReg18,
    APP_eCfg_TargetReg19,
    APP_eCfg_TargetReg20,
    APP_eCfg_TargetReg21,
    APP_eCfg_TargetReg22,
    APP_eCfg_TargetReg23,
    APP_eCfg_TargetReg24,
    APP_eCfg_TargetReg25,
    APP_eCfg_TargetReg26,
    APP_eCfg_TargetReg27,
    APP_eCfg_TargetReg28,
    APP_eCfg_TargetReg29,
    APP_eCfg_TargetReg30,
    APP_eCfg_TargetReg31,
    APP_eCfg_DesiredTempDay1,
    APP_eCfg_DesiredTempDay1Lo = APP_eCfg_DesiredTempDay1,
    APP_eCfg_DesiredTempDay1High,
    APP_eCfg_DesiredTempNight1,
    APP_eCfg_DesiredTempNight1Lo = APP_eCfg_DesiredTempNight1,
    APP_eCfg_DesiredTempNight1High,
    APP_eCfg_DesiredTempDay2,
    APP_eCfg_DesiredTempDay2Lo = APP_eCfg_DesiredTempDay2,
    APP_eCfg_DesiredTempDay2High,
    APP_eCfg_DesiredTempNight2,
    APP_eCfg_DesiredTempNight2Lo = APP_eCfg_DesiredTempNight2,
    APP_eCfg_DesiredTempNight2High,
    APP_eCfg_DesiredTempAway,
    APP_eCfg_DesiredTempAwayLo = APP_eCfg_DesiredTempAway,
    APP_eCfg_DesiredTempAwayHigh,
    APP_eCfg_DesiredTempWindowOpened,
    APP_eCfg_DesiredTempWindowOpenedLo = APP_eCfg_DesiredTempWindowOpened,
    APP_eCfg_DesiredTempWindowOpenedHigh,
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
/** @} */
