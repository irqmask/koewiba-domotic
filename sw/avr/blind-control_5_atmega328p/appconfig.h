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

    APP_eReg_B0_TimeUp1_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B0_TimeUp1_Hour,        //!< Hour when blind wents up.
    APP_eReg_B0_TimeUp1_Minute,      //!< Minute when blind wents up.
    APP_eReg_B0_TimeUp1_unused_1,
    APP_eReg_B0_TimeUp1_unused_0,
    APP_eReg_B0_TimeDown1_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B0_TimeDown1_Hour,      //!< Hour when blind wents down.
    APP_eReg_B0_TimeDown1_Minute,    //!< Minute when blind wents down.
    APP_eReg_B0_TimeDown1_unused_1,
    APP_eReg_B0_TimeDown1_unused_0,
    APP_eReg_B0_TimeUp2_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B0_TimeUp2_Hour,        //!< Hour when blind wents up.
    APP_eReg_B0_TimeUp2_Minute,      //!< Minute when blind wents up.
    APP_eReg_B0_TimeUp2_unused_1,
    APP_eReg_B0_TimeUp2_unused_0,
    APP_eReg_B0_TimeDown2_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B0_TimeDown2_Hour,      //!< Hour when blind wents down.
    APP_eReg_B0_TimeDown2_Minute,    //!< Minute when blind wents down.
    APP_eReg_B0_TimeDown2_unused_1,
    APP_eReg_B0_TimeDown2_unused_0,

    APP_eReg_B1_TimeUp1_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B1_TimeUp1_Hour,        //!< Hour when blind wents up.
    APP_eReg_B1_TimeUp1_Minute,      //!< Minute when blind wents up.
    APP_eReg_B1_TimeUp1_unused_1,
    APP_eReg_B1_TimeUp1_unused_0,
    APP_eReg_B1_TimeDown1_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B1_TimeDown1_Hour,      //!< Hour when blind wents down.
    APP_eReg_B1_TimeDown1_Minute,    //!< Minute when blind wents down.
    APP_eReg_B1_TimeDown1_unused_1,
    APP_eReg_B1_TimeDown1_unused_0,
    APP_eReg_B1_TimeUp2_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B1_TimeUp2_Hour,        //!< Hour when blind wents up.
    APP_eReg_B1_TimeUp2_Minute,      //!< Minute when blind wents up.
    APP_eReg_B1_TimeUp2_unused_1,
    APP_eReg_B1_TimeUp2_unused_0,
    APP_eReg_B1_TimeDown2_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B1_TimeDown2_Hour,      //!< Hour when blind wents down.
    APP_eReg_B1_TimeDown2_Minute,    //!< Minute when blind wents down.
    APP_eReg_B1_TimeDown2_unused_1,
    APP_eReg_B1_TimeDown2_unused_0,

    APP_eReg_B2_TimeUp1_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B2_TimeUp1_Hour,        //!< Hour when blind wents up.
    APP_eReg_B2_TimeUp1_Minute,      //!< Minute when blind wents up.
    APP_eReg_B2_TimeUp1_unused_1,
    APP_eReg_B2_TimeUp1_unused_0,
    APP_eReg_B2_TimeDown1_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B2_TimeDown1_Hour,      //!< Hour when blind wents down.
    APP_eReg_B2_TimeDown1_Minute,    //!< Minute when blind wents down.
    APP_eReg_B2_TimeDown1_unused_1,
    APP_eReg_B2_TimeDown1_unused_0,
    APP_eReg_B2_TimeUp2_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B2_TimeUp2_Hour,        //!< Hour when blind wents up.
    APP_eReg_B2_TimeUp2_Minute,      //!< Minute when blind wents up.
    APP_eReg_B2_TimeUp2_unused_1,
    APP_eReg_B2_TimeUp2_unused_0,
    APP_eReg_B2_TimeDown2_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B2_TimeDown2_Hour,      //!< Hour when blind wents down.
    APP_eReg_B2_TimeDown2_Minute,    //!< Minute when blind wents down.
    APP_eReg_B2_TimeDown2_unused_1,
    APP_eReg_B2_TimeDown2_unused_0,

    APP_eReg_B3_TimeUp1_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B3_TimeUp1_Hour,        //!< Hour when blind wents up.
    APP_eReg_B3_TimeUp1_Minute,      //!< Minute when blind wents up.
    APP_eReg_B3_TimeUp1_unused_1,
    APP_eReg_B3_TimeUp1_unused_0,
    APP_eReg_B3_TimeDown1_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B3_TimeDown1_Hour,      //!< Hour when blind wents down.
    APP_eReg_B3_TimeDown1_Minute,    //!< Minute when blind wents down.
    APP_eReg_B3_TimeDown1_unused_1,
    APP_eReg_B3_TimeDown1_unused_0,
    APP_eReg_B3_TimeUp2_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B3_TimeUp2_Hour,        //!< Hour when blind wents up.
    APP_eReg_B3_TimeUp2_Minute,      //!< Minute when blind wents up.
    APP_eReg_B3_TimeUp2_unuMOD_eReg_FirstAppSpecificsed_1,
    APP_eReg_B3_TimeUp2_unused_0,
    APP_eReg_B3_TimeDown2_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B3_TimeDown2_Hour,      //!< Hour when blind wents down.
    APP_eReg_B3_TimeDown2_Minute,    //!< Minute when blind wents down.
    APP_eReg_B3_TimeDown2_unused_1,
    APP_eReg_B3_TimeDown2_unused_0,

    APP_eReg_B4_TimeUp1_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B4_TimeUp1_Hour,        //!< Hour when blind wents up.
    APP_eReg_B4_TimeUp1_Minute,      //!< Minute when blind wents up.
    APP_eReg_B4_TimeUp1_unused_1,
    APP_eReg_B4_TimeUp1_unused_0,
    APP_eReg_B4_TimeDown1_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B4_TimeDown1_Hour,      //!< Hour when blind wents down.
    APP_eReg_B4_TimeDown1_Minute,    //!< Minute when blind wents down.
    APP_eReg_B4_TimeDown1_unused_1,
    APP_eReg_B4_TimeDown1_unused_0,
    APP_eReg_B4_TimeUp2_Weekday,     //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B4_TimeUp2_Hour,        //!< Hour when blind wents up.
    APP_eReg_B4_TimeUp2_Minute,      //!< Minute when blind wents up.
    APP_eReg_B4_TimeUp2_unused_1,
    APP_eReg_B4_TimeUp2_unused_0,
    APP_eReg_B4_TimeDown2_Weekday,   //!< Flags on which weekday this timer applies. Index flags with day-of-week
    APP_eReg_B4_TimeDown2_Hour,      //!< Hour when blind wents down.
    APP_eReg_B4_TimeDown2_Minute,    //!< Minute when blind wents down.
    APP_eReg_B4_TimeDown2_unused_1,
    APP_eReg_B4_TimeDown2_unused_0,

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
#define NUM_EEBYTES_PER_CHN (APP_eCfg_Chn0_Mode - APP_eCfg_Chn0_ThresholdOff + 1)

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
