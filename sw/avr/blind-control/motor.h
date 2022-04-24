/**
 * @addtogroup MOTOR
 * @brief Public interface of motor control.
 *
 * This module contains functions to control the motor relays.
 *
 * @{
 * @file    motor.h
 * @brief   This module contains functions to control the motor relays.
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
#ifndef _MOTOR_H_
#define _MOTOR_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! Mode of switching motor into CW / CCW movement.
 typedef enum motor_mode {
    eMOTOR_DURATION,    //!< motor output is switch on for the duration of motor turning.
    eMOTOR_PULSE,       //!< motor output is only pulsed 500ms for turning up/down.
    eMOTOR_MODE_LAST
} motor_mode_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void motor_up               (uint8_t index);

void motor_down             (uint8_t index);

void motor_stop             (uint8_t index);

bool motor_is_running       (uint8_t index);

void motor_set_mode         (uint8_t index, motor_mode_t mode);

void motors_initialize      (void);

void motors_background      (void);

#endif /* _MOTOR_H_ */
/** @} */
