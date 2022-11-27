 
/**
 * @addtogroup ROOMTHERMOSTAT
 * @addtogroup APPLICATION
 * @brief Declaration of application wide functions and variables
 *
 * @{
 * @file    application.h
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void app_set_desired_temp(uint16_t desired_temp, bool publish);

void app_set_temperature_offset(int16_t offset);

int16_t app_get_temperature_offset(void);

void app_set_windowcontact_moduleid(uint16_t moduleid);

uint16_t app_get_windowcontact_moduleid(void);

void app_set_windowcontact_reg(uint8_t reg);

uint8_t app_get_windowcontact_reg(void);

void app_set_mode(uint8_t mode);

uint8_t app_get_mode(void);

void app_set_windowopen_temperature(uint16_t temp);

uint16_t app_get_windowopen_temperature(void);

void app_set_vacation_temperature(uint16_t temp);

uint16_t app_get_vacation_temperature(void);

void app_draw_desired_temp(void);

void app_draw_current_temp(void);

void app_draw_window(void);

void app_register_load(void);

// --- Global functions --------------------------------------------------------

#endif /* _APPLICATION_H_ */
/** @} */
