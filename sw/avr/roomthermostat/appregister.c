/**
 * @addtogroup ROOMTHERMOSTAT
 * @addtogroup APPREGISTER
 * @brief Registers of the application roomthermostat.
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application roomthermostat.
 *
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

#include <avr/boot.h>
#include <avr/eeprom.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "moddef_common.h"

#include "register.h"

#include "appconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

bool        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue)
{
    eRegType_t  regtype;

    if (peRegType == NULL) peRegType = &regtype;
    if (pvValue == NULL) return false;
    *peRegType = eRegType_U8;

    switch (uRegNo) {
    // registers saved in EEProm
    case APP_eReg_DesiredTempDay1:
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempDay1]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempNight1:
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempNight1]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempDay2:
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempDay2]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempNight2:
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempNight2]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempAway:
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempAway]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempWindowOpened:
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempWindowOpened]);
        *peRegType = eRegType_U16;
        break;

    // registers in ROM/RAM
    default:
        return false;
        break;
    }
    return true;
}

void        app_register_set        (uint8_t                uRegNo,
                                     uint32_t               uValue)
{
    uint16_t    tempval16;
    uint8_t     tempval;

    tempval16 = (uint16_t)(uValue & 0x0000FFFF);
    tempval = (uint8_t)(uValue & 0x000000FF);

    switch (uRegNo) {
    // registers saved in EEProm
    case APP_eReg_DesiredTempDay1:
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempDay1], tempval16);
        break;
    case APP_eReg_DesiredTempNight1:
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eReg_DesiredTempNight1], tempval16);
        break;
    case APP_eReg_DesiredTempDay2:
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_DesiredTempDay2], tempval16);
        break;
    case APP_eReg_DesiredTempNight2:
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eReg_DesiredTempNight2], tempval16);
        break;
    case APP_eReg_DesiredTempAway:
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eReg_DesiredTempAway], tempval16);
        break;
    case APP_eReg_DesiredTempWindowOpened:
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eReg_DesiredTempWindowOpened], tempval16);
        break;

    // registers in ROM/RAM

    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
