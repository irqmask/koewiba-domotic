/**
 * @addtogroup BLINDCONTROL
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "blind-control_atmega328p" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "blind-control_atmega328p" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@web.de>
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

#include "cmddef_common.h"
#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG       1
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE   100     //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN    16     //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN    64 + 2 //!< Maximum length of a message to be received.

#define BLIND_APPCONFIG     1
#define BLIND_COUNT         1       //!< Number of blind which this software is capable to control.

#define DATETIME_APPCONFIG  1
#define DATETIME_APP_HAS_ON_MINUTE_FUNCTION 1

#define APP_UPDOWNTIMER_COUNT   4   //!< Number of up/down timer pairs.

#define ALARMCLOCK_APPCONFIG    1
#define ALARM_COUNT         (APP_UPDOWNTIMER_COUNT*2) //!< Number of programmable alarms.

#define APP_eSLEEPMASK_MOTOR    (1<<0)  //!< Mask used for sleep_prevent() function.
                                        //!< Prevent the controller to fall asleep if motor is moving.


// --- Type definitions --------------------------------------------------------

//! Application specific commands
typedef enum appcommands {
    APP_eCmd_Stop = eCMD_FIRST_APP_SPECIFIC,    //!< Stop blind movement
} eCommands_t;

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_B0_PositionCurrent = MOD_eReg_FirstAppSpecific, //!< Current position of blind
    APP_eReg_B0_PositionSetPoint,  //!< Wanted position of blind. 100 = completely closed. 0 = completely open
    APP_eReg_B0_PositionDifference,//!< Difference between wanted and current position.
    APP_eReg_B0_ReactionDelay,     //!< Delay time in timer ticks (1/100sec) until movement starts after applying power.
    APP_eReg_B0_DurationOpen,      //!< Duration in timer ticks (1/100sec) to fully open the blind.
    APP_eReg_B0_DurationClose,     //!< Duration in timer ticks (1/100sec) to fully close the blind.
    APP_eReg_B0_Mode,              //!< Mode how the blind/motor is controlled.
    APP_eReg_B0_Unused3,           //!< unused
    APP_eReg_B0_Unused2,           //!< unused
    APP_eReg_B0_Unused1,           //!< unused

    APP_eReg_TimeUp1_Weekday,
    APP_eReg_TimeUp1_Hour,
    APP_eReg_TimeUp1_Minute,
    APP_eReg_TimeUp1_Unused2,
    APP_eReg_TimeUp1_Unused1,
    APP_eReg_TimeDown1_Weekday,
    APP_eReg_TimeDown1_Hour,
    APP_eReg_TimeDown1_Minute,
    APP_eReg_TimeDown1_Unused2,
    APP_eReg_TimeDown1_Unused1,
    APP_eReg_TimeUp2_Weekday,
    APP_eReg_TimeUp2_Hour,
    APP_eReg_TimeUp2_Minute,
    APP_eReg_TimeUp2_Unused2,
    APP_eReg_TimeUp2_Unused1,
    APP_eReg_TimeDown2_Weekday,
    APP_eReg_TimeDown2_Hour,
    APP_eReg_TimeDown2_Minute,
    APP_eReg_TimeDown2_Unused2,
    APP_eReg_TimeDown2_Unused1,
    APP_eReg_TimeUp3_Weekday,
    APP_eReg_TimeUp3_Hour,
    APP_eReg_TimeUp3_Minute,
    APP_eReg_TimeUp3_Unused2,
    APP_eReg_TimeUp3_Unused1,
    APP_eReg_TimeDown3_Weekday,
    APP_eReg_TimeDown3_Hour,
    APP_eReg_TimeDown3_Minute,
    APP_eReg_TimeDown3_Unused2,
    APP_eReg_TimeDown3_Unused1,
    APP_eReg_TimeUp4_Weekday,
    APP_eReg_TimeUp4_Hour,
    APP_eReg_TimeUp4_Minute,
    APP_eReg_TimeUp4_Unused2,
    APP_eReg_TimeUp4_Unused1,
    APP_eReg_TimeDown4_Weekday,
    APP_eReg_TimeDown4_Hour,
    APP_eReg_TimeDown4_Minute,
    APP_eReg_TimeDown4_Unused2,
    APP_eReg_TimeDown4_Unused1,

    APP_eReg_WindowState = 46,  //!< State of the window

    APP_eReg_Year = 223,        //!< Current year.
    APP_eReg_Month,             //!< Current month.
    APP_eReg_Day,               //!< Current day.
    APP_eReg_DayOfWeek,         //!< Current day of week.
    APP_eReg_Hour,              //!< Current hour.
    APP_eReg_Minute,            //!< Current minute.
    APP_eReg_Second,            //!< Current second.

    // insert application specific registers here
    APP_eReg_Last
} eRegisters_t;

//! Number of registers per blind
#define APP_NUM_REGS_PER_BLIND (APP_eReg_B0_Unused2 - APP_eReg_B0_PositionCurrent + 1)

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_ReactionDelay = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_DurationOpen,
    APP_eCfg_DurationOpenLow = APP_eCfg_DurationOpen,
    APP_eCfg_DurationOpenHigh,
    APP_eCfg_DurationClose,
    APP_eCfg_DurationCloseLow = APP_eCfg_DurationClose,
    APP_eCfg_DurationCloseHigh,
    APP_eCfg_Mode,

    APP_eCfg_TimeUp1_Weekdays,
    APP_eCfg_TimeUp1_Hour,
    APP_eCfg_TimeUp1_Minute,
    APP_eCfg_TimeDown1_Weekdays,
    APP_eCfg_TimeDown1_Hour,
    APP_eCfg_TimeDown1_Minute,
    APP_eCfg_TimeUp2_Weekdays,
    APP_eCfg_TimeUp2_Hour,
    APP_eCfg_TimeUp2_Minute,
    APP_eCfg_TimeDown2_Weekdays,
    APP_eCfg_TimeDown2_Hour,
    APP_eCfg_TimeDown2_Minute,
    APP_eCfg_TimeUp3_Weekdays,
    APP_eCfg_TimeUp3_Hour,
    APP_eCfg_TimeUp3_Minute,
    APP_eCfg_TimeDown3_Weekdays,
    APP_eCfg_TimeDown3_Hour,
    APP_eCfg_TimeDown3_Minute,
    APP_eCfg_TimeUp4_Weekdays,
    APP_eCfg_TimeUp4_Hour,
    APP_eCfg_TimeUp4_Minute,
    APP_eCfg_TimeDown4_Weekdays,
    APP_eCfg_TimeDown4_Hour,
    APP_eCfg_TimeDown4_Minute,

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
