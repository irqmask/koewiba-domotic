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
 * @file    led2chn.c
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

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "prjtypes.h"
#include "pwm16bit2chn.h"

#include "led2chn.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection LED2CHN_PCBCONFIG
 * Configure pinout of two channel LED controller.
 * @{
 */
#ifndef LED2CHN_PCBCONFIG
 #define LED2CHN_PCBCONFIG  1
 //! Defines the DDR of a port to switch the power supply
 #define LED_POWER_DDR 	DDRC
 //! Defines the port to switch the power supply
 #define LED_POWER_PORT PORTC
 //! Defines the port bit to switch the power supply
 #define LED_POWER_BIT 	PC0

#endif // LED2CHN_PCBCONFIG
/** @} */

/**
 * @subsection LED2CHN_APPCONFIG
 * Configuration of led 2 channel module.
 * @{
 */
#ifndef LED2CHN_APPCONFIG
 #define LED2CHN_APPCONFIG

 
#endif // LED2CHN_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * LED controller specific initializations.
 */
void led2_init (void) 
{
    LED_POWER_DDR |= LED_POWER_BIT;
    LED_POWER_PORT &= ~LED_POWER_BIT;
    
    pwm16bit_channel_init(0);
    pwm16bit_channel_init(1);
    //TODO insert LED specific initializations here!
}

/**
 * LED controller background task.
 * 
 * Executed once per main loop cycle.
 */
void led2_background (void)
{
    //TODO insert application specific background routines here!
}
/** @} */
