/**
 * @addtogroup STM8_LCD
 * @brief A module to interface STM8 LCD display.
 *
 * @{
 * @file    lcd.h
 * @brief   A module to interface STM8 LCD display.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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

#ifndef _STM8_LCD_H_
#define _STM8_LCD_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize LCD peripheral
 */
void lcd_initialize(void);

/**
 * Switch blinking display on or off
 * @param[in]   on      true = blinking, otherwise false
 */
void lcd_blink(bool o);

void lcd_digit(uint8_t index, char value);

void lcd_disp_value(uint16_t value, uint8_t decpoint);

void lcd_disp_symbol(char symbol, bool on);

#endif // _STM8_LCD_H_
/** @} */
