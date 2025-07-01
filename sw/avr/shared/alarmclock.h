/**
 * @addtogroup ALARMCLOCK
 * @brief A module for managing timed alarms/events.
 * 
 * If you need to perform actions at a specific point in time, it can be 
 * controlled with this ALARMCLOCK module.
 * To keep track of time it uses the DATETIME module. 
 *
 * @{
 * @file    alarmclock.h
 * @brief   A module for managing timed alarms/events.
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

#ifndef _ALARMCLOCK_H_
#define _ALARMCLOCK_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

#define MINUTES_PER_WEEK    7*24*60

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void alarm_initialize(void);

void alarm_set(int8_t index, 
               uint8_t hour, uint8_t minute, uint8_t days_of_week_mask);

void alarm_get(int8_t index, 
               uint8_t *hour, uint8_t *minute, uint8_t *days_of_week_mask);

void alarm_set_hour(int8_t index, uint8_t hour);

uint8_t alarm_get_hour(int8_t index);

void alarm_set_minute(int8_t index, uint8_t minute);

uint8_t alarm_get_minute(int8_t index);

void alarm_set_data(int8_t index, uint8_t data);

bool alarm_get_data(int8_t index, uint8_t* p_data);

void alarm_set_days_of_week_mask(int8_t index, uint8_t dow_mask);

uint8_t alarm_get_days_of_week_mask(int8_t index);

void alarm_activate(int8_t index, bool alarm_on);

bool alarm_check(int8_t* index);

#endif // _ALARMCLOCK_H_
/** @} */
