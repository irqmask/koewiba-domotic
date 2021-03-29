/**
 * @addtogroup ELEKTOR_BUS-NODE
 * @addtogroup APPLICATION
 * @brief Interface of the business logic of the application "elektor_bus-node".
 *
 * @{
 * @file    application.h
 * @brief   Interface of the business logic of the application "elektor_bus-node".
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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

#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

sBus_t  app_bus;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void        app_initialize_modes    (void);

// --- Global functions --------------------------------------------------------

void        app_initialize          (void);

void        app_check_keys          (void);

bool        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue);

void        app_register_set        (uint8_t                uRegNo,
                                     uint32_t               uValue);

/** @} */
