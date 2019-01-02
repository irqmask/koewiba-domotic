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

BOOL        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue)
{
    eRegType_t  regtype;
    uint8_t index;

    if (peRegType == NULL) peRegType = &regtype;
    if (pvValue == NULL) return FALSE;
    *peRegType = eRegType_U8;

    // registers saved in EEProm
    if (uRegNo >= APP_eReg_RemoteAddr00 && uRegNo <= APP_eReg_RemoteAddr31) {
        index = (uRegNo - APP_eReg_RemoteAddr00) * 2;
        index += APP_eCfg_RemoteAddr00;
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[index]);
        *peRegType = eRegType_U16;
    }
    else if (uRegNo >= APP_eReg_RemoteReg00 && uRegNo <= APP_eReg_RemoteReg31) {
        index = uRegNo - APP_eReg_RemoteReg00;
        index += APP_eCfg_RemoteReg00;
        *(uint8_t*)pvValue = eeprom_read_byte(&register_eeprom_array[index]);
    }
    else if (uRegNo >= APP_eReg_TargetReg00 && uRegNo <= APP_eReg_TargetReg31) {
        index = uRegNo - APP_eReg_TargetReg00;
        index += APP_eCfg_TargetReg00;
        *(uint8_t*)pvValue = eeprom_read_byte(&register_eeprom_array[index]);
    }
    else switch (uRegNo) {
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
        return FALSE;
        break;
    }
    return TRUE;
}

void        app_register_set        (uint8_t                uRegNo,
                                     uint32_t               uValue)
{
    uint16_t    tempval16;
    uint8_t     tempval, index;

    tempval16 = (uint16_t)(uValue & 0x0000FFFF);
    tempval = (uint8_t)(uValue & 0x000000FF);

    // registers saved in EEProm
    if (uRegNo >= APP_eReg_RemoteAddr00 && uRegNo <= APP_eReg_RemoteAddr31) {
        index = (uRegNo - APP_eReg_RemoteAddr00) * 2;
        index += APP_eCfg_RemoteAddr00;
        eeprom_write_word((uint16_t*)&register_eeprom_array[index], tempval16);
    }
    else if (uRegNo >= APP_eReg_RemoteReg00 && uRegNo <= APP_eReg_RemoteReg31) {
        index = uRegNo - APP_eReg_RemoteReg00;
        index += APP_eCfg_RemoteReg00;
        eeprom_write_byte(&register_eeprom_array[index], tempval);
    }
    else if (uRegNo >= APP_eReg_TargetReg00 && uRegNo <= APP_eReg_TargetReg31) {
        index = uRegNo - APP_eReg_TargetReg00;
        index += APP_eCfg_TargetReg00;
        eeprom_write_byte(&register_eeprom_array[index], tempval);
    }
    else switch (uRegNo) {
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
