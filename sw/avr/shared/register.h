/**
 * @addtogroup REGISTER
 * @brief Module to handle register read and write operations.
 *
 * Contains access functions for channels and registers for different data
 * types.
 *
 * @{
 * @file    register.h
 * @brief   Module to handle register read and write operations.
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
#ifndef _REGISTER_H_
#define _REGISTER_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#include "bus.h"

// --- Definitions -------------------------------------------------------------

#define REG_MAX_MAPPINGS    32  //!< Maximum number of register mappings.

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern uint8_t register_eeprom_array[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

bool        register_get            (uint8_t                reg_no,
                                     eRegType_t*            reg_type,
                                     void*                  value);

void        register_set_u8         (uint8_t                reg_no,
                                     uint8_t                value);

void        register_set_u16        (uint8_t                reg_no,
                                     uint16_t               value);

void        register_set_u32        (uint8_t                reg_no,
                                     uint32_t               value);

void        register_do_command     (uint16_t               sender,
                                     uint8_t                msglen,
                                     uint8_t*               msg);

void        register_send_u8        (uint16_t               receiver,
                                     uint8_t                reg_no,
                                     uint8_t                value);

void        register_send_u16       (uint16_t               receiver,
                                     uint8_t                reg_no,
                                     uint16_t               value);

void        register_send_u32       (uint16_t               receiver,
                                     uint8_t                reg_no,
                                     uint32_t               value);

bool        register_do_mapping     (uint16_t               remote_module_addr,
                                     uint8_t                remote_register,
                                     uint32_t               value);

#endif // _REGISTER_H_
/**
 * @}
 * @}
 */
