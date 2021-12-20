/**
 * @addtogroup DATETIME
 *
 * @{
 * @file    datetime.c
 * @brief   A clock module for date and time.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@web.de>
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

#include "prjtypes.h"
#include "datetime.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection DATETIME_APPCONFIG
 * Configure datetime module. E.g. application specific callbacks.
 * @{
 */
#ifndef DATETIME_APPCONFIG
 #define DATETIME_APPCONFIG 1
 #undef  DATETIME_APP_HAS_ON_MINUTE_FUNCTION
#endif // DATETIME_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

//! Number of days per month.
static const uint8_t g_days_per_month[12] = {31, 28, 31, 30, 31, 30,
                                             31, 31, 30, 31, 30, 31};

static datetime_t g_dtdata;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Increase month by one.
 */
static void dt_tick_month(void)
{
    g_dtdata.month++;
    if (g_dtdata.month > 12) {
        g_dtdata.month = 1;
        if (g_dtdata.year < 65535) g_dtdata.year++;
    }
}

/**
 * Increase day by one.
 */
static void dt_tick_day(void)
{
    uint8_t dpm = 0;

    g_dtdata.dow++;
    if (g_dtdata.dow > DT_SUNDAY) g_dtdata.dow = DT_MONDAY;

    if (g_dtdata.month > 0 && g_dtdata.month <= 12) {
        dpm = g_days_per_month[g_dtdata.month - 1];
    }

    // leap year?
    if (g_dtdata.month == 2 && g_dtdata.year % 4 == 0) dpm++;

    g_dtdata.day++;
    if (g_dtdata.day > dpm) {
        g_dtdata.day = 1;
        dt_tick_month();
    }
}

/**
 * Increase hour by one.
 *
 * @param[in]   cd      Pointer to clock data.
 */
static void dt_tick_hour(void)
{
    g_dtdata.hour++;
    if (g_dtdata.hour > 23) {
        g_dtdata.hour = 0;
        dt_tick_day();
    }
}

#ifdef DATETIME_APP_HAS_ON_MINUTE_FUNCTION
    extern void app_on_minute(void);
#endif

/**
 * Increase minute by one.
 */
