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

#include <stm8l15x_tim1.h>

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

/**
 * Initialize timer.
 */
void timer_initialize(void)
{
 //   g_tim2_millis = 0;
    // 16000 ticks
 //   TIM2_ARRH = 0x3E;
 //   TIM2_ARRL = 0x80;
    //TIM2_PSCR = 0b010;

   // TIM2_IER |= 0x1; // Update interrupt
   // TIM2_CR1 = 0x1; // enable timer
    g_millisec_ticks = 0;
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
    TIM1_TimeBaseInit(TIMER_TICK_PRESCALER,
                      TIM1_CounterMode_Up,
                      TIMER_TICK_PERIOD,
                      TIMER_TICK_REPTETION_COUNTER);
    TIM1_ITConfig(TIM1_IT_Update, ENABLE);
    TIM1_Cmd(ENABLE);
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

/*
void TIM1_UPD_OVF_TRG_COM_IRQHandler() __interrupt(IPT_TIM1_UPD_OVF_TRG_COM)
{
    timer_irq_handler();
    TIM1_ClearITPendingBit(TIM1_IT_Update);
}*/


/** @} */
