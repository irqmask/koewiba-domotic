/**
 * @addtogroup ROOMTHERMOSTAT
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "roomthermostat_atmega328p" project.
 * 
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "roomthermostat_atmega328p" project.
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

#define BUS_APPCONFIG       1
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE   100     //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

#define GDISPLAY_APPCONFIG  1
#define GDISP_RAM_PAGES     8
#define GDISP_WIDTH         132
#define GDISP_HEIGHT        64
#undef GDISP_TYPE_ST7565
#define GDISP_TYPE_SH1106   1

#define DATETIME_APPCONFIG  1
#define DATETIME_APP_HAS_ON_MINUTE_FUNCTION 1

#define ALARMCLOCK_APPCONFIG 1
#define ALARM_COUNT         4       //!< Number of programmable alarms.

#define TIMER_WAKEUP        1       //!< Timer shall wake the controller.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_TempCurrent = MOD_eReg_FirstAppSpecific, //!< Current temperature in 1/100K.
    APP_eReg_TempSetPoint,          //!< Wanted temperature in 1/100K.
    APP_eReg_TempOffset,            //!< Temperature sensor offset in 1/100K (signed)
    APP_eReg_WindowSensorModuleID,  //!< Module ID of module which delivers the window state.
    APP_eReg_WindowSensorReg,       //!< Register number of window state.

    APP_eReg_Mode,                  //!< Mode: 0=normal, 1=vaccation
    APP_eReg_TemperatureWindowOpen, //!< Temperature to set when a window is opened
    APP_eReg_TemperatureVaccation,  //!< Temperature to set when in mode vaccation

    APP_eReg_Temp1_Weekday,         //!< Timepoint 1: Weekday when to switch temperature
    APP_eReg_Temp1_Hour,            //!< Timepoint 1: Hour when to switch temperature
    APP_eReg_Temp1_Minute,          //!< Timepoint 1: Minute when to switch temperature
    APP_eReg_Temp1_Temperature,     //!< Timepoint 1: Temperature to set when timepoint is reached

    APP_eReg_Temp2_Weekday,         //!< Timepoint 2: Weekday when to switch temperature
    APP_eReg_Temp2_Hour,            //!< Timepoint 2: Hour when to switch temperature
    APP_eReg_Temp2_Minute,          //!< Timepoint 2: Minute when to switch temperature
    APP_eReg_Temp2_Temperature,     //!< Timepoint 2: Temperature to set when timepoint is reached

    APP_eReg_Temp3_Weekday,         //!< Timepoint 3: Weekday when to switch temperature
    APP_eReg_Temp3_Hour,            //!< Timepoint 3: Hour when to switch temperature
    APP_eReg_Temp3_Minute,          //!< Timepoint 3: Minute when to switch temperature
    APP_eReg_Temp3_Temperature,     //!< Timepoint 3: Temperature to set when timepoint is reached

    APP_eReg_Temp4_Weekday,         //!< Timepoint 4: Weekday when to switch temperature
    APP_eReg_Temp4_Hour,            //!< Timepoint 4: Hour when to switch temperature
    APP_eReg_Temp4_Minute,          //!< Timepoint 4: Minute when to switch temperature
    APP_eReg_Temp4_Temperature,     //!< Timepoint 4: Temperature to set when timepoint is reached

    APP_eReg_Temp5_Weekday,         //!< Timepoint 5: Weekday when to switch temperature
    APP_eReg_Temp5_Hour,            //!< Timepoint 5: Hour when to switch temperature
    APP_eReg_Temp5_Minute,          //!< Timepoint 5: Minute when to switch temperature
    APP_eReg_Temp5_Temperature,     //!< Timepoint 5: Temperature to set when timepoint is reached

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

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_Offset = MOD_eCfg_FirstAppSpecific, //!< emperature sensor offset in 1/100K (signed)
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
