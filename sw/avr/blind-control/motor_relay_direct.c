/**
 * @addtogroup MOTOR
 * @brief This module contains functions to control the motor relays by
 * controller port pins.
 *
 * @{
 * @file    motor_relay_direct.c
 * @brief   This module contains functions to control the motor relays by
 * controller port pins.
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
// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <stdbool.h>

#include "motor.h"

// --- Definitions -------------------------------------------------------------

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#ifndef MOTOR_PCBCONFIG
 #define MOTOR_PCBCONFIG    1

 //! Number of motors supported by the application
 #define MOTOR_COUNT 1
 #define MOTOR_0_UPDOWN_PORT    PORTC
 #define MOTOR_0_UPDOWN_DDR     DDRC
 #define MOTOR_0_ONOFF_PORT     PORTC
 #define MOTOR_0_ONOFF_DDR      DDRC
 //! Pin to control motor's up/down relay
 #define MOTOR_0_UPDOWN_PIN     PC0
 //! Pin to control motor's off/on relay
 #define MOTOR_0_ONOFF_PIN      PC1
 //! Delay time in milliseconds between switching on/off and up/down relay
 #define MOTOR_SWITCH_DELAY     50
#endif
#if MOTOR_COUNT > 8
#error "MOTOR_COUNT too high, maximum number of eight blinds supported!"
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void motor_relay_onoff(uint8_t index, bool onoff)
{
    if (onoff == true) {
        switch (index) {
        case 0: MOTOR_0_ONOFF_PORT |= (1<<MOTOR_0_ONOFF_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_ONOFF_PORT |= (1<<MOTOR_1_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_ONOFF_PORT |= (1<<MOTOR_2_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_ONOFF_PORT |= (1<<MOTOR_3_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_ONOFF_PORT |= (1<<MOTOR_4_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_ONOFF_PORT |= (1<<MOTOR_5_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_ONOFF_PORT |= (1<<MOTOR_6_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_ONOFF_PORT |= (1<<MOTOR_7_ONOFF_PIN); break;
    #endif
        default:
            break;
        }
    } else {
        switch (index) {
        case 0: MOTOR_0_ONOFF_PORT &= ~(1<<MOTOR_0_ONOFF_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_ONOFF_PORT &= ~(1<<MOTOR_1_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_ONOFF_PORT &= ~(1<<MOTOR_2_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_ONOFF_PORT &= ~(1<<MOTOR_3_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_ONOFF_PORT &= ~(1<<MOTOR_4_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_ONOFF_PORT &= ~(1<<MOTOR_5_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_ONOFF_PORT &= ~(1<<MOTOR_6_ONOFF_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_ONOFF_PORT &= ~(1<<MOTOR_7_ONOFF_PIN); break;
    #endif
        default:
            break;
        }
    }
}

void motor_relay_updown(uint8_t index, bool updown)
{
    if (updown == true) {
        switch (index) {
        case 0: MOTOR_0_UPDOWN_PORT |= (1<<MOTOR_0_UPDOWN_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_UPDOWN_PORT |= (1<<MOTOR_1_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_UPDOWN_PORT |= (1<<MOTOR_2_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_UPDOWN_PORT |= (1<<MOTOR_3_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_UPDOWN_PORT |= (1<<MOTOR_4_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_UPDOWN_PORT |= (1<<MOTOR_5_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_UPDOWN_PORT |= (1<<MOTOR_6_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_UPDOWN_PORT |= (1<<MOTOR_7_UPDOWN_PIN); break;
    #endif
        default:
            break;
        }
    } else {
        switch (index) {
        case 0: MOTOR_0_UPDOWN_PORT &= ~(1<<MOTOR_0_UPDOWN_PIN); break;
    #if MOTOR_COUNT > 1
        case 1: MOTOR_1_UPDOWN_PORT &= ~(1<<MOTOR_1_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 2
        case 2: MOTOR_2_UPDOWN_PORT &= ~(1<<MOTOR_2_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 3
        case 3: MOTOR_3_UPDOWN_PORT &= ~(1<<MOTOR_3_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 4
        case 4: MOTOR_4_UPDOWN_PORT &= ~(1<<MOTOR_4_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 5
        case 5: MOTOR_5_UPDOWN_PORT &= ~(1<<MOTOR_5_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 6
        case 6: MOTOR_6_UPDOWN_PORT &= ~(1<<MOTOR_6_UPDOWN_PIN); break;
    #endif
    #if MOTOR_COUNT > 7
        case 7: MOTOR_7_UPDOWN_PORT &= ~(1<<MOTOR_7_UPDOWN_PIN); break;
    #endif
        default:
            break;
        }
    }
}

/**
 * Initialize motor module. Configure output pins.
 */