static void dt_tick_minute(void)
{
    g_dtdata.minute++;
    if (g_dtdata.minute > 59) {
        g_dtdata.minute = 0;
        dt_tick_hour();
    }
    // minute and maybe hour and maybe day of week have changed,
    // check if an alarm for this time is scheduled.
#ifdef DATETIME_APP_HAS_ON_MINUTE_FUNCTION
    app_on_minute();
#endif
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize clock instance. Sets the clock to 0000-01-01 00:00:00.
 */
void dt_initialize(void)
{
    g_dtdata.year = 0;
    g_dtdata.month = 1;
    g_dtdata.day = 1;
    g_dtdata.hour = 0;
    g_dtdata.minute = 0;
    g_dtdata.second = 0;
    g_dtdata.dow = DT_MONDAY;
}

/**
 * Set date and time and store it in clock data. If one parameter is out of
 * range, all clock data won't be updated.
 *
 * @param[in]   year    Year to set. All possible values are valid.
 * @param[in]   month   Month to set. Possible values are 1..12.
 * @param[in]   day     Day to set. Possible values are 1..31.
 * @param[in]   hour    Hour to set. Possible values are 0..23.
 * @param[in]   minute  Minute to set. Possible values are 0..59.
 * @param[in]   second  Second to set. Possible values are 0..59.
 * @param[in]   dow     Day of week since sunday. Possible values are
 *                      DT_SUNDAY .. DT_SATURDAY.
 * @note Number of days per month and appropriate weekday is not checked here!
 */
void dt_set(uint16_t year, uint8_t month, uint8_t day,
            uint8_t hour, uint8_t minute, uint8_t second,
            dt_day_of_week_t dow)
{
    dt_set_year(year);
    dt_set_month(month);
    dt_set_day(day);
    dt_set_hour(hour);
    dt_set_minute(minute);
    dt_set_second(second);
    dt_set_day_of_week(dow);
}

/**
 * Get date and time. Most of the parameters are optional and may be NULL.
 *
 * @param[in]   cd      Pointer to clock data.
 * @param[out]  year    Year to set. All possible values are valid.
 * @param[out]  month   Month to set. Possible values are 1..12.
 * @param[out]  day     Day to set. Possible values are 1..31.
 * @param[out]  hour    Hour to set. Possible values are 0..23.
 * @param[out]  minute  Minute to set. Possible values are 0..59.
 * @param[out]  second  Second to set. Possible values are 0..59.
 * @param[out]  dow     Day of week. Possible values are
 *                      DT_SUNDAY .. DT_SATURDAY.
 */
void dt_get(uint16_t *year, uint8_t *month, uint8_t *day,
            uint8_t *hour, uint8_t *minute, uint8_t *second,
            dt_day_of_week_t *dow)
{
    uint16_t dummy16;
    uint8_t dummy8;

    if (year == NULL) year = &dummy16;
    if (month == NULL) month = &dummy8;
    if (day == NULL) day = &dummy8;
    if (hour == NULL) hour = &dummy8;
    if (minute == NULL) minute = &dummy8;
    if (second == NULL) second = &dummy8;
    if (dow == NULL) dow = (dt_day_of_week_t*)&dummy8;

    *year = g_dtdata.year;
    *month = g_dtdata.month;
    *day = g_dtdata.day;
    *hour = g_dtdata.hour;
    *minute = g_dtdata.minute;
    *second = g_dtdata.second;
    *dow = g_dtdata.dow;
}

/**
 * @returns current year.
 */
uint16_t dt_get_year()
{
    return g_dtdata.year;
}

/**
 * Sets the current year.
 * @param[in]   year    Year to set.
 */
void dt_set_year(uint16_t year)
{
    g_dtdata.year = year;
}

/**
 * @returns current month.
 */
uint8_t dt_get_month()
{
    return g_dtdata.month;
}

/**
 * Sets the current month.
 * @param[in]   month     Month to set.
 *
 * @note Number of days per month is not checked here!
 */
void dt_set_month(uint8_t month)
{
    if (month > 0 && month <= 12) {
        g_dtdata.month = month;
    }
}

/**
 * @returns current day of week.
 */
dt_day_of_week_t dt_get_day_of_week(void)
{
    return g_dtdata.dow;
}

/**
 * Sets the current day of week.
 * @param[in]   dow     Day of week.
 *
 * @note Appropriate weekday at year/month/day is not checked here! E.g. you could
 * set the 2002-04-30 to DT_FRIDAY, but is was indeed a DT_TUESDAY.
 */
void dt_set_day_of_week(dt_day_of_week_t dow)
{
    while (dow > DT_SATURDAY) dow -= 7; // sunday is 0, saturday is 6
    g_dtdata.dow = dow;
}

/**
 * @returns current day.
 */
uint8_t dt_get_day()
{
    return g_dtdata.day;
}

/**
 * Sets the current day.
 * @param[in]   day    Day to set.
 */
void dt_set_day(uint8_t day)
{
    if (day > 0 && day <= 31) {
        g_dtdata.day = day;
    }
}

/**
 * @returns current hour.
 */
uint8_t dt_get_hour()
{
    return g_dtdata.hour;
}

/**
 * Sets the current hour.
 * @param[in]   hour    Hour to set.
 */
void dt_set_hour(uint8_t hour)
{
    if (hour <= 23) {
        g_dtdata.hour = hour;
    }
}

/**
 * @returns current minute.
 */
uint8_t dt_get_minute()
{
    return g_dtdata.minute;
}

/**
 * Sets the current minute.
 * @param[in]   minute  Minute to set.
 */
void dt_set_minute(uint8_t minute)
{
    if (minute <= 59) {
        g_dtdata.minute = minute;
    }
}

/**
 * @returns current second.
 */
uint8_t dt_get_second()
{
    return g_dtdata.second;
}

/**
 * Sets the current second.
 * @param[in]   second  Second to set.
 */
void dt_set_second(uint8_t second)
{
    if (second <= 59) {
        g_dtdata.second = second;
    }
}

/**
 * Increase second by one. This function is mostly called by a timer.
 * @note If an alarm clock is implemented, the alarm functions will
 * be called in contexrt of this function!
 */
void dt_tick_second(void)
{
    g_dtdata.second++;
    if (g_dtdata.second > 59) {
        g_dtdata.second = 0;
        dt_tick_minute();
    }
}

/** @} */
