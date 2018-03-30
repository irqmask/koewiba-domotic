/**
 * @addtogroup DATETIME
 * @brief A clock module for date and time.
 *
 * @{
 * @file    datetime.h
 * @brief   A clock module for date and time.
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

#ifndef _DATETIME_H_
#define _DATETIME_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! Enumeration of days of week.
typedef enum _dayofweek_t {
    CLK_MONDAY,
    CLK_TUEDAY,
    CLK_WEDNESDAY,
    CLK_THURSDAY,
    CLK_FRIDAY,
    CLK_SATURDAY,
    CLK_SUNDAY
} dt_day_of_week_t;

//! structure to hold local date and time.
typedef struct _datetime_t {
    uint16_t    year;       //!< absolute year 1999=1999
    uint8_t     month;      //!< valid values for month 1..12 represeting Jan..Dec
    uint8_t     day;        //!< valid values for day 1..31
    uint8_t     hour;       //!< valid values for hour 0..23
    uint8_t     minute;     //!< valid values for minute 0..59
    uint8_t     second;     //!< valid values for second 0..59
    dt_day_of_week_t dow;   //!< representing current weekday.
} datetime_t;

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void dt_initialize(void);

void dt_set(uint16_t year, uint8_t month, uint8_t day,
               uint8_t hour, uint8_t minute, uint8_t second, 
               dt_day_of_week_t dow);

void dt_get(uint16_t *year, uint8_t *month, uint8_t *day,
               uint8_t *hour, uint8_t *minute, uint8_t *second,
               dt_day_of_week_t *dow);

uint16_t dt_get_year(void);

uint8_t dt_get_month(void);

uint8_t dt_get_day(void);

uint8_t dt_get_hour(void);

uint8_t dt_get_minute(void);

uint8_t dt_get_second(void);

dt_day_of_week_t dt_get_day_of_week(void);

void dt_tick_second(void);

#endif // _DATETIME_H_
/** @} */
