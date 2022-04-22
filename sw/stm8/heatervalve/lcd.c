/**
 * @addtogroup STM8_LCD
 *
 * @{
 * @file    lcd.c
 * @brief   A module to interface STM8 LCD display.
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

#include "lcd.h"
#include "STM8L052C6.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize LCD interface.
 */
void lcd_initialize(void)
{
    //LCD_CR1 = 0x00;
    //LCD_CR2 = 0x00;
    //LCD_CR3 = 0x00;
    //LCD_FRQ = 0x11;
    //LCD_PM0 = ;
    //LCD_PM1 = ;
    //LCD_PM2 = ;

}

/** @} */
