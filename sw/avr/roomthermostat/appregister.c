/**
 * @addtogroup ROOMTHERMOSTAT
 * @addtogroup APPREGISTER
 * @brief Registers of the application "roomthermostat_atmega328p".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "roomthermostat_atmega328p".
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

#include "alarmclock.h"
#include "datetime.h"
#include "register.h"

#include "appconfig.h"
#include "application.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

extern uint16_t app_current_temp;
extern uint16_t app_desired_temp;
extern int16_t app_temp_offset;

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void app_register_load(void)
{
    app_set_temperature_offset(eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_TempOffset]));
    app_set_windowcontact_moduleid(eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_WindowSensorModuleID]));
    app_set_windowcontact_reg(eeprom_read_byte(&register_eeprom_array[APP_eCfg_WindowSensorReg]));
    app_set_mode(eeprom_read_byte(&register_eeprom_array[APP_eCfg_Mode]));
    app_set_windowopen_temperature(eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_TemperatureWindowOpen]));
    app_set_vacation_temperature(eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_TemperatureVacation]));
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
    case APP_eReg_TempCurrent:
        *(uint16_t*)pvalue = app_current_temp;
        *preg_type = eRegType_U16;
        break;

    case APP_eReg_TempSetPoint:
        *(uint16_t*)pvalue = app_desired_temp;
        *preg_type = eRegType_U16;
        break;

    case APP_eReg_TempOffset:
        *(uint16_t*)pvalue = app_get_temperature_offset();
        *preg_type = eRegType_U16;
        break;

    case APP_eReg_WindowSensorModuleID:
        *(uint16_t*)pvalue = app_get_windowcontact_moduleid();
        *preg_type = eRegType_U16;
        break;

    case APP_eReg_WindowSensorReg:
        *(uint8_t*)pvalue = app_get_windowcontact_reg();
        *preg_type = eRegType_U8;
        break;

        // TODO add handler for with application specific registers here!

    // registers in ROM/RAM
    case APP_eReg_Year:
        *(uint16_t*)pvalue = dt_get_year();
        *preg_type = eRegType_U16;
        break;

    case APP_eReg_Month:
        *(uint8_t*)pvalue = dt_get_month();
        break;

    case APP_eReg_Day:
        *(uint8_t*)pvalue = dt_get_day();
        break;

    case APP_eReg_DayOfWeek:
        *(uint8_t*)pvalue = dt_get_day_of_week();
        break;

    case APP_eReg_Hour:
        *(uint8_t*)pvalue = dt_get_hour();
        break;

    case APP_eReg_Minute:
        *(uint8_t*)pvalue = dt_get_minute();
        break;

    case APP_eReg_Second:
        *(uint8_t*)pvalue = dt_get_second();
        break;

    default:
        return false;
        break;
    }
    return true;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t    tempval16;
    uint8_t     tempval8;

    tempval16 = (uint16_t)(value & 0x0000FFFF);
    tempval8 = (uint8_t)(value & 0x000000FF);

    switch (reg_no) {
    // registers saved in EEProm
    // TODO add handler for with application specific registers here!

    // registers in ROM/RAM
     case APP_eReg_TempCurrent:
         // read only
         break;

     case APP_eReg_TempSetPoint:
         app_desired_temp = tempval16;
         app_draw_desired_temp();
         break;

     case APP_eReg_TempOffset:
         app_set_temperature_offset(tempval16);
         eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_TempOffset], tempval16);
         break;

     case APP_eReg_WindowSensorModuleID:
         app_set_windowcontact_moduleid(tempval16);
         eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_WindowSensorModuleID], tempval16);
         break;

     case APP_eReg_WindowSensorReg:
         app_set_windowcontact_reg(tempval8);
         eeprom_write_byte((uint8_t*)&register_eeprom_array[APP_eCfg_WindowSensorReg], tempval8);
         break;

     case APP_eReg_Mode:
         app_set_mode(tempval8);
         eeprom_write_byte((uint8_t*)&register_eeprom_array[APP_eCfg_Mode], tempval8);
         break;

     case APP_eReg_TemperatureWindowOpen:
         app_set_windowopen_temperature(tempval16);
         eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eReg_TemperatureWindowOpen], tempval16);
         break;

     case APP_eReg_TemperatureVacation:
         app_set_vacation_temperature(tempval16);
         eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eReg_TemperatureVacation], tempval16);
         break;

     // registers in ROM/RAM
     case APP_eReg_Year:
         dt_set_year(tempval16);
         break;

     case APP_eReg_Month:
         dt_set_month(tempval8);
         break;

     case APP_eReg_Day:
         dt_set_day(tempval8);
         break;

     case APP_eReg_DayOfWeek:
         dt_set_day_of_week(tempval8);
         break;

     case APP_eReg_Hour:
         dt_set_hour(tempval8);
         break;

     case APP_eReg_Minute:
         dt_set_minute(tempval8);
         break;

     case APP_eReg_Second:
         dt_set_second(tempval8);
         break;
    // registers in ROM/RAM

    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
