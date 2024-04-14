/**
 * @addtogroup HEATERVALVE_GATEWAY_ATTINY1634
 * @addtogroup APPREGISTER
 * @brief Registers of the application "heatervalve_gateway_attiny1634".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "heatervalve_gateway_attiny1634".
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

#include <avr/boot.h>
#include <avr/eeprom.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "moddef_common.h"

#include "register.h"

#include "appconfig.h"
#include "control_temp.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern void app_set_kp(uint16_t kp);
extern uint16_t app_get_kp(void);

void        app_register_load       (void)
{
    app_rem_temp_curr_modid = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_RemTempCurrentModuleID]);
    app_rem_temp_curr_regno = eeprom_read_byte(&register_eeprom_array[APP_eCfg_RemTempCurrentRegNo]);
    app_rem_temp_setp_modid = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_RemTempSetpointModuleID]);
    app_rem_temp_setp_regno = eeprom_read_byte(&register_eeprom_array[APP_eCfg_RemTempSetpointModuleID]);
    app_debug_receiver = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DebugReceiverModuleID]);
    ctrl_set_kp(eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_Kp]));
}

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    switch (reg_no) {
    // registers saved in EEProm
    case APP_eReg_RemTempCurrentModuleID:
        *(uint16_t*)pvalue = app_rem_temp_curr_modid;
        *preg_type = eRegType_U16;
        break;
    case APP_eReg_RemTempCurrentRegNo:
        *(uint8_t*)pvalue = app_rem_temp_curr_regno;
        break;
    case APP_eReg_RemTempSetpointModuleID:
        *(uint16_t*)pvalue = app_rem_temp_setp_modid;
        *preg_type = eRegType_U16;
        break;
    case APP_eReg_RemTempSetpointRegNo:
        *(uint8_t*)pvalue = app_rem_temp_setp_regno;
        break;
    case APP_eReg_DbgRecvModuleID:
        *(uint16_t*)pvalue = app_debug_receiver;
        *preg_type = eRegType_U16;
        break;
    case APP_eReg_Kp:
        *(uint16_t*)pvalue = ctrl_get_kp();
        *preg_type = eRegType_U16;
        break;

    // registers in ROM/RAM
    default:
        return false;
        break;
    }
    return true;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t    value16;
    uint8_t     value8;

    value16 = (uint16_t)(value & 0x0000FFFF);
    value8 = (uint8_t)(value & 0x000000FF);

    switch (reg_no) {
    // registers saved in EEProm
    case APP_eReg_RemTempCurrentModuleID:
        app_rem_temp_curr_modid = value16;
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_RemTempCurrentModuleID], value16);
        break;
    case APP_eReg_RemTempCurrentRegNo:
        app_rem_temp_curr_regno = value8;
        eeprom_write_byte(&register_eeprom_array[APP_eCfg_RemTempCurrentRegNo], value8);
        break;
    case APP_eReg_RemTempSetpointModuleID:
        app_rem_temp_setp_modid = value16;
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_RemTempSetpointModuleID], value16);
        break;
    case APP_eReg_RemTempSetpointRegNo:
        app_rem_temp_setp_regno = value8;
        eeprom_write_byte(&register_eeprom_array[APP_eCfg_RemTempSetpointRegNo], value8);
        break;
    case APP_eReg_DbgRecvModuleID:
        app_debug_receiver = value16;
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_DebugReceiverModuleID], value16);
        break;
    case APP_eReg_Kp:
        ctrl_set_kp(value16);
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_Kp], value16);
        break;

    // registers in ROM/RAM

    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
