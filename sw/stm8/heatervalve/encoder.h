/**
 * @addtogroup ENCODER
 * @brief   A module to interface the heatervalve's encoder.
 *
 * @{
 * @file    encoder.h
 * @brief   A module to interface the heatervalve's encoder.
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

#ifndef _ENCODER_H_
#define _ENCODER_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

void enc_initialize(void);

void enc_on_irq(void);

bool enc_val_changed(void);

uint8_t enc_read(void);

void enc_background(void);

#endif // _ENCODER_H_
/** @} */
