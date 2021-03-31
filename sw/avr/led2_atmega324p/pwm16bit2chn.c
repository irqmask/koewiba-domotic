/**
 * @addtogroup LED2_ATMEGA324P
 * @addtogroup PWM16BIT
 * @brief 16bit on-board PWM.
 *
 * @{
 * @file pwm16bit2chn.c
 * @brief 16bit on-board PWM.
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

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize PWM.
 *
 * Fast PWM TOP=ICRn
 * WGM13    WGM12   WGM11    WGM10
 *     1        1       1        0
 *
 * Prescaler = 1024
 *  CS12    CS11    CS10
 *     0       1       0
 */
void pwm16bit_init(void)
{
   TCCR1A = (1 << WGM11);
   TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
   ICR1H = 0xFF;
   ICR1L = 0xFF;
}

/**
 * Initialize PWM channel.
 *
 * Clear output on compare match.
 * @param[in] chn Channel number starting at 0.
 */
void pwm16bit_channel_init (uint8_t chn) 
{
    switch (chn) {
    case 0:
        TCCR1A |= (1 << COM1A1);
        break;
    case 1:
        TCCR1A |= (1 << COM1B1);
        break;
    default:
        break;
    }
}

/**
 * Set PWM duty cycle.
 * @param[in] chn Channel number starting at 0.
 * @param[in] dutycycle Duty cycle
 */
void pwm16bit_channel_set (uint8_t chn, uint16_t dutycycle)
{
    switch (chn) {
    case 0:
        OCR1AH = (dutycycle >> 8);
        OCR1AL = (dutycycle & 0x00FF);
        break;
    case 1:
        OCR1BH = (dutycycle >> 8);
        OCR1BL = (dutycycle & 0x00FF);
        break;
    default:
        break;
    }
}

/** @} */
