/**
 * @addtogroup LED2_ATMEGA324P
 * @addtogroup LED_2_CHANNEL
 * @brief   Two channel LED controller. 
 * 
 * Controls two channels of PWM controlled LEDs and an additional output to 
 * switch the LED power-supply ON or OFF dependent on the used LED channel state.
 * This module makes use of the atmega324 16bit timer peripherals.
 * 
 * @{
 * @file    led2chn.h
 * @brief   Two channel LED controller.
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

#ifndef _LED2CHN_H_
#define _LED2CHN_H_

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

void led2_init (void);

void led2_background (void);

#endif /* _APPCONFIG_H_ */
/** @} */
