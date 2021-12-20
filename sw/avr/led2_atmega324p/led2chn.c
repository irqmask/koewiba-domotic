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
#include <avr/pgmspace.h>

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
 #define LED_POWER_DDR 	DDRB
 //! Defines the port to switch the power supply
 #define LED_POWER_PORT PORTB
 //! Defines the port bit to switch the power supply
 #define LED_POWER_BIT 	PB0
 //! Defines the DDR of a port of LED channel 0
 #define LED_CHN0_DDR  DDRD
 //! Defines the port of LED channel 0
 #define LED_CHN0_PORT PORTD
 //! Defines the port bit of LED channel 0
 #define LED_CHN0_BIT  PD5
 //! Defines the DDR of a port of LED channel 1
 #define LED_CHN1_DDR  DDRD
 //! Defines the port of LED channel 1
 #define LED_CHN1_PORT PORTD
 //! Defines the port bit of LED channel 1
 #define LED_CHN1_BIT  PD4

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

//! Timeout until LED power is switched off
#define LED_OFF_TIMEOUT 100

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

const uint16_t pwmtable[256] PROGMEM =
{
    0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7,
    7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 15,
    15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    31, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 49, 52, 54, 56, 59,
    61, 64, 67, 70, 73, 76, 79, 83, 87, 91, 95, 99, 103, 108, 112,
    117, 123, 128, 134, 140, 146, 152, 159, 166, 173, 181, 189, 197,
    206, 215, 225, 235, 245, 256, 267, 279, 292, 304, 318, 332, 347,
    362, 378, 395, 412, 431, 450, 470, 490, 512, 535, 558, 583, 609,
    636, 664, 693, 724, 756, 790, 825, 861, 899, 939, 981, 1024, 1069,
    1117, 1166, 1218, 1272, 1328, 1387, 1448, 1512, 1579, 1649, 1722,
    1798, 1878, 1961, 2048, 2139, 2233, 2332, 2435, 2543, 2656, 2773,
    2896, 3025, 3158, 3298, 3444, 3597, 3756, 3922, 4096, 4277, 4467,
    4664, 4871, 5087, 5312, 5547, 5793, 6049, 6317, 6596, 6889, 7194,
    7512, 7845, 8192, 8555, 8933, 9329, 9742, 10173, 10624, 11094,
    11585, 12098, 12634, 13193, 13777, 14387, 15024, 15689, 16384,
    17109, 17867, 18658, 19484, 20346, 21247, 22188, 23170, 24196,
    25267, 26386, 27554, 28774, 30048, 31378, 32768, 34218, 35733,
    37315, 38967, 40693, 42494, 44376, 46340, 48392, 50534, 52772,
    55108, 57548, 60096, 62757, 65535
};

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static uint8_t g_dutycycle[PWM_MAX_CHANNEL];
static uint8_t g_off_timeout = LED_OFF_TIMEOUT;

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * LED controller specific initializations.
 */
void led2_init (void) 
{
    LED_POWER_DDR |= (1<<LED_POWER_BIT);
    LED_POWER_PORT &= ~(1<<LED_POWER_BIT);
    
    g_dutycycle[0] = 0;
    g_dutycycle[1] = 0;
    g_off_timeout = LED_OFF_TIMEOUT;

    pwm16bit_init();
    pwm16bit_channel_init(0);
    pwm16bit_channel_init(1);

    LED_CHN0_DDR |= (1<<LED_CHN0_BIT);
    LED_CHN0_PORT &= ~(1<<LED_CHN0_BIT);
    LED_CHN1_DDR |= (1<<LED_CHN1_BIT);
    LED_CHN1_PORT &= ~(1<<LED_CHN1_BIT);

    pwm16bit_channel_set(0, g_dutycycle[0]);
    pwm16bit_channel_set(1, g_dutycycle[1]);
}

void led2_set_intensity (uint8_t channel, uint8_t intensity)
{
    if (channel > PWM_MAX_CHANNEL) return;
    g_dutycycle[channel] = intensity;
    if (g_dutycycle[0] == 0 && g_dutycycle[1] == 0) {
        g_off_timeout = LED_OFF_TIMEOUT;
    }
    else {
        g_off_timeout = 0;
        LED_POWER_PORT |= (1<<LED_POWER_BIT);
    }
    pwm16bit_channel_set(channel, pgm_read_word(&pwmtable[intensity]));
}

/**
 * LED controller background task.
 * 
 * Executed once per main loop cycle.
 */
void led2_background (void)
{
    if (g_off_timeout > 0) {
        g_off_timeout--;
        if (g_off_timeout == 0) LED_POWER_PORT &= ~(1<<LED_POWER_BIT);
    }
}
/** @} */
