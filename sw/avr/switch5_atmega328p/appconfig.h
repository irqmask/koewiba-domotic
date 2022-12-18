/**
 * @addtogroup SWITCH5_ATMEGA328S
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "switch5_atmega328" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "switch5_atmega328" project.
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

#define DATETIME_APPCONFIG  1
#define DATETIME_APP_HAS_ON_MINUTE_FUNCTION 1


#define APP_NUM_CHANNEL         5   //!< Number of channels. Always change along
                                    //!< with number of output channels!
#define APP_SWITCHTIMER_COUNT   8   //!< Number of timepoints per channel.
#define APP_REGS_PER_ALARM      4   //!< Number of registers per alarmclock

// order of alarms is
//  0: chn 0 idx 0
//  1: chn 0 idx 1
//  2: chn 0 idx 2
// ...
//  7: chn 0 idx 7
// ...
// 37: chn 4 idx 5
// 38: chn 4 idx 6
// 39: chn 4 idx 7

#define ALARMCLOCK_APPCONFIG    1

//! Number of programmable alarms.
#define ALARM_COUNT         (APP_NUM_CHANNEL * APP_SWITCHTIMER_COUNT)

#define TIMER_WAKEUP        1       //!< Timer shall wake the controller.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_Chn0_SwitchCurrent = MOD_eReg_FirstAppSpecific,
    APP_eReg_Chn0_SwitchSetPoint,
    APP_eReg_Chn0_ThresholdOff,
    APP_eReg_Chn0_ThresholdOn,
    APP_eReg_Chn0_Mode,
    APP_eReg_Chn0_Timer0_Weekday,
    APP_eReg_Chn0_Timer0_Hour,
    APP_eReg_Chn0_Timer0_Minute,
    APP_eReg_Chn0_Timer0_SetPoint,
    APP_eReg_Chn0_Timer1_Weekday,
    APP_eReg_Chn0_Timer1_Hour,
    APP_eReg_Chn0_Timer1_Minute,
    APP_eReg_Chn0_Timer1_SetPoint,
    APP_eReg_Chn0_Timer2_Weekday,
    APP_eReg_Chn0_Timer2_Hour,
    APP_eReg_Chn0_Timer2_Minute,
    APP_eReg_Chn0_Timer2_SetPoint,
    APP_eReg_Chn0_Timer3_Weekday,
    APP_eReg_Chn0_Timer3_Hour,
    APP_eReg_Chn0_Timer3_Minute,
    APP_eReg_Chn0_Timer3_SetPoint,
    APP_eReg_Chn0_Timer4_Weekday,
    APP_eReg_Chn0_Timer4_Hour,
    APP_eReg_Chn0_Timer4_Minute,
    APP_eReg_Chn0_Timer4_SetPoint,
    APP_eReg_Chn0_Timer5_Weekday,
    APP_eReg_Chn0_Timer5_Hour,
    APP_eReg_Chn0_Timer5_Minute,
    APP_eReg_Chn0_Timer5_SetPoint,
    APP_eReg_Chn0_Timer6_Weekday,
    APP_eReg_Chn0_Timer6_Hour,
    APP_eReg_Chn0_Timer6_Minute,
    APP_eReg_Chn0_Timer6_SetPoint,
    APP_eReg_Chn0_Timer7_Weekday,
    APP_eReg_Chn0_Timer7_Hour,
    APP_eReg_Chn0_Timer7_Minute,
    APP_eReg_Chn0_Timer7_SetPoint,
    APP_eReg_Chn0_Unused2,
    APP_eReg_Chn0_Unused1,
    APP_eReg_Chn0_Unused0,

    APP_eReg_Chn1_SwitchCurrent,
    APP_eReg_Chn1_SwitchSetPoint,
    APP_eReg_Chn1_ThresholdOff,
    APP_eReg_Chn1_ThresholdOn,
    APP_eReg_Chn1_Mode,
    APP_eReg_Chn1_Timer0_Weekday,
    APP_eReg_Chn1_Timer0_Hour,
    APP_eReg_Chn1_Timer0_Minute,
    APP_eReg_Chn1_Timer0_SetPoint,
    APP_eReg_Chn1_Timer1_Weekday,
    APP_eReg_Chn1_Timer1_Hour,
    APP_eReg_Chn1_Timer1_Minute,
    APP_eReg_Chn1_Timer1_SetPoint,
    APP_eReg_Chn1_Timer2_Weekday,
    APP_eReg_Chn1_Timer2_Hour,
    APP_eReg_Chn1_Timer2_Minute,
    APP_eReg_Chn1_Timer2_SetPoint,
    APP_eReg_Chn1_Timer3_Weekday,
    APP_eReg_Chn1_Timer3_Hour,
    APP_eReg_Chn1_Timer3_Minute,
    APP_eReg_Chn1_Timer3_SetPoint,
    APP_eReg_Chn1_Timer4_Weekday,
    APP_eReg_Chn1_Timer4_Hour,
    APP_eReg_Chn1_Timer4_Minute,
    APP_eReg_Chn1_Timer4_SetPoint,
    APP_eReg_Chn1_Timer5_Weekday,
    APP_eReg_Chn1_Timer5_Hour,
    APP_eReg_Chn1_Timer5_Minute,
    APP_eReg_Chn1_Timer5_SetPoint,
    APP_eReg_Chn1_Timer6_Weekday,
    APP_eReg_Chn1_Timer6_Hour,
    APP_eReg_Chn1_Timer6_Minute,
    APP_eReg_Chn1_Timer6_SetPoint,
    APP_eReg_Chn1_Timer7_Weekday,
    APP_eReg_Chn1_Timer7_Hour,
    APP_eReg_Chn1_Timer7_Minute,
    APP_eReg_Chn1_Timer7_SetPoint,
    APP_eReg_Chn1_Unused2,
    APP_eReg_Chn1_Unused1,
    APP_eReg_Chn1_Unused0,

    APP_eReg_Chn2_SwitchCurrent,
    APP_eReg_Chn2_SwitchSetPoint,
    APP_eReg_Chn2_ThresholdOff,
    APP_eReg_Chn2_ThresholdOn,
    APP_eReg_Chn2_Mode,
    APP_eReg_Chn2_Timer0_Weekday,
    APP_eReg_Chn2_Timer0_Hour,
    APP_eReg_Chn2_Timer0_Minute,
    APP_eReg_Chn2_Timer0_SetPoint,
    APP_eReg_Chn2_Timer1_Weekday,
    APP_eReg_Chn2_Timer1_Hour,
    APP_eReg_Chn2_Timer1_Minute,
    APP_eReg_Chn2_Timer1_SetPoint,
    APP_eReg_Chn2_Timer2_Weekday,
    APP_eReg_Chn2_Timer2_Hour,
    APP_eReg_Chn2_Timer2_Minute,
    APP_eReg_Chn2_Timer2_SetPoint,
    APP_eReg_Chn2_Timer3_Weekday,
    APP_eReg_Chn2_Timer3_Hour,
    APP_eReg_Chn2_Timer3_Minute,
    APP_eReg_Chn2_Timer3_SetPoint,
    APP_eReg_Chn2_Timer4_Weekday,
    APP_eReg_Chn2_Timer4_Hour,
    APP_eReg_Chn2_Timer4_Minute,
    APP_eReg_Chn2_Timer4_SetPoint,
    APP_eReg_Chn2_Timer5_Weekday,
    APP_eReg_Chn2_Timer5_Hour,
    APP_eReg_Chn2_Timer5_Minute,
    APP_eReg_Chn2_Timer5_SetPoint,
    APP_eReg_Chn2_Timer6_Weekday,
    APP_eReg_Chn2_Timer6_Hour,
    APP_eReg_Chn2_Timer6_Minute,
    APP_eReg_Chn2_Timer6_SetPoint,
    APP_eReg_Chn2_Timer7_Weekday,
    APP_eReg_Chn2_Timer7_Hour,
    APP_eReg_Chn2_Timer7_Minute,
    APP_eReg_Chn2_Timer7_SetPoint,
    APP_eReg_Chn2_Unused2,
    APP_eReg_Chn2_Unused1,
    APP_eReg_Chn2_Unused0,

    APP_eReg_Chn3_SwitchCurrent,
    APP_eReg_Chn3_SwitchSetPoint,
    APP_eReg_Chn3_ThresholdOff,
    APP_eReg_Chn3_ThresholdOn,
    APP_eReg_Chn3_Mode,
    APP_eReg_Chn3_Timer0_Weekday,
    APP_eReg_Chn3_Timer0_Hour,
    APP_eReg_Chn3_Timer0_Minute,
    APP_eReg_Chn3_Timer0_SetPoint,
    APP_eReg_Chn3_Timer1_Weekday,
    APP_eReg_Chn3_Timer1_Hour,
    APP_eReg_Chn3_Timer1_Minute,
    APP_eReg_Chn3_Timer1_SetPoint,
    APP_eReg_Chn3_Timer2_Weekday,
    APP_eReg_Chn3_Timer2_Hour,
    APP_eReg_Chn3_Timer2_Minute,
    APP_eReg_Chn3_Timer2_SetPoint,
    APP_eReg_Chn3_Timer3_Weekday,
    APP_eReg_Chn3_Timer3_Hour,
    APP_eReg_Chn3_Timer3_Minute,
    APP_eReg_Chn3_Timer3_SetPoint,
    APP_eReg_Chn3_Timer4_Weekday,
    APP_eReg_Chn3_Timer4_Hour,
    APP_eReg_Chn3_Timer4_Minute,
    APP_eReg_Chn3_Timer4_SetPoint,
    APP_eReg_Chn3_Timer5_Weekday,
    APP_eReg_Chn3_Timer5_Hour,
    APP_eReg_Chn3_Timer5_Minute,
    APP_eReg_Chn3_Timer5_SetPoint,
    APP_eReg_Chn3_Timer6_Weekday,
    APP_eReg_Chn3_Timer6_Hour,
    APP_eReg_Chn3_Timer6_Minute,
    APP_eReg_Chn3_Timer6_SetPoint,
    APP_eReg_Chn3_Timer7_Weekday,
    APP_eReg_Chn3_Timer7_Hour,
    APP_eReg_Chn3_Timer7_Minute,
    APP_eReg_Chn3_Timer7_SetPoint,
    APP_eReg_Chn3_Unused2,
    APP_eReg_Chn3_Unused1,
    APP_eReg_Chn3_Unused0,

    APP_eReg_Chn4_SwitchCurrent,
    APP_eReg_Chn4_SwitchSetPoint,
    APP_eReg_Chn4_ThresholdOff,
    APP_eReg_Chn4_ThresholdOn,
    APP_eReg_Chn4_Mode,
    APP_eReg_Chn4_Timer0_Weekday,
    APP_eReg_Chn4_Timer0_Hour,
    APP_eReg_Chn4_Timer0_Minute,
    APP_eReg_Chn4_Timer0_SetPoint,
    APP_eReg_Chn4_Timer1_Weekday,
    APP_eReg_Chn4_Timer1_Hour,
    APP_eReg_Chn4_Timer1_Minute,
    APP_eReg_Chn4_Timer1_SetPoint,
    APP_eReg_Chn4_Timer2_Weekday,
    APP_eReg_Chn4_Timer2_Hour,
    APP_eReg_Chn4_Timer2_Minute,
    APP_eReg_Chn4_Timer2_SetPoint,
    APP_eReg_Chn4_Timer3_Weekday,
    APP_eReg_Chn4_Timer3_Hour,
    APP_eReg_Chn4_Timer3_Minute,
    APP_eReg_Chn4_Timer3_SetPoint,
    APP_eReg_Chn4_Timer4_Weekday,
    APP_eReg_Chn4_Timer4_Hour,
    APP_eReg_Chn4_Timer4_Minute,
    APP_eReg_Chn4_Timer4_SetPoint,
    APP_eReg_Chn4_Timer5_Weekday,
    APP_eReg_Chn4_Timer5_Hour,
    APP_eReg_Chn4_Timer5_Minute,
    APP_eReg_Chn4_Timer5_SetPoint,
    APP_eReg_Chn4_Timer6_Weekday,
    APP_eReg_Chn4_Timer6_Hour,
    APP_eReg_Chn4_Timer6_Minute,
    APP_eReg_Chn4_Timer6_SetPoint,
    APP_eReg_Chn4_Timer7_Weekday,
    APP_eReg_Chn4_Timer7_Hour,
    APP_eReg_Chn4_Timer7_Minute,
    APP_eReg_Chn4_Timer7_SetPoint,
    APP_eReg_Chn4_Unused2,
    APP_eReg_Chn4_Unused1,
    APP_eReg_Chn4_Unused0,

    // insert application specific registers here

    APP_eReg_Year = 223,        //!< Current year.
    APP_eReg_Month,             //!< Current month.
    APP_eReg_Day,               //!< Current day.
    APP_eReg_DayOfWeek,         //!< Current day of week.
    APP_eReg_Hour,              //!< Current hour.
    APP_eReg_Minute,            //!< Current minute.
    APP_eReg_Second,            //!< Current second.

    APP_eReg_Last
} eRegisters_t;

//! Number of registers per channel
#define APP_REGS_PER_CHN (APP_eReg_Chn0_Unused0 - APP_eReg_Chn0_SwitchCurrent + 1)

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_Chn0_ThresholdOff = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_Chn0_ThresholdOn,
    APP_eCfg_Chn0_Mode,
    APP_eCfg_Chn0_Timer0_Weekday,
    APP_eCfg_Chn0_Timer0_Hour,
    APP_eCfg_Chn0_Timer0_Minute,
    APP_eCfg_Chn0_Timer0_SetPoint,
    APP_eCfg_Chn0_Timer1_Weekday,
    APP_eCfg_Chn0_Timer1_Hour,
    APP_eCfg_Chn0_Timer1_Minute,
    APP_eCfg_Chn0_Timer1_SetPoint,
    APP_eCfg_Chn0_Timer2_Weekday,
    APP_eCfg_Chn0_Timer2_Hour,
    APP_eCfg_Chn0_Timer2_Minute,
    APP_eCfg_Chn0_Timer2_SetPoint,
    APP_eCfg_Chn0_Timer3_Weekday,
    APP_eCfg_Chn0_Timer3_Hour,
    APP_eCfg_Chn0_Timer3_Minute,
    APP_eCfg_Chn0_Timer3_SetPoint,
    APP_eCfg_Chn0_Timer4_Weekday,
    APP_eCfg_Chn0_Timer4_Hour,
    APP_eCfg_Chn0_Timer4_Minute,
    APP_eCfg_Chn0_Timer4_SetPoint,
    APP_eCfg_Chn0_Timer5_Weekday,
    APP_eCfg_Chn0_Timer5_Hour,
    APP_eCfg_Chn0_Timer5_Minute,
    APP_eCfg_Chn0_Timer5_SetPoint,
    APP_eCfg_Chn0_Timer6_Weekday,
    APP_eCfg_Chn0_Timer6_Hour,
    APP_eCfg_Chn0_Timer6_Minute,
    APP_eCfg_Chn0_Timer6_SetPoint,
    APP_eCfg_Chn0_Timer7_Weekday,
    APP_eCfg_Chn0_Timer7_Hour,
    APP_eCfg_Chn0_Timer7_Minute,
    APP_eCfg_Chn0_Timer7_SetPoint,
    APP_eCfg_Chn0_Unused0,

    APP_eCfg_Chn1_ThresholdOff,
    APP_eCfg_Chn1_ThresholdOn,
    APP_eCfg_Chn1_Mode,
    APP_eCfg_Chn1_Timer0_Weekday,
    APP_eCfg_Chn1_Timer0_Hour,
    APP_eCfg_Chn1_Timer0_Minute,
    APP_eCfg_Chn1_Timer0_SetPoint,
    APP_eCfg_Chn1_Timer1_Weekday,
    APP_eCfg_Chn1_Timer1_Hour,
    APP_eCfg_Chn1_Timer1_Minute,
    APP_eCfg_Chn1_Timer1_SetPoint,
    APP_eCfg_Chn1_Timer2_Weekday,
    APP_eCfg_Chn1_Timer2_Hour,
    APP_eCfg_Chn1_Timer2_Minute,
    APP_eCfg_Chn1_Timer2_SetPoint,
    APP_eCfg_Chn1_Timer3_Weekday,
    APP_eCfg_Chn1_Timer3_Hour,
    APP_eCfg_Chn1_Timer3_Minute,
    APP_eCfg_Chn1_Timer3_SetPoint,
    APP_eCfg_Chn1_Timer4_Weekday,
    APP_eCfg_Chn1_Timer4_Hour,
    APP_eCfg_Chn1_Timer4_Minute,
    APP_eCfg_Chn1_Timer4_SetPoint,
    APP_eCfg_Chn1_Timer5_Weekday,
    APP_eCfg_Chn1_Timer5_Hour,
    APP_eCfg_Chn1_Timer5_Minute,
    APP_eCfg_Chn1_Timer5_SetPoint,
    APP_eCfg_Chn1_Timer6_Weekday,
    APP_eCfg_Chn1_Timer6_Hour,
    APP_eCfg_Chn1_Timer6_Minute,
    APP_eCfg_Chn1_Timer6_SetPoint,
    APP_eCfg_Chn1_Timer7_Weekday,
    APP_eCfg_Chn1_Timer7_Hour,
    APP_eCfg_Chn1_Timer7_Minute,
    APP_eCfg_Chn1_Timer7_SetPoint,
    APP_eCfg_Chn1_Unused0,

    APP_eCfg_Chn2_ThresholdOff,
    APP_eCfg_Chn2_ThresholdOn,
    APP_eCfg_Chn2_Mode,
    APP_eCfg_Chn2_Timer0_Weekday,
    APP_eCfg_Chn2_Timer0_Hour,
    APP_eCfg_Chn2_Timer0_Minute,
    APP_eCfg_Chn2_Timer0_SetPoint,
    APP_eCfg_Chn2_Timer1_Weekday,
    APP_eCfg_Chn2_Timer1_Hour,
    APP_eCfg_Chn2_Timer1_Minute,
    APP_eCfg_Chn2_Timer1_SetPoint,
    APP_eCfg_Chn2_Timer2_Weekday,
    APP_eCfg_Chn2_Timer2_Hour,
    APP_eCfg_Chn2_Timer2_Minute,
    APP_eCfg_Chn2_Timer2_SetPoint,
    APP_eCfg_Chn2_Timer3_Weekday,
    APP_eCfg_Chn2_Timer3_Hour,
    APP_eCfg_Chn2_Timer3_Minute,
    APP_eCfg_Chn2_Timer3_SetPoint,
    APP_eCfg_Chn2_Timer4_Weekday,
    APP_eCfg_Chn2_Timer4_Hour,
    APP_eCfg_Chn2_Timer4_Minute,
    APP_eCfg_Chn2_Timer4_SetPoint,
    APP_eCfg_Chn2_Timer5_Weekday,
    APP_eCfg_Chn2_Timer5_Hour,
    APP_eCfg_Chn2_Timer5_Minute,
    APP_eCfg_Chn2_Timer5_SetPoint,
    APP_eCfg_Chn2_Timer6_Weekday,
    APP_eCfg_Chn2_Timer6_Hour,
    APP_eCfg_Chn2_Timer6_Minute,
    APP_eCfg_Chn2_Timer6_SetPoint,
    APP_eCfg_Chn2_Timer7_Weekday,
    APP_eCfg_Chn2_Timer7_Hour,
    APP_eCfg_Chn2_Timer7_Minute,
    APP_eCfg_Chn2_Timer7_SetPoint,
    APP_eCfg_Chn2_Unused0,

    APP_eCfg_Chn3_ThresholdOff,
    APP_eCfg_Chn3_ThresholdOn,
    APP_eCfg_Chn3_Mode,
    APP_eCfg_Chn3_Timer0_Weekday,
    APP_eCfg_Chn3_Timer0_Hour,
    APP_eCfg_Chn3_Timer0_Minute,
    APP_eCfg_Chn3_Timer0_SetPoint,
    APP_eCfg_Chn3_Timer1_Weekday,
    APP_eCfg_Chn3_Timer1_Hour,
    APP_eCfg_Chn3_Timer1_Minute,
    APP_eCfg_Chn3_Timer1_SetPoint,
    APP_eCfg_Chn3_Timer2_Weekday,
    APP_eCfg_Chn3_Timer2_Hour,
    APP_eCfg_Chn3_Timer2_Minute,
    APP_eCfg_Chn3_Timer2_SetPoint,
    APP_eCfg_Chn3_Timer3_Weekday,
    APP_eCfg_Chn3_Timer3_Hour,
    APP_eCfg_Chn3_Timer3_Minute,
    APP_eCfg_Chn3_Timer3_SetPoint,
    APP_eCfg_Chn3_Timer4_Weekday,
    APP_eCfg_Chn3_Timer4_Hour,
    APP_eCfg_Chn3_Timer4_Minute,
    APP_eCfg_Chn3_Timer4_SetPoint,
    APP_eCfg_Chn3_Timer5_Weekday,
    APP_eCfg_Chn3_Timer5_Hour,
    APP_eCfg_Chn3_Timer5_Minute,
    APP_eCfg_Chn3_Timer5_SetPoint,
    APP_eCfg_Chn3_Timer6_Weekday,
    APP_eCfg_Chn3_Timer6_Hour,
    APP_eCfg_Chn3_Timer6_Minute,
    APP_eCfg_Chn3_Timer6_SetPoint,
    APP_eCfg_Chn3_Timer7_Weekday,
    APP_eCfg_Chn3_Timer7_Hour,
    APP_eCfg_Chn3_Timer7_Minute,
    APP_eCfg_Chn3_Timer7_SetPoint,
    APP_eCfg_Chn3_Unused0,

    APP_eCfg_Chn4_ThresholdOff,
    APP_eCfg_Chn4_ThresholdOn,
    APP_eCfg_Chn4_Mode,
    APP_eCfg_Chn4_Timer0_Weekday,
    APP_eCfg_Chn4_Timer0_Hour,
    APP_eCfg_Chn4_Timer0_Minute,
    APP_eCfg_Chn4_Timer0_SetPoint,
    APP_eCfg_Chn4_Timer1_Weekday,
    APP_eCfg_Chn4_Timer1_Hour,
    APP_eCfg_Chn4_Timer1_Minute,
    APP_eCfg_Chn4_Timer1_SetPoint,
    APP_eCfg_Chn4_Timer2_Weekday,
    APP_eCfg_Chn4_Timer2_Hour,
    APP_eCfg_Chn4_Timer2_Minute,
    APP_eCfg_Chn4_Timer2_SetPoint,
    APP_eCfg_Chn4_Timer3_Weekday,
    APP_eCfg_Chn4_Timer3_Hour,
    APP_eCfg_Chn4_Timer3_Minute,
    APP_eCfg_Chn4_Timer3_SetPoint,
    APP_eCfg_Chn4_Timer4_Weekday,
    APP_eCfg_Chn4_Timer4_Hour,
    APP_eCfg_Chn4_Timer4_Minute,
    APP_eCfg_Chn4_Timer4_SetPoint,
    APP_eCfg_Chn4_Timer5_Weekday,
    APP_eCfg_Chn4_Timer5_Hour,
    APP_eCfg_Chn4_Timer5_Minute,
    APP_eCfg_Chn4_Timer5_SetPoint,
    APP_eCfg_Chn4_Timer6_Weekday,
    APP_eCfg_Chn4_Timer6_Hour,
    APP_eCfg_Chn4_Timer6_Minute,
    APP_eCfg_Chn4_Timer6_SetPoint,
    APP_eCfg_Chn4_Timer7_Weekday,
    APP_eCfg_Chn4_Timer7_Hour,
    APP_eCfg_Chn4_Timer7_Minute,
    APP_eCfg_Chn4_Timer7_SetPoint,
    APP_eCfg_Chn4_Unused0,

    // insert application specific configuration here
    APP_eCfg_Last
} eConfiguration_t;

//! Number of bytes in non-volatile memory per channel
#define APP_CFG_BYTES_PER_CHN (APP_eCfg_Chn0_Unused0 - APP_eCfg_Chn0_ThresholdOff + 1)

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
