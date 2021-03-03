/**
 * @addtogroup SWITCH5_ATMEGA328
 * @addtogroup APPREGISTER
 * @brief Registers of the application "switch5_atmega328".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "switch5_atmega328".
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2020  christian <irqmask@web.de>
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
    uint8_t index;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    // registers saved in EEProm
    if (reg_no >= APP_eReg_Chn0_IntensityCurrent && reg_no <= APP_eReg_Chn4_Unused0) {
        index = (reg_no - APP_eReg_Chn0_IntensityCurrent) * 2;
        /// todo index += APP_eCfg_RemoteAddr00;
        //*(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[index]);
        //*preg_type = eRegType_U16;
    }

    else {
           // registers in ROM/RAM
           switch (reg_no) {
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
       }
    return true;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t    tempval16;
    uint8_t     tempval, index;

    tempval16 = (uint16_t)(value & 0x0000FFFF);
    tempval = (uint8_t)(value & 0x000000FF);

    // registers saved in EEProm
    if (reg_no >= APP_eReg_Chn0_IntensityCurrent && reg_no <= APP_eReg_Chn4_Unused0) {
        index = (reg_no - APP_eReg_Chn0_IntensityCurrent);
        /// todo index += APP_eCfg_RemoteAddr00;
        //eeprom_write_word((uint16_t*)&register_eeprom_array[index], tempval16);
    }
    else switch (reg_no) {
    // registers saved in EEProm
    // TODO add handler for with application specific registers here!


    // registers in ROM/RAM

    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
