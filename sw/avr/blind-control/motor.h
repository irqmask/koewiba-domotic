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
