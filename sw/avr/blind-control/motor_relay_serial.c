/**
 * @addtogroup MOTOR
 * @brief This module contains functions to control the motor relays over 74595
 * shift registers.
 *
 * @{
 * @file    motor_relay_serial.c
 * @brief   This module contains functions to control the motor relays over
 *          74595 shift registers.
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

#include <stdbool.h>

#include "sn74595.h"
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
 #define MOTOR_0_ONOFF      {0x00, 0x01}
 #define MOTOR_1_ONOFF      {0x00, 0x04}
 #define MOTOR_2_ONOFF      {0x00, 0x10}
 #define MOTOR_3_ONOFF      {0x00, 0x40}
 #define MOTOR_4_ONOFF      {0x01, 0x00}
 #define MOTOR_5_ONOFF      {0x04, 0x00}
 #define MOTOR_6_ONOFF      {0x10, 0x00}
 #define MOTOR_7_ONOFF      {0x40, 0x00}

 #define MOTOR_0_UPDOWN     {0x00, 0x02}
 #define MOTOR_1_UPDOWN     {0x00, 0x08}
 #define MOTOR_2_UPDOWN     {0x00, 0x20}
 #define MOTOR_3_UPDOWN     {0x00, 0x80}
 #define MOTOR_4_UPDOWN     {0x02, 0x00}
 #define MOTOR_5_UPDOWN     {0x08, 0x00}
 #define MOTOR_6_UPDOWN     {0x20, 0x00}
 #define MOTOR_7_UPDOWN     {0x80, 0x00}
//! Number of motors supported by the application
 #define MOTOR_COUNT 8
#endif

#if MOTOR_COUNT > 8
#error "MOTOR_COUNT too high, maximum number of eight blinds supported!"
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

//! Mask of port bits of shift registers where on/off relays are connected to
static const uint8_t g_motor_relay_onoff_mask[MOTOR_COUNT][2] = {
    MOTOR_0_ONOFF,
#if (MOTOR_COUNT > 1)
    MOTOR_1_ONOFF,
#endif
#if (MOTOR_COUNT > 2)
    MOTOR_2_ONOFF,
#endif
#if (MOTOR_COUNT > 3)
    MOTOR_3_ONOFF,
#endif
#if (MOTOR_COUNT > 4)
    MOTOR_4_ONOFF,
#endif
#if (MOTOR_COUNT > 5)
    MOTOR_5_ONOFF,
#endif
#if (MOTOR_COUNT > 6)
    MOTOR_6_ONOFF,
#endif
#if (MOTOR_COUNT > 7)
    MOTOR_7_ONOFF,
#endif
};

//! Mask of port bits of shift registers where up/down relays are connected to
static const uint8_t g_motor_relay_updown_mask[MOTOR_COUNT][2] = {
    MOTOR_0_UPDOWN,
#if (MOTOR_COUNT > 1)
    MOTOR_1_UPDOWN,
#endif
#if (MOTOR_COUNT > 2)
    MOTOR_2_UPDOWN,
#endif
#if (MOTOR_COUNT > 3)
    MOTOR_3_UPDOWN,
#endif
#if (MOTOR_COUNT > 4)
    MOTOR_4_UPDOWN,
#endif
#if (MOTOR_COUNT > 5)
    MOTOR_5_UPDOWN,
#endif
#if (MOTOR_COUNT > 6)
    MOTOR_6_UPDOWN,
#endif
#if (MOTOR_COUNT > 7)
    MOTOR_7_UPDOWN,
#endif
};

//! Shadow register of shift register data
static uint8_t g_motor_relay_shadow[2];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------


/**
 * Initialize motor relays.
 */
void motors_relay_initialize       (void)
{
    uint8_t i;
    uint8_t mask[MOTOR_COUNT] = {0,0};

    g_motor_relay_shadow[0] = 0;
    g_motor_relay_shadow[1] = 0;
    sn74595_initialize();
    for(i=0; i<MOTOR_COUNT; i++)
    {
        mask[0] |= g_motor_relay_onoff_mask[i][0] | g_motor_relay_updown_mask[i][0];
        mask[1] |= g_motor_relay_onoff_mask[i][1] | g_motor_relay_updown_mask[i][1];
    }
    sn74595_set_byte(0, g_motor_relay_shadow[0], mask[0]);
    sn74595_set_byte(1, g_motor_relay_shadow[1], mask[1]);
    sn74595_send();
    sn74595_OE_on();
}

void motor_relay_onoff(uint8_t index, bool onoff)
{
    if (index >= MOTOR_COUNT) return;

    if (onoff == true) {
        g_motor_relay_shadow[0] |= g_motor_relay_onoff_mask[index][0];
        g_motor_relay_shadow[1] |= g_motor_relay_onoff_mask[index][1];
    }
    else {
        g_motor_relay_shadow[0] &= ~g_motor_relay_onoff_mask[index][0];
        g_motor_relay_shadow[1] &= ~g_motor_relay_onoff_mask[index][1];
    }
    sn74595_set_byte(0, g_motor_relay_shadow[0], g_motor_relay_onoff_mask[index][0]);
    sn74595_set_byte(1, g_motor_relay_shadow[1], g_motor_relay_onoff_mask[index][1]);
    sn74595_send();
}

void motor_relay_updown(uint8_t index, bool updown)
{
    if (index >= MOTOR_COUNT) return;

    if (updown == true) {
        g_motor_relay_shadow[0] |= g_motor_relay_updown_mask[index][0];
        g_motor_relay_shadow[1] |= g_motor_relay_updown_mask[index][1];
    }
    else {
        g_motor_relay_shadow[0] &= ~g_motor_relay_updown_mask[index][0];
        g_motor_relay_shadow[1] &= ~g_motor_relay_updown_mask[index][1];
    }
    sn74595_set_byte(0, g_motor_relay_shadow[0], g_motor_relay_updown_mask[index][0]);
    sn74595_set_byte(1, g_motor_relay_shadow[1], g_motor_relay_updown_mask[index][1]);
    sn74595_send();
}


// --- Global functions --------------------------------------------------------

/** @} */
