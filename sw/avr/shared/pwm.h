/**
 * @addtogroup PWM
 * @brief Pulse width modulation driver.
 *
 * Contains functions to do software PWM on several outputs. The number of
 * outputs and which outputs to be used can be configured at compile time.
 *
 * Supports pcbconfig.h for individual PCB settings of the PWM pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    pwm.h
 * @brief   Pulse width modulation driver.
 *
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
#ifndef _PWM_H_
#define _PWM_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include "ucontroller.h"
#endif

#include "prjtypes.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection PWM_PCBCONFIG
 * Configure PWM pinout.
 * @{
 */
#ifndef PWM_PCBCONFIG
 #define PWM_PCBCONFIG      1
 #define PWM_CHN_PORT       PORTC
 #define PWM_CHN_DDR        DDRC
 #define PWM_CHN0_PIN       0
 #define PWM_CHN1_PIN       1
 #define PWM_CHN2_PIN       2

 #define PWMCOUNT           REG_TIMER2_TCNT
 #define PWMCOMPARE         REG_TIMER2_OCRA
 #define PWM_INTERRUPT      ISR(INTERRUPT_TIMER2_COMPA)
 #define PWM_MAX_COUNT      256
#endif // PWM_PCBCONFIG
/** @} */

/**
 * @subsection PWM_APPCONFIG
 * Configure setiings of the PWM driver. E.g. base frequency.
 * @{
 */
#ifndef PWM_APPCONFIG
 #define PWM_APPCONFIG      1
 #define PWM_NUM_CHANNELS   3
#endif // PWM_APPCONFIG
/** @} */



// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void                pwm_init               (void);

void                pwm_set                (uint8_t uChannel, uint8_t uValue);

void                pwm_update             (void);

#endif // _PWM_H_
/** @} */
