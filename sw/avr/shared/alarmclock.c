/**
 * @addtogroup ALARMCLOCK
 *
 * @{
 * @file    alarmclock.c
 * @brief   A module for managing timed alarms/events.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018 christian <irqmask@web.de>
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

// --- Include section ---------------------------------------------------------

#include "datetime.h"
#include "prjtypes.h"

#include "alarmclock.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection ALARMCLOCK_APPCONFIG
 * Configuration of alarmclock module.
 * @{
 */
#ifndef ALARMCLOCK_APPCONFIG
 #define ALARMCLOCK_APPCONFIG
 //! Maximum number of different alarms
 #define ALARM_COUNT     4
#endif // ALARMCLOCK_APPCONFIG
/** @} */

#define ALARM_ACTIVE (1<<7)

// --- Type definitions --------------------------------------------------------

//! The time and weekdays of the wake-times is stored in this structure.
typedef struct waketime {
    uint8_t hour;               //!< Hour to wake-up.
    uint8_t minute;             //!< Minute to wake-up.
    uint8_t days_of_week_mask;  //!< bitmask of days (of type 1<<dt_day_of_week_t)
                                //!< MSB (1<<7) selects if alarm is active 1 or 
                                //!< inactive 0.
    uint8_t add_data;           //!< Additional data that can be used, when alarm is executed.
} alarm_waketime_t;

// --- Local variables ---------------------------------------------------------

alarm_waketime_t g_waketimes[ALARM_COUNT];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize and reset wake times. All alarms will be deactivated.
 */
void alarm_initialize(void)
{
    int8_t index;
    
    for (index=0; index<ALARM_COUNT; index++) {
        g_waketimes[index].hour = 0;
        g_waketimes[index].minute = 0;
        g_waketimes[index].days_of_week_mask = 0;
    }
}

/**
 * Set a specific wake time and days of week.
 * 
 * @param[in]   index   Index of alarm to set.
 * @param[in]   hour    Wake, when this hour is reached.
 * @param[in]   minute  Wake, when this minute is reached.
 * @param[in]   days_of_week_mask Mask of days, when the alarm is active.
 */
void alarm_set(int8_t index, 
               uint8_t hour, uint8_t minute, uint8_t days_of_week_mask)
{
    if (index < 0 || index >= ALARM_COUNT ||
        hour > 23 || minute > 59) return;
    
    g_waketimes[index].hour = hour;
    g_waketimes[index].minute = minute;
    g_waketimes[index].days_of_week_mask = (days_of_week_mask & 0x7F);
}

/**
 * Get a specific wake time and days of week.
 */
void alarm_get(int8_t index, 
               uint8_t *hour, uint8_t *minute, uint8_t *days_of_week_mask)
{
    if (index < 0 || index >= ALARM_COUNT) return;
    if (hour != NULL) *hour = g_waketimes[index].hour;
    if (minute != NULL) *minute = g_waketimes[index].minute;
    if (days_of_week_mask != NULL) *days_of_week_mask = g_waketimes[index].days_of_week_mask;
}

/**
 * Set hour of alarm.
 */
void alarm_set_hour(int8_t index, uint8_t hour)
{
    if (index < 0 || index >= ALARM_COUNT || hour > 23) return;
    g_waketimes[index].hour = hour;
}

/** 
 * Get hour of alarm time.
 */
uint8_t alarm_get_hour(int8_t index)
{
    if (index < 0 || index >= ALARM_COUNT) return 0;
    return g_waketimes[index].hour;
}

/** 
 * Set minute of alarm time.
 */
void alarm_set_minute(int8_t index, uint8_t minute)
{
    if (index < 0 || index >= ALARM_COUNT || minute > 59) return;
    g_waketimes[index].minute = minute;
}

/** 
 * Get minute of alarm time.
 */
uint8_t alarm_get_minute(int8_t index)
{
    if (index < 0 || index >= ALARM_COUNT) return 0;
    return g_waketimes[index].minute;
}

/** 
 * Set data of alarm entry.
 */
void alarm_set_data(int8_t index, uint8_t data)
{
    if (index < 0 || index >= ALARM_COUNT) return;
    g_waketimes[index].add_data = data;
}

/**
 * Get data of alarm entry.
 */
bool alarm_get_data(int8_t index, uint8_t* p_data)
{
    if (index < 0 || index >= ALARM_COUNT) return false;
    *p_data = g_waketimes[index].add_data;
    return true;
}

/**
 * Set mask of days, when this alarm is active.
 */
void alarm_set_days_of_week_mask(int8_t index, uint8_t days_of_week_mask)
{
    if (index < 0 || index >= ALARM_COUNT) return;
    g_waketimes[index].days_of_week_mask = days_of_week_mask;
    
}

/** 
 * Get mask of days, when this alarm is active.
 */
uint8_t alarm_get_days_of_week_mask(int8_t index)
{
    if (index < 0 || index >= ALARM_COUNT) return 0;
    return g_waketimes[index].days_of_week_mask;
}

/**
 * Activate the alarm.
 * 
 * @param[in]   index       Index of alarm to activate
 * @param[in]   alarm_on    True = activate alarm, false = deactivate.
 */
void alarm_activate(int8_t index, bool alarm_on)
{
    if (index <0 || index >= ALARM_COUNT) return;
    if (alarm_on) {
        g_waketimes[index].days_of_week_mask |= ALARM_ACTIVE;
    } else {
        g_waketimes[index].days_of_week_mask &= ~ALARM_ACTIVE;
    }
}

/** 
 * Check if one alarm is active.
 * Call this function only once per minute!
 * 
 * @param[out]  index   Index of alarm which occurred.
 * @returns true, alarm occurred.
 */
bool alarm_check(int8_t* index)
{
    int8_t ii;
    uint8_t dow;
    
    for (ii=0; ii<ALARM_COUNT; ii++) {
        if ((g_waketimes[ii].days_of_week_mask & ALARM_ACTIVE) == 0) continue;
        dow = dt_get_day_of_week();
        if (((1<<dow) & g_waketimes[ii].days_of_week_mask) == 0) continue;
        if (g_waketimes[ii].hour == dt_get_hour() &&
            g_waketimes[ii].minute == dt_get_minute()) {
            *index = ii;
            return true;
        }
    }
    return false;
}

/** @} */
