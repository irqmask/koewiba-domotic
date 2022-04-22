/**
 * @addtogroup HEATERVALVE_MOTOR
 * @brief Interface for heateralve's motor.
 *
 * @{
 * @file    motor.h
 * @brief   Interface for heateralve's motors.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
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

#ifndef _HEATERVALVE_MOTOR_H_
#define _HEATERVALVE_MOTOR_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void motor_initialize(void);

void motor_set(bool on, bool cw);

uint16_t motor_get_valve_pos(void);

bool motor_start_homing(void);

bool motor_start_move_pos(uint16_t pos);

void motor_background(void);

#endif // _HEATERVALVE_MOTOR_H_
/** @} */
