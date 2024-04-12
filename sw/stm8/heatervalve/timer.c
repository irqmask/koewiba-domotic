/**
 * @addtogroup STM8_TIMER
 *
 * @{
 * @file    timer.c
 * @brief   A module containing simple timing routines.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
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

#include "timer.h"

// include
#include "stm8l052c6.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

#define TIMER_TICK_PRESCALER            10
#define TIMER_TICK_PERIOD               199
#define TIMER_TICK_REPTETION_COUNTER    0

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

volatile uint32_t g_millisec_ticks = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

extern void app_irq_every_millisec(void);
/**
 * Initialize timer.
 */
void timer_initialize(void)
{
    g_millisec_ticks = 0;
    CLK_PCKENR2 |= CLK_PCKENR2_TIM1;

    // set auto-reload register value
    TIM1_ARRH = (uint8_t)(TIMER_TICK_PERIOD >> 8);
    TIM1_ARRL = (uint8_t)(TIMER_TICK_PERIOD & 0xFF);

    // set prescaler value
    TIM1_PSCRH = (uint8_t)(TIMER_TICK_PRESCALER >> 8);
    TIM1_PSCRL = (uint8_t)(TIMER_TICK_PRESCALER & 0xFF);

    // Select the Counter Mode: count up (=0)
    TIM1_CR1 &= ~(TIM_CR1_CMSH | TIM_CR1_CMSL | TIM_CR1_DIR);

    TIM1_RCR = TIMER_TICK_REPTETION_COUNTER;

    TIM1_IER |= TIM_IER_UIE; // update interrupt enable

    // enable timer 1
    TIM1_CR1 |= TIM_CR1_CEN;
}


void timer_irq_handler(void)
{
    g_millisec_ticks++;
}


uint32_t timer_get_millis(void)
{
    return g_millisec_ticks;
}


void delay_ms(uint16_t ms)
{
    uint32_t start = timer_get_millis();
    while ((timer_get_millis() - start) < ms);
}


void TIM1_UPD_OVF_TRG_COM_IRQHandler() __interrupt(IPT_TIM1_UPD_OVF_TRG_COM)
{
    timer_irq_handler();
    app_irq_every_millisec();
    TIM1_SR1 = ~(uint8_t)TIM_SR1_UIF;
}

/** @} */
