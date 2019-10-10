/**
 * @addtogroup LED2_ATMEGA324P
 * @addtogroup PWM16BIT
 * @brief 16bit onboard PWM.
 *
 * @{
 * @file pwm16bit2chn.h
 * @brief 16bit onboard PWM.
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

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define PWM_MAX_CHANNEL 2   //!< Supported number of PWM channels

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void pwm16bit_init(void);

void pwm16bit_channel_init (uint8_t chn);

void pwm16bit_channel_set (uint8_t chn, uint16_t dutycycle);

/** @} */
