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
#define APP_ONOFFTIMER_COUNT    4   //!< Number of on/off pairs per channel.
#define APP_REGS_PER_ALARM      3   //!< Number of registers per alarmclock

// order of alarms is
//  0: chn 0 idx 0 on
//  1: chn 0 idx 0 off
//  2: chn 0 idx 1 on
//  3: chn 0 idx 1 off
//  4: chn 0 idx 2 on
// ...
// 36: chn 4 idx 2 on
// 37: chn 4 idx 2 off
// 38: chn 4 idx 3 on
// 39: chn 4 idx 3 off

#define ALARMCLOCK_APPCONFIG    1

//! Number of programmable alarms.
#define ALARM_COUNT         (APP_NUM_CHANNEL * APP_ONOFFTIMER_COUNT * 2)

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
    APP_eReg_Chn0_TimeOn0_Weekday,
    APP_eReg_Chn0_TimeOn0_Hour,
    APP_eReg_Chn0_TimeOn0_Minute,
    APP_eReg_Chn0_TimeOff0_Weekday,
    APP_eReg_Chn0_TimeOff0_Hour,
    APP_eReg_Chn0_TimeOff0_Minute,
    APP_eReg_Chn0_TimeOn1_Weekday,
    APP_eReg_Chn0_TimeOn1_Hour,
    APP_eReg_Chn0_TimeOn1_Minute,
    APP_eReg_Chn0_TimeOff1_Weekday,
    APP_eReg_Chn0_TimeOff1_Hour,
    APP_eReg_Chn0_TimeOff1_Minute,
    APP_eReg_Chn0_TimeOn2_Weekday,
    APP_eReg_Chn0_TimeOn2_Hour,
    APP_eReg_Chn0_TimeOn2_Minute,
    APP_eReg_Chn0_TimeOff2_Weekday,
    APP_eReg_Chn0_TimeOff2_Hour,
    APP_eReg_Chn0_TimeOff2_Minute,
    APP_eReg_Chn0_TimeOn3_Weekday,
    APP_eReg_Chn0_TimeOn3_Hour,
    APP_eReg_Chn0_TimeOn3_Minute,
    APP_eReg_Chn0_TimeOff3_Weekday,
    APP_eReg_Chn0_TimeOff3_Hour,
    APP_eReg_Chn0_TimeOff3_Minute,
    APP_eReg_Chn0_Unused10,
    APP_eReg_Chn0_Unused9,
    APP_eReg_Chn0_Unused8,
    APP_eReg_Chn0_Unused7,
    APP_eReg_Chn0_Unused6,
    APP_eReg_Chn0_Unused5,
    APP_eReg_Chn0_Unused4,
    APP_eReg_Chn0_Unused3,
    APP_eReg_Chn0_Unused2,
    APP_eReg_Chn0_Unused1,
    APP_eReg_Chn0_Unused0,

    APP_eReg_Chn1_SwitchCurrent,
    APP_eReg_Chn1_SwitchSetPoint,
    APP_eReg_Chn1_ThresholdOff,
    APP_eReg_Chn1_ThresholdOn,
    APP_eReg_Chn1_Mode,
    APP_eReg_Chn1_TimeOn0_Weekday,
    APP_eReg_Chn1_TimeOn0_Hour,
    APP_eReg_Chn1_TimeOn0_Minute,
    APP_eReg_Chn1_TimeOff0_Weekday,
    APP_eReg_Chn1_TimeOff0_Hour,
    APP_eReg_Chn1_TimeOff0_Minute,
    APP_eReg_Chn1_TimeOn1_Weekday,
    APP_eReg_Chn1_TimeOn1_Hour,
    APP_eReg_Chn1_TimeOn1_Minute,
    APP_eReg_Chn1_TimeOff1_Weekday,
    APP_eReg_Chn1_TimeOff1_Hour,
    APP_eReg_Chn1_TimeOff1_Minute,
    APP_eReg_Chn1_TimeOn2_Weekday,
    APP_eReg_Chn1_TimeOn2_Hour,
    APP_eReg_Chn1_TimeOn2_Minute,
    APP_eReg_Chn1_TimeOff2_Weekday,
    APP_eReg_Chn1_TimeOff2_Hour,
    APP_eReg_Chn1_TimeOff2_Minute,
    APP_eReg_Chn1_TimeOn3_Weekday,
    APP_eReg_Chn1_TimeOn3_Hour,
    APP_eReg_Chn1_TimeOn3_Minute,
    APP_eReg_Chn1_TimeOff3_Weekday,
    APP_eReg_Chn1_TimeOff3_Hour,
    APP_eReg_Chn1_TimeOff3_Minute,
    APP_eReg_Chn1_Unused10,
    APP_eReg_Chn1_Unused9,
    APP_eReg_Chn1_Unused8,
    APP_eReg_Chn1_Unused7,
    APP_eReg_Chn1_Unused6,
    APP_eReg_Chn1_Unused5,
    APP_eReg_Chn1_Unused4,
    APP_eReg_Chn1_Unused3,
    APP_eReg_Chn1_Unused2,
    APP_eReg_Chn1_Unused1,
    APP_eReg_Chn1_Unused0,

    APP_eReg_Chn2_SwitchCurrent,
    APP_eReg_Chn2_SwitchSetPoint,
    APP_eReg_Chn2_ThresholdOff,
    APP_eReg_Chn2_ThresholdOn,
    APP_eReg_Chn2_Mode,
    APP_eReg_Chn2_TimeOn0_Weekday,
    APP_eReg_Chn2_TimeOn0_Hour,
    APP_eReg_Chn2_TimeOn0_Minute,
    APP_eReg_Chn2_TimeOff0_Weekday,
    APP_eReg_Chn2_TimeOff0_Hour,
    APP_eReg_Chn2_TimeOff0_Minute,
    APP_eReg_Chn2_TimeOn1_Weekday,
    APP_eReg_Chn2_TimeOn1_Hour,
    APP_eReg_Chn2_TimeOn1_Minute,
    APP_eReg_Chn2_TimeOff1_Weekday,
    APP_eReg_Chn2_TimeOff1_Hour,
    APP_eReg_Chn2_TimeOff1_Minute,
    APP_eReg_Chn2_TimeOn2_Weekday,
    APP_eReg_Chn2_TimeOn2_Hour,
    APP_eReg_Chn2_TimeOn2_Minute,
    APP_eReg_Chn2_TimeOff2_Weekday,
    APP_eReg_Chn2_TimeOff2_Hour,
    APP_eReg_Chn2_TimeOff2_Minute,
    APP_eReg_Chn2_TimeOn3_Weekday,
    APP_eReg_Chn2_TimeOn3_Hour,
    APP_eReg_Chn2_TimeOn3_Minute,
    APP_eReg_Chn2_TimeOff3_Weekday,
    APP_eReg_Chn2_TimeOff3_Hour,
    APP_eReg_Chn2_TimeOff3_Minute,
    APP_eReg_Chn2_Unused10,
    APP_eReg_Chn2_Unused9,
    APP_eReg_Chn2_Unused8,
    APP_eReg_Chn2_Unused7,
    APP_eReg_Chn2_Unused6,
    APP_eReg_Chn2_Unused5,
    APP_eReg_Chn2_Unused4,
    APP_eReg_Chn2_Unused3,
    APP_eReg_Chn2_Unused2,
    APP_eReg_Chn2_Unused1,
    APP_eReg_Chn2_Unused0,

    APP_eReg_Chn3_SwitchCurrent,
    APP_eReg_Chn3_SwitchSetPoint,
    APP_eReg_Chn3_ThresholdOff,
    APP_eReg_Chn3_ThresholdOn,
    APP_eReg_Chn3_Mode,
    APP_eReg_Chn3_TimeOn0_Weekday,
    APP_eReg_Chn3_TimeOn0_Hour,
    APP_eReg_Chn3_TimeOn0_Minute,
    APP_eReg_Chn3_TimeOff0_Weekday,
    APP_eReg_Chn3_TimeOff0_Hour,
    APP_eReg_Chn3_TimeOff0_Minute,
    APP_eReg_Chn3_TimeOn1_Weekday,
    APP_eReg_Chn3_TimeOn1_Hour,
    APP_eReg_Chn3_TimeOn1_Minute,
    APP_eReg_Chn3_TimeOff1_Weekday,
    APP_eReg_Chn3_TimeOff1_Hour,
    APP_eReg_Chn3_TimeOff1_Minute,
    APP_eReg_Chn3_TimeOn2_Weekday,
    APP_eReg_Chn3_TimeOn2_Hour,
    APP_eReg_Chn3_TimeOn2_Minute,
    APP_eReg_Chn3_TimeOff2_Weekday,
    APP_eReg_Chn3_TimeOff2_Hour,
    APP_eReg_Chn3_TimeOff2_Minute,
    APP_eReg_Chn3_TimeOn3_Weekday,
    APP_eReg_Chn3_TimeOn3_Hour,
    APP_eReg_Chn3_TimeOn3_Minute,
    APP_eReg_Chn3_TimeOff3_Weekday,
    APP_eReg_Chn3_TimeOff3_Hour,
    APP_eReg_Chn3_TimeOff3_Minute,
    APP_eReg_Chn3_Unused10,
    APP_eReg_Chn3_Unused9,
    APP_eReg_Chn3_Unused8,
    APP_eReg_Chn3_Unused7,
    APP_eReg_Chn3_Unused6,
    APP_eReg_Chn3_Unused5,
    APP_eReg_Chn3_Unused4,
    APP_eReg_Chn3_Unused3,
    APP_eReg_Chn3_Unused2,
    APP_eReg_Chn3_Unused1,
    APP_eReg_Chn3_Unused0,

    APP_eReg_Chn4_SwitchCurrent,
    APP_eReg_Chn4_SwitchSetPoint,
    APP_eReg_Chn4_ThresholdOff,
    APP_eReg_Chn4_ThresholdOn,
    APP_eReg_Chn4_Mode,
    APP_eReg_Chn4_TimeOn0_Weekday,
    APP_eReg_Chn4_TimeOn0_Hour,
    APP_eReg_Chn4_TimeOn0_Minute,
    APP_eReg_Chn4_TimeOff0_Weekday,
    APP_eReg_Chn4_TimeOff0_Hour,
    APP_eReg_Chn4_TimeOff0_Minute,
    APP_eReg_Chn4_TimeOn1_Weekday,
    APP_eReg_Chn4_TimeOn1_Hour,
    APP_eReg_Chn4_TimeOn1_Minute,
    APP_eReg_Chn4_TimeOff1_Weekday,
    APP_eReg_Chn4_TimeOff1_Hour,
    APP_eReg_Chn4_TimeOff1_Minute,
    APP_eReg_Chn4_TimeOn2_Weekday,
    APP_eReg_Chn4_TimeOn2_Hour,
    APP_eReg_Chn4_TimeOn2_Minute,
    APP_eReg_Chn4_TimeOff2_Weekday,
    APP_eReg_Chn4_TimeOff2_Hour,
    APP_eReg_Chn4_TimeOff2_Minute,
    APP_eReg_Chn4_TimeOn3_Weekday,
    APP_eReg_Chn4_TimeOn3_Hour,
    APP_eReg_Chn4_TimeOn3_Minute,
    APP_eReg_Chn4_TimeOff3_Weekday,
    APP_eReg_Chn4_TimeOff3_Hour,
    APP_eReg_Chn4_TimeOff3_Minute,
    APP_eReg_Chn4_Unused10,
    APP_eReg_Chn4_Unused9,
    APP_eReg_Chn4_Unused8,
    APP_eReg_Chn4_Unused7,
    APP_eReg_Chn4_Unused6,
    APP_eReg_Chn4_Unused5,
    APP_eReg_Chn4_Unused4,
    APP_eReg_Chn4_Unused3,
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
    APP_eCfg_Chn0_TimeOn0_Weekday,
    APP_eCfg_Chn0_TimeOn0_Hour,
    APP_eCfg_Chn0_TimeOn0_Minute,
    APP_eCfg_Chn0_TimeOff0_Weekday,
    APP_eCfg_Chn0_TimeOff0_Hour,
    APP_eCfg_Chn0_TimeOff0_Minute,
    APP_eCfg_Chn0_TimeOn1_Weekday,
    APP_eCfg_Chn0_TimeOn1_Hour,
    APP_eCfg_Chn0_TimeOn1_Minute,
    APP_eCfg_Chn0_TimeOff1_Weekday,
    APP_eCfg_Chn0_TimeOff1_Hour,
    APP_eCfg_Chn0_TimeOff1_Minute,
    APP_eCfg_Chn0_TimeOn2_Weekday,
    APP_eCfg_Chn0_TimeOn2_Hour,
    APP_eCfg_Chn0_TimeOn2_Minute,
    APP_eCfg_Chn0_TimeOff2_Weekday,
    APP_eCfg_Chn0_TimeOff2_Hour,
    APP_eCfg_Chn0_TimeOff2_Minute,
    APP_eCfg_Chn0_TimeOn3_Weekday,
    APP_eCfg_Chn0_TimeOn3_Hour,
    APP_eCfg_Chn0_TimeOn3_Minute,
    APP_eCfg_Chn0_TimeOff3_Weekday,
    APP_eCfg_Chn0_TimeOff3_Hour,
    APP_eCfg_Chn0_TimeOff3_Minute,
    APP_eCfg_Chn0_Unused4,
    APP_eCfg_Chn0_Unused3,
    APP_eCfg_Chn0_Unused2,
    APP_eCfg_Chn0_Unused1,
    APP_eCfg_Chn0_Unused0,

    APP_eCfg_Chn1_ThresholdOff,
    APP_eCfg_Chn1_ThresholdOn,
    APP_eCfg_Chn1_Mode,
    APP_eCfg_Chn1_TimeOn0_Weekday,
    APP_eCfg_Chn1_TimeOn0_Hour,
    APP_eCfg_Chn1_TimeOn0_Minute,
    APP_eCfg_Chn1_TimeOff0_Weekday,
    APP_eCfg_Chn1_TimeOff0_Hour,
    APP_eCfg_Chn1_TimeOff0_Minute,
    APP_eCfg_Chn1_TimeOn1_Weekday,
    APP_eCfg_Chn1_TimeOn1_Hour,
    APP_eCfg_Chn1_TimeOn1_Minute,
    APP_eCfg_Chn1_TimeOff1_Weekday,
    APP_eCfg_Chn1_TimeOff1_Hour,
    APP_eCfg_Chn1_TimeOff1_Minute,
    APP_eCfg_Chn1_TimeOn2_Weekday,
    APP_eCfg_Chn1_TimeOn2_Hour,
    APP_eCfg_Chn1_TimeOn2_Minute,
    APP_eCfg_Chn1_TimeOff2_Weekday,
    APP_eCfg_Chn1_TimeOff2_Hour,
    APP_eCfg_Chn1_TimeOff2_Minute,
    APP_eCfg_Chn1_TimeOn3_Weekday,
    APP_eCfg_Chn1_TimeOn3_Hour,
    APP_eCfg_Chn1_TimeOn3_Minute,
    APP_eCfg_Chn1_TimeOff3_Weekday,
    APP_eCfg_Chn1_TimeOff3_Hour,
    APP_eCfg_Chn1_TimeOff3_Minute,
    APP_eCfg_Chn1_Unused4,
    APP_eCfg_Chn1_Unused3,
    APP_eCfg_Chn1_Unused2,
    APP_eCfg_Chn1_Unused1,
    APP_eCfg_Chn1_Unused0,

    APP_eCfg_Chn2_ThresholdOff,
    APP_eCfg_Chn2_ThresholdOn,
    APP_eCfg_Chn2_Mode,
    APP_eCfg_Chn2_TimeOn0_Weekday,
    APP_eCfg_Chn2_TimeOn0_Hour,
    APP_eCfg_Chn2_TimeOn0_Minute,
    APP_eCfg_Chn2_TimeOff0_Weekday,
    APP_eCfg_Chn2_TimeOff0_Hour,
    APP_eCfg_Chn2_TimeOff0_Minute,
    APP_eCfg_Chn2_TimeOn1_Weekday,
    APP_eCfg_Chn2_TimeOn1_Hour,
    APP_eCfg_Chn2_TimeOn1_Minute,
    APP_eCfg_Chn2_TimeOff1_Weekday,
    APP_eCfg_Chn2_TimeOff1_Hour,
    APP_eCfg_Chn2_TimeOff1_Minute,
    APP_eCfg_Chn2_TimeOn2_Weekday,
    APP_eCfg_Chn2_TimeOn2_Hour,
    APP_eCfg_Chn2_TimeOn2_Minute,
    APP_eCfg_Chn2_TimeOff2_Weekday,
    APP_eCfg_Chn2_TimeOff2_Hour,
    APP_eCfg_Chn2_TimeOff2_Minute,
    APP_eCfg_Chn2_TimeOn3_Weekday,
    APP_eCfg_Chn2_TimeOn3_Hour,
    APP_eCfg_Chn2_TimeOn3_Minute,
    APP_eCfg_Chn2_TimeOff3_Weekday,
    APP_eCfg_Chn2_TimeOff3_Hour,
    APP_eCfg_Chn2_TimeOff3_Minute,
    APP_eCfg_Chn2_Unused4,
    APP_eCfg_Chn2_Unused3,
    APP_eCfg_Chn2_Unused2,
    APP_eCfg_Chn2_Unused1,
    APP_eCfg_Chn2_Unused0,

    APP_eCfg_Chn3_ThresholdOff,
    APP_eCfg_Chn3_ThresholdOn,
    APP_eCfg_Chn3_Mode,
    APP_eCfg_Chn3_TimeOn0_Weekday,
    APP_eCfg_Chn3_TimeOn0_Hour,
    APP_eCfg_Chn3_TimeOn0_Minute,
    APP_eCfg_Chn3_TimeOff0_Weekday,
    APP_eCfg_Chn3_TimeOff0_Hour,
    APP_eCfg_Chn3_TimeOff0_Minute,
    APP_eCfg_Chn3_TimeOn1_Weekday,
    APP_eCfg_Chn3_TimeOn1_Hour,
    APP_eCfg_Chn3_TimeOn1_Minute,
    APP_eCfg_Chn3_TimeOff1_Weekday,
    APP_eCfg_Chn3_TimeOff1_Hour,
    APP_eCfg_Chn3_TimeOff1_Minute,
    APP_eCfg_Chn3_TimeOn2_Weekday,
    APP_eCfg_Chn3_TimeOn2_Hour,
    APP_eCfg_Chn3_TimeOn2_Minute,
    APP_eCfg_Chn3_TimeOff2_Weekday,
    APP_eCfg_Chn3_TimeOff2_Hour,
    APP_eCfg_Chn3_TimeOff2_Minute,
    APP_eCfg_Chn3_TimeOn3_Weekday,
    APP_eCfg_Chn3_TimeOn3_Hour,
    APP_eCfg_Chn3_TimeOn3_Minute,
    APP_eCfg_Chn3_TimeOff3_Weekday,
    APP_eCfg_Chn3_TimeOff3_Hour,
    APP_eCfg_Chn3_TimeOff3_Minute,
    APP_eCfg_Chn3_Unused4,
    APP_eCfg_Chn3_Unused3,
    APP_eCfg_Chn3_Unused2,
    APP_eCfg_Chn3_Unused1,
    APP_eCfg_Chn3_Unused0,

    APP_eCfg_Chn4_ThresholdOff,
    APP_eCfg_Chn4_ThresholdOn,
    APP_eCfg_Chn4_Mode,
    APP_eCfg_Chn4_TimeOn0_Weekday,
    APP_eCfg_Chn4_TimeOn0_Hour,
    APP_eCfg_Chn4_TimeOn0_Minute,
    APP_eCfg_Chn4_TimeOff0_Weekday,
    APP_eCfg_Chn4_TimeOff0_Hour,
    APP_eCfg_Chn4_TimeOff0_Minute,
    APP_eCfg_Chn4_TimeOn1_Weekday,
    APP_eCfg_Chn4_TimeOn1_Hour,
    APP_eCfg_Chn4_TimeOn1_Minute,
    APP_eCfg_Chn4_TimeOff1_Weekday,
    APP_eCfg_Chn4_TimeOff1_Hour,
    APP_eCfg_Chn4_TimeOff1_Minute,
    APP_eCfg_Chn4_TimeOn2_Weekday,
    APP_eCfg_Chn4_TimeOn2_Hour,
    APP_eCfg_Chn4_TimeOn2_Minute,
    APP_eCfg_Chn4_TimeOff2_Weekday,
    APP_eCfg_Chn4_TimeOff2_Hour,
    APP_eCfg_Chn4_TimeOff2_Minute,
    APP_eCfg_Chn4_TimeOn3_Weekday,
    APP_eCfg_Chn4_TimeOn3_Hour,
    APP_eCfg_Chn4_TimeOn3_Minute,
    APP_eCfg_Chn4_TimeOff3_Weekday,
    APP_eCfg_Chn4_TimeOff3_Hour,
    APP_eCfg_Chn4_TimeOff3_Minute,
    APP_eCfg_Chn4_Unused4,
    APP_eCfg_Chn4_Unused3,
    APP_eCfg_Chn4_Unused2,
    APP_eCfg_Chn4_Unused1,
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
