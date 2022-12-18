/**
 * @addtogroup SWITCH5_ATMEGA328S
 * @addtogroup APPLICATION
 * @brief Application module.
 *
 * @{
 * @file    application.h
 * @brief   Common header of application module.
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
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "appconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

extern uint8_t app_chn_mode[APP_NUM_CHANNEL];

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

uint8_t app_get_alarm_setpoint(uint8_t chn, uint8_t alarmidx);

void app_set_mode(uint8_t chn, uint8_t mode);

void app_send_state(uint8_t chn, uint8_t state);

void app_register_load(void);

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
