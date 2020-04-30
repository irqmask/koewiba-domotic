/**
 * @addtogroup LED2_ATMEGA324P
 * @addtogroup APPREGISTER
 * @brief Registers of the application "led2_atmega324p".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "led2_atmega324p".
 *///---------------------------------------------------------------------------
/*
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

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    switch (reg_no) {
    case APP_eReg_IntensityChn0Current:
    	break;

    case APP_eReg_IntensityChn1Current:
    	break;

    case APP_eReg_IntensityChn0SetPoint:
    	*(uint8_t*)pvalue = app_led_get_intensity(0);
    	break;

    case APP_eReg_IntensityChn1SetPoint:
    	*(uint8_t*)pvalue = app_led_get_intensity(1);
    	break;

    // registers saved in EEProm
    case APP_eReg_IntensityChn0Store:
        *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_IntensityChn0Store]);
        break;

    case APP_eReg_IntensityChn1Store:
        *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_IntensityChn1Store]);
        break;

    // registers in ROM/RAM
    default:
        return false;
    }
    return true;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint8_t     value8;

    value8 = (uint8_t)(value & 0x000000FF);

    switch (reg_no) {
    case APP_eReg_IntensityChn0Current:
    	// read-only register
    	break;

    case APP_eReg_IntensityChn1Current:
    	// read-only register
    	break;

    case APP_eReg_IntensityChn0SetPoint:
        app_led_set_intensity(0, value8);
    	break;

    case APP_eReg_IntensityChn1SetPoint:
        app_led_set_intensity(1, value8);
    	break;

    // registers saved in EEProm
    case APP_eReg_IntensityChn0Store:
        eeprom_write_byte(&register_eeprom_array[APP_eCfg_IntensityChn0Store], value8);
        app_led_set_intensity(0, value8);
        break;

    case APP_eReg_IntensityChn1Store:
        eeprom_write_byte(&register_eeprom_array[APP_eCfg_IntensityChn1Store], value8);
        app_led_set_intensity(1, value8);
        break;

    // registers in ROM/RAM

    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
