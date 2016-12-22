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

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void motor_initialize       (void);

void motor_up               (void);

void motor_down             (void);

void motor_stop             (void);

#endif /* _MOTOR_H_ */
/** @} */
