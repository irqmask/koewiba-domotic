/**
 * @addtogroup SLEEPMODE
 * @brief Control of sleep modes.
 *
 * @{
 * @file    sleepmode.c
 * @brief   Control of sleep modes..
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2019  christian <irqmask@web.de>
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
// --- Include section ---------------------------------------------------------

#include <util/delay.h>

#include "led_debug.h"
#include "timer.h"

#include "sleepmode.h"

#include "pcbconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// Currently we don't need to save, if seeping was requested.
// There is no mechanism to detect, when sleeping is no longer wanted
uint8_t g_sleep_required = 0;
uint8_t g_sleep_prevent_mask = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void sleep_request(bool shall_sleep)
{
    g_sleep_required = shall_sleep;
}

/**
 * Prevent controller from going into sleep mode.
 * @param[in] prevent_mask  Bitmask to set or clear.
 * @param[in] on            != 0 set prevent mask == 0 clear prevent mask
 *
 * If all bits in prevent mask are cleared, we check, if sleeping is
 * required and go to sleep mode then..
 */
void sleep_prevent(uint8_t prevent_mask, uint8_t on)
{
    if (on != 0) {
        g_sleep_prevent_mask |= prevent_mask;
    } else {
        g_sleep_prevent_mask &= ~prevent_mask;
    }
}

/**
 * Check if the controller shall sleep and is able to sleep.
 * If the controller can sleep, it is set to sleepmode.
 *
 * @note call this function from a proper place, since the controller will
 *       also wake up here.
 */
bool sleep_check_and_goodnight(void)
{
    if (g_sleep_prevent_mask == 0 && g_sleep_required != 0) {
        // disable clock-timer, otherwise IRQ will cause immediate wakeup.
        timer_sleep(true);

        // sleep till byte is received or key is pressed.
        sleep_pinchange_enable();
        sleep_set_mode(SLEEP_MODE_IDLE);

        LED_ERROR_OFF;
        sleep_mode();

        // ...sleeping... zzzZZZ

        _delay_ms(1);      // wait for sys-clock becoming stable
        sleep_pinchange_disable();
        // enable timer
        timer_sleep(false);
        LED_ERROR_ON;

        return true;
    }
    return false;
}
/** @} */
