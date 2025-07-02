/**
 * @addtogroup BLINDCONTROL_5_ATMEGA328
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "blindcontrol_5_atmega328p" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "blindcontrol_5_atmega328p" project.
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
#define BLIND_COUNT         5       //!< Number of blind which this software is capable to control.
#define OUTPUT_COUNT        6       //!< Number of output channels which this software is capable to control.

#define DATETIME_APPCONFIG  1
#define DATETIME_APP_HAS_ON_MINUTE_FUNCTION 1

#define TIMER_WAKEUP		1

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
    APP_eReg_B0_Last = APP_eReg_B0_Unused1,

    APP_eReg_B1_PositionCurrent,   //!< Current position of blind
    APP_eReg_B1_PositionSetPoint,  //!< Wanted position of blind. 100 = completely closed. 0 = completely open
    APP_eReg_B1_PositionDifference,//!< Difference between wanted and current position.
    APP_eReg_B1_ReactionDelay,     //!< Delay time in timer ticks (1/100sec) until movement starts after applying power.
    APP_eReg_B1_DurationOpen,      //!< Duration in timer ticks (1/100sec) to fully open the blind.
    APP_eReg_B1_DurationClose,     //!< Duration in timer ticks (1/100sec) to fully close the blind.
    APP_eReg_B1_Mode,              //!< Mode how the blind/motor is controlled.
    APP_eReg_B1_Unused3,           //!< unused
    APP_eReg_B1_Unused2,           //!< unused
    APP_eReg_B1_Unused1,           //!< unused
    APP_eReg_B1_Last = APP_eReg_B1_Unused1,

    APP_eReg_B2_PositionCurrent,   //!< Current position of blind
    APP_eReg_B2_PositionSetPoint,  //!< Wanted position of blind. 100 = completely closed. 0 = completely open
    APP_eReg_B2_PositionDifference,//!< Difference between wanted and current position.
    APP_eReg_B2_ReactionDelay,     //!< Delay time in timer ticks (1/100sec) until movement starts after applying power.
    APP_eReg_B2_DurationOpen,      //!< Duration in timer ticks (1/100sec) to fully open the blind.
    APP_eReg_B2_DurationClose,     //!< Duration in timer ticks (1/100sec) to fully close the blind.
    APP_eReg_B2_Mode,              //!< Mode how the blind/motor is controlled.
    APP_eReg_B2_Unused3,           //!< unused
    APP_eReg_B2_Unused2,           //!< unused
    APP_eReg_B2_Unused1,           //!< unused
    APP_eReg_B2_Last = APP_eReg_B2_Unused1,

    APP_eReg_B3_PositionCurrent,   //!< Current position of blind
    APP_eReg_B3_PositionSetPoint,  //!< Wanted position of blind. 100 = completely closed. 0 = completely open
    APP_eReg_B3_PositionDifference,//!< Difference between wanted and current position.
    APP_eReg_B3_ReactionDelay,     //!< Delay time in timer ticks (1/100sec) until movement starts after applying power.
    APP_eReg_B3_DurationOpen,      //!< Duration in timer ticks (1/100sec) to fully open the blind.
    APP_eReg_B3_DurationClose,     //!< Duration in timer ticks (1/100sec) to fully close the blind.
    APP_eReg_B3_Mode,              //!< Mode how the blind/motor is controlled.
    APP_eReg_B3_Unused3,           //!< unused
    APP_eReg_B3_Unused2,           //!< unused
    APP_eReg_B3_Unused1,           //!< unused
    APP_eReg_B3_Last = APP_eReg_B3_Unused1,

    APP_eReg_B4_PositionCurrent,   //!< Current position of blind
    APP_eReg_B4_PositionSetPoint,  //!< Wanted position of blind. 100 = completely closed. 0 = completely open
    APP_eReg_B4_PositionDifference,//!< Difference between wanted and current position.
    APP_eReg_B4_ReactionDelay,     //!< Delay time in timer ticks (1/100sec) until movement starts after applying power.
    APP_eReg_B4_DurationOpen,      //!< Duration in timer ticks (1/100sec) to fully open the blind.
    APP_eReg_B4_DurationClose,     //!< Duration in timer ticks (1/100sec) to fully close the blind.
    APP_eReg_B4_Mode,              //!< Mode how the blind/motor is controlled.
    APP_eReg_B4_Unused3,           //!< unused
    APP_eReg_B4_Unused2,           //!< unused
    APP_eReg_B4_Unused1,           //!< unused
    APP_eReg_B4_Last = APP_eReg_B4_Unused1,

    APP_eReg_MasterExclusionFlags,         //!< Action is not executed when: Bit0=Public holiday, Bit1=..., Bit7=General.
    APP_eReg_BlindEventSunriseOffset,      //!< Minutes that are added to the event time in the morning when offset is activated.
    APP_eReg_BlindEventSunsetOffset,       //!< Minutes that are subtracted from the event time in the evening when offset is activated.
    App_eReg_BlindEvent_Unused,
    APP_eReg_BlindEvent0_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent0_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent0_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent0_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent0_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent0_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent1_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent1_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent1_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent1_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent1_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent1_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent2_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent2_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent2_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent2_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent2_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent2_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent3_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent3_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent3_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent3_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent3_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent3_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent4_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent4_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent4_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent4_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent4_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent4_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent5_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent5_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent5_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent5_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent5_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent5_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent6_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent6_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent6_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent6_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent6_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent6_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent7_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent7_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent7_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent7_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent7_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent7_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent8_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent8_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent8_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent8_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent8_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent8_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent9_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent9_Hour,             //!< Hour when this timer applies.
    APP_eReg_BlindEvent9_Minute,           //!< Minute when this timer applies.
    APP_eReg_BlindEvent9_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent9_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent9_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent10_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent10_Hour,            //!< Hour when this timer applies.
    APP_eReg_BlindEvent10_Minute,          //!< Minute when this timer applies.
    APP_eReg_BlindEvent10_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent10_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent10_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent11_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent11_Hour,            //!< Hour when this timer applies.
    APP_eReg_BlindEvent11_Minute,          //!< Minute when this timer applies.
    APP_eReg_BlindEvent11_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent11_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent11_ExclusionFlags, //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent12_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent12_Hour,            //!< Hour when this timer applies.
    APP_eReg_BlindEvent12_Minute,          //!< Minute when this timer applies.
    APP_eReg_BlindEvent12_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent12_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent12_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent13_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent13_Hour,            //!< Hour when this timer applies.
    APP_eReg_BlindEvent13_Minute,          //!< Minute when this timer applies.
    APP_eReg_BlindEvent13_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent13_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent13_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent14_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent14_Hour,            //!< Hour when this timer applies.
    APP_eReg_BlindEvent14_Minute,          //!< Minute when this timer applies.
    APP_eReg_BlindEvent14_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eReg_BlindEvent14_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent14_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eReg_BlindEvent15_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eReg_BlindEvent15_Hour,            //!< Hour when this timer applies.
    APP_eReg_BlindEvent15_Minute,          //!< Minute when this timer applies.
    APP_eReg_BlindEvent15_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eReg_BlindEvent15_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eReg_BlindEvent15_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.

    APP_eReg_Chn0_SwitchCurrent,     //!< Current switch state (0...255)
    APP_eReg_Chn0_SwitchSetPoint,    //!< Target switch state (0...255)
    APP_eReg_Chn0_ThresholdOff,      //!< If value goes below, channel switches off
    APP_eReg_Chn0_ThresholdOn,       //!< If value goes above, channel switches on
    APP_eReg_Chn0_Mode,              //!< not used yet

    APP_eReg_Chn1_SwitchCurrent,     //!< Current switch state (0...255)
    APP_eReg_Chn1_SwitchSetPoint,    //!< Target switch state (0...255)
    APP_eReg_Chn1_ThresholdOff,      //!< If value goes below, channel switches off
    APP_eReg_Chn1_ThresholdOn,       //!< If value goes above, channel switches on
    APP_eReg_Chn1_Mode,              //!< not used yet

    APP_eReg_Chn2_SwitchCurrent,     //!< Current switch state (0...255)
    APP_eReg_Chn2_SwitchSetPoint,    //!< Target switch state (0...255)
    APP_eReg_Chn2_ThresholdOff,      //!< If value goes below, channel switches off
    APP_eReg_Chn2_ThresholdOn,       //!< If value goes above, channel switches on
    APP_eReg_Chn2_Mode,              //!< not used yet

    APP_eReg_Chn3_SwitchCurrent,     //!< Current switch state (0...255)
    APP_eReg_Chn3_SwitchSetPoint,    //!< Target switch state (0...255)
    APP_eReg_Chn3_ThresholdOff,      //!< If value goes below, channel switches off
    APP_eReg_Chn3_ThresholdOn,       //!< If value goes above, channel switches on
    APP_eReg_Chn3_Mode,              //!< not used yet

    APP_eReg_Chn4_SwitchCurrent,     //!< Current switch state (0...255)
    APP_eReg_Chn4_SwitchSetPoint,    //!< Target switch state (0...255)
    APP_eReg_Chn4_ThresholdOff,      //!< If value goes below, channel switches off
    APP_eReg_Chn4_ThresholdOn,       //!< If value goes above, channel switches on
    APP_eReg_Chn4_Mode,              //!< not used yet

    APP_eReg_Chn5_SwitchCurrent,     //!< Current switch state (0...255)
    APP_eReg_Chn5_SwitchSetPoint,    //!< Target switch state (0...255)
    APP_eReg_Chn5_ThresholdOff,      //!< If value goes below, channel switches off
    APP_eReg_Chn5_ThresholdOn,       //!< If value goes above, channel switches on
    APP_eReg_Chn5_Mode,              //!< not used yet

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
#define APP_NUM_REGS_PER_BLIND (APP_eReg_B0_Last - APP_eReg_B0_PositionCurrent + 1)
//! Number of registers per channel
#define APP_NUM_REGS_PER_CHN (APP_eReg_Chn0_Mode - APP_eReg_Chn0_SwitchCurrent + 1)
//! Number of registers per blind event
#define APP_NUM_REGS_PER_BLINDEVENT (APP_eReg_BlindEvent0_ExclusionFlags - APP_eReg_BlindEvent0_Weekday + 1)


//! Number of blind events
#define APP_NOF_BLINDEVENTS ((1+APP_eReg_BlindEvent15_ExclusionFlags-APP_eReg_BlindEvent0_Weekday) / APP_NUM_REGS_PER_BLINDEVENT)


//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_B0_ReactionDelay = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_B0_DurationOpen,
    APP_eCfg_B0_DurationOpenLow = APP_eCfg_B0_DurationOpen,
    APP_eCfg_B0_DurationOpenHigh,
    APP_eCfg_B0_DurationClose,
    APP_eCfg_B0_DurationCloseLow = APP_eCfg_B0_DurationClose,
    APP_eCfg_B0_DurationCloseHigh,
    APP_eCfg_B0_Mode,

    APP_eCfg_B1_ReactionDelay,
    APP_eCfg_B1_DurationOpen,
    APP_eCfg_B1_DurationOpenLow = APP_eCfg_B1_DurationOpen,
    APP_eCfg_B1_DurationOpenHigh,
    APP_eCfg_B1_DurationClose,
    APP_eCfg_B1_DurationCloseLow = APP_eCfg_B1_DurationClose,
    APP_eCfg_B1_DurationCloseHigh,
    APP_eCfg_B1_Mode,

    APP_eCfg_B2_ReactionDelay,
    APP_eCfg_B2_DurationOpen,
    APP_eCfg_B2_DurationOpenLow = APP_eCfg_B2_DurationOpen,
    APP_eCfg_B2_DurationOpenHigh,
    APP_eCfg_B2_DurationClose,
    APP_eCfg_B2_DurationCloseLow = APP_eCfg_B2_DurationClose,
    APP_eCfg_B2_DurationCloseHigh,
    APP_eCfg_B2_Mode,

    APP_eCfg_B3_ReactionDelay,
    APP_eCfg_B3_DurationOpen,
    APP_eCfg_B3_DurationOpenLow = APP_eCfg_B3_DurationOpen,
    APP_eCfg_B3_DurationOpenHigh,
    APP_eCfg_B3_DurationClose,
    APP_eCfg_B3_DurationCloseLow = APP_eCfg_B3_DurationClose,
    APP_eCfg_B3_DurationCloseHigh,
    APP_eCfg_B3_Mode,

    APP_eCfg_B4_ReactionDelay,
    APP_eCfg_B4_DurationOpen,
    APP_eCfg_B4_DurationOpenLow = APP_eCfg_B4_DurationOpen,
    APP_eCfg_B4_DurationOpenHigh,
    APP_eCfg_B4_DurationClose,
    APP_eCfg_B4_DurationCloseLow = APP_eCfg_B4_DurationClose,
    APP_eCfg_B4_DurationCloseHigh,
    APP_eCfg_B4_Mode,

    APP_eCfg_MasterExclusionFlags,         //!< Action is not executed when: Bit0=Public holiday, Bit1=..., Bit7=General.
    APP_eCfg_BlindEventSunriseOffset,      //!< Minutes that are added to the event time in the morning when offset is activated.
    APP_eCfg_BlindEventSunsetOffset,       //!< Minutes that are subtracted from the event time in the evening when offset is activated.
    APP_eCfg_BlindEvent0_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent0_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent0_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent0_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent0_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent0_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent1_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent1_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent1_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent1_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent1_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent1_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent2_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent2_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent2_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent2_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent2_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent2_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent3_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent3_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent3_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent3_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent3_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent3_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent4_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent4_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent4_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent4_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent4_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent4_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent5_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent5_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent5_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent5_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent5_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent5_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent6_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent6_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent6_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent6_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent6_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent6_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent7_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent7_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent7_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent7_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent7_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent7_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent8_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent8_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent8_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent8_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent8_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent8_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent9_Weekday,          //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent9_Hour,             //!< Hour when this timer applies.
    APP_eCfg_BlindEvent9_Minute,           //!< Minute when this timer applies.
    APP_eCfg_BlindEvent9_Blinds,           //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent9_PositionValue,    //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent9_ExclusionFlags,   //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent10_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent10_Hour,            //!< Hour when this timer applies.
    APP_eCfg_BlindEvent10_Minute,          //!< Minute when this timer applies.
    APP_eCfg_BlindEvent10_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent10_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent10_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent11_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent11_Hour,            //!< Hour when this timer applies.
    APP_eCfg_BlindEvent11_Minute,          //!< Minute when this timer applies.
    APP_eCfg_BlindEvent11_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent11_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent11_ExclusionFlags, //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent12_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent12_Hour,            //!< Hour when this timer applies.
    APP_eCfg_BlindEvent12_Minute,          //!< Minute when this timer applies.
    APP_eCfg_BlindEvent12_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent12_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent12_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent13_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent13_Hour,            //!< Hour when this timer applies.
    APP_eCfg_BlindEvent13_Minute,          //!< Minute when this timer applies.
    APP_eCfg_BlindEvent13_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent13_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent13_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent14_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent14_Hour,            //!< Hour when this timer applies.
    APP_eCfg_BlindEvent14_Minute,          //!< Minute when this timer applies.
    APP_eCfg_BlindEvent14_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4 .
    APP_eCfg_BlindEvent14_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent14_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.
    APP_eCfg_BlindEvent15_Weekday,         //!< Flags on which weekday this timer applies. Index flags with day-of-week.
    APP_eCfg_BlindEvent15_Hour,            //!< Hour when this timer applies.
    APP_eCfg_BlindEvent15_Minute,          //!< Minute when this timer applies.
    APP_eCfg_BlindEvent15_Blinds,          //!< Blinds involved in action. Bit0=Blind0, ..., Bit4=Blind4.
    APP_eCfg_BlindEvent15_PositionValue,   //!< Target position of blind. 100 = completely closed. 0 = completely open.
    APP_eCfg_BlindEvent15_ExclusionFlags,  //!< When one of these Flags in 'MasterExclusionFlag' is set, action won't be executed.

    APP_eCfg_Chn0_ThresholdOff,
    APP_eCfg_Chn0_ThresholdOn,
    APP_eCfg_Chn0_Mode,
    APP_eCfg_Chn1_ThresholdOff,
    APP_eCfg_Chn1_ThresholdOn,
    APP_eCfg_Chn1_Mode,
    APP_eCfg_Chn2_ThresholdOff,
    APP_eCfg_Chn2_ThresholdOn,
    APP_eCfg_Chn2_Mode,
    APP_eCfg_Chn3_ThresholdOff,
    APP_eCfg_Chn3_ThresholdOn,
    APP_eCfg_Chn3_Mode,
    APP_eCfg_Chn4_ThresholdOff,
    APP_eCfg_Chn4_ThresholdOn,
    APP_eCfg_Chn4_Mode,
    APP_eCfg_Chn5_ThresholdOff,
    APP_eCfg_Chn5_ThresholdOn,
    APP_eCfg_Chn5_Mode,

    // insert application specific configuration here
    APP_eCfg_Last
} eConfiguration_t;

//! Number of eeprom bytes per blind
#define NUM_EEBYTES_PER_BLIND (APP_eCfg_B1_ReactionDelay - APP_eCfg_B0_ReactionDelay)
//! Number of configuration registers per channel
#define NUM_EEBYTES_PER_CHN (APP_eCfg_Chn1_ThresholdOff - APP_eCfg_Chn0_ThresholdOff)
//! Number of eeprom bytes per blind event
#define NUM_EEBYTES_PER_BLINDEVENT (APP_eCfg_BlindEvent1_Weekday - APP_eCfg_BlindEvent0_Weekday)

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