void motors_relay_initialize       (void)
{
    for (uint8_t index=0; index<MOTOR_COUNT; index++) {
        switch (index) {
        case 0:
            MOTOR_0_ONOFF_DDR |= (1<<MOTOR_0_ONOFF_PIN);
            MOTOR_0_ONOFF_PORT &= ~(1<<MOTOR_0_ONOFF_PIN);
            MOTOR_0_UPDOWN_DDR |= (1<<MOTOR_0_UPDOWN_PIN);
            MOTOR_0_UPDOWN_PORT &= ~(1<<MOTOR_0_UPDOWN_PIN);
            break;
    #if MOTOR_COUNT > 1
        case 1:
            MOTOR_1_ONOFF_DDR |= (1<<MOTOR_1_ONOFF_PIN);
            MOTOR_1_ONOFF_PORT &= ~(1<<MOTOR_1_ONOFF_PIN);
            MOTOR_1_UPDOWN_DDR |= (1<<MOTOR_1_UPDOWN_PIN);
            MOTOR_1_UPDOWN_PORT &= ~(1<<MOTOR_1_UPDOWN_PIN);
           break;
    #endif
    #if MOTOR_COUNT > 2
        case 2:
            MOTOR_2_ONOFF_DDR |= (1<<MOTOR_2_ONOFF_PIN);
            MOTOR_2_ONOFF_PORT &= ~(1<<MOTOR_2_ONOFF_PIN);
            MOTOR_2_UPDOWN_DDR |= (1<<MOTOR_2_UPDOWN_PIN);
            MOTOR_2_UPDOWN_PORT &= ~(1<<MOTOR_2_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 3
        case 3:
            MOTOR_3_ONOFF_DDR |= (1<<MOTOR_3_ONOFF_PIN);
            MOTOR_3_ONOFF_PORT &= ~(1<<MOTOR_3_ONOFF_PIN);
            MOTOR_3_UPDOWN_DDR |= (1<<MOTOR_3_UPDOWN_PIN);
            MOTOR_3_UPDOWN_PORT &= ~(1<<MOTOR_3_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 4
        case 4:
            MOTOR_4_ONOFF_DDR |= (1<<MOTOR_4_ONOFF_PIN);
            MOTOR_4_ONOFF_PORT &= ~(1<<MOTOR_4_ONOFF_PIN);
            MOTOR_4_UPDOWN_DDR |= (1<<MOTOR_4_UPDOWN_PIN);
            MOTOR_4_UPDOWN_PORT &= ~(1<<MOTOR_4_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 5
        case 5:
            MOTOR_5_ONOFF_DDR |= (1<<MOTOR_5_ONOFF_PIN);
            MOTOR_5_ONOFF_PORT &= ~(1<<MOTOR_5_ONOFF_PIN);
            MOTOR_5_UPDOWN_DDR |= (1<<MOTOR_5_UPDOWN_PIN);
            MOTOR_5_UPDOWN_PORT &= ~(1<<MOTOR_5_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 6
        case 6:
            MOTOR_6_ONOFF_DDR |= (1<<MOTOR_6_ONOFF_PIN);
            MOTOR_6_ONOFF_PORT &= ~(1<<MOTOR_6_ONOFF_PIN);
            MOTOR_6_UPDOWN_DDR |= (1<<MOTOR_6_UPDOWN_PIN);
            MOTOR_6_UPDOWN_PORT &= ~(1<<MOTOR_6_UPDOWN_PIN);
            break;
    #endif
    #if MOTOR_COUNT > 7
        case 7:
            MOTOR_7_ONOFF_DDR |= (1<<MOTOR_7_ONOFF_PIN);
            MOTOR_7_ONOFF_PORT &= ~(1<<MOTOR_7_ONOFF_PIN);
            MOTOR_7_UPDOWN_DDR |= (1<<MOTOR_7_UPDOWN_PIN);
            MOTOR_7_UPDOWN_PORT &= ~(1<<MOTOR_7_UPDOWN_PIN);
            break;
    #endif
        default:
            break;
        }
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
