/**
 * @addtogroup BLINDCONTROL_5_ATMEGA328
 * @addtogroup APPREGISTER
 * @brief Registers of the application "blindcontrol_5_atmega328p".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "blindcontrol_5_atmega328p".
 *
 * @author  Christian Verhalen
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

#include "appconfig.h"
#include "blindctrl.h"
#include "datetime.h"
#include "moddef_common.h"
#include "prjtypes.h"
#include "register.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void        app_register_load       (void)
{
    uint8_t index, eepindex;

    for (index=0; index<BLIND_COUNT; index++) {
        eepindex = APP_eCfg_B0_ReactionDelay + index * NUM_EEBYTES_PER_BLIND;
        blind_set_reaction_delay(index, eeprom_read_byte(&register_eeprom_array[eepindex]));
        eepindex = APP_eCfg_B0_DurationOpen + index * NUM_EEBYTES_PER_BLIND;
        blind_set_duration_open(index, eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]));
        eepindex = APP_eCfg_B0_DurationClose + index * NUM_EEBYTES_PER_BLIND;
        blind_set_duration_close(index, eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]));
        eepindex = APP_eCfg_B0_Mode + index * NUM_EEBYTES_PER_BLIND;
        blind_set_mode(index, eeprom_read_byte(&register_eeprom_array[eepindex]));
    }
}

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;
    uint8_t index=0, eepindex=0;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    if (reg_no >= APP_eReg_B0_PositionCurrent &&
        reg_no < (APP_eReg_B0_PositionCurrent + BLIND_COUNT*APP_NUM_REGS_PER_BLIND)) {

        // calculate index of blind and normalize register number
        index = (reg_no - APP_eReg_B0_PositionCurrent) / APP_NUM_REGS_PER_BLIND;
        reg_no -= (index * APP_NUM_REGS_PER_BLIND);

        switch (reg_no) {
        // registers in ROM/RAM
        case APP_eReg_B0_PositionCurrent:
            *(uint8_t*)pvalue = blind_get_current_position(index);
            break;
        case APP_eReg_B0_PositionSetPoint:
            *(uint8_t*)pvalue = blind_get_position_setpoint(index);
            break;
        case APP_eReg_B0_PositionDifference:
            *(uint8_t*)pvalue = (blind_get_position_setpoint(index) - blind_get_current_position(index));
            break;

        // registers saved in EEProm
        case APP_eReg_B0_ReactionDelay:
            eepindex = APP_eCfg_B0_ReactionDelay + index * NUM_EEBYTES_PER_BLIND;
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[eepindex]);
            break;
        case APP_eReg_B0_DurationOpen:
            eepindex = APP_eCfg_B0_DurationOpen + index * NUM_EEBYTES_PER_BLIND;
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);
            *preg_type = eRegType_U16;
            break;
        case APP_eReg_B0_DurationClose:
            eepindex = APP_eCfg_B0_DurationClose + index * NUM_EEBYTES_PER_BLIND;
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);
            *preg_type = eRegType_U16;
            break;
        case APP_eReg_B0_Mode:
            eepindex = APP_eCfg_B0_Mode + index * NUM_EEBYTES_PER_BLIND;
            *(uint8_t*)pvalue = eeprom_read_byte((uint8_t*)&register_eeprom_array[eepindex]);
            break;

        default:
            return false;
            break;
        }
        return true;
    } else {
        // common registers
        switch (reg_no) {
        // registers in ROM/RAM
        case APP_eReg_Year:
            *(uint16_t*)pvalue = dt_get_year();
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
        }
        return true;
    }
    return false;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t value16;
    uint8_t value8, index=0, eepindex=0;

    if (reg_no >= APP_eReg_B0_PositionCurrent &&
        reg_no < (APP_eReg_B0_PositionCurrent + BLIND_COUNT*APP_NUM_REGS_PER_BLIND)) {

        // calculate index of blind and normalize register number
        index = (reg_no - APP_eReg_B0_PositionCurrent) / APP_NUM_REGS_PER_BLIND;
        reg_no -= (index * APP_NUM_REGS_PER_BLIND);

        // registers in ROM/RAM
        switch (reg_no) {
        case APP_eReg_B0_PositionCurrent:
            // read only
            break;
        case APP_eReg_B0_PositionSetPoint:
            blind_move_to_position(index, value & 0x000000FF);
            break;

        // registers saved in EEProm
        case APP_eReg_B0_ReactionDelay:
            value8 = value & 0x000000FF;
            blind_set_reaction_delay(index, value8);
            eepindex = APP_eCfg_B0_ReactionDelay + index * NUM_EEBYTES_PER_BLIND;
            eeprom_write_byte(&register_eeprom_array[eepindex], value8);
            break;
        case APP_eReg_B0_DurationOpen:
            value16 = value & 0x0000FFFF;
            blind_set_duration_open(index, value16);
            eepindex = APP_eCfg_B0_DurationOpen + index * NUM_EEBYTES_PER_BLIND;
            eeprom_write_word((uint16_t*)&register_eeprom_array[eepindex], value16);
            break;
        case APP_eReg_B0_DurationClose:
            value16 = value & 0x0000FFFF;
            blind_set_duration_close(index, value16);
            eepindex = APP_eCfg_B0_DurationClose + index * NUM_EEBYTES_PER_BLIND;
            eeprom_write_word((uint16_t*)&register_eeprom_array[eepindex], value16);
            break;
        case APP_eReg_B0_Mode:
            value8 = value & 0x000000FF;
            blind_set_mode(index, value8);
            eepindex = APP_eCfg_B0_Mode + index * NUM_EEBYTES_PER_BLIND;
            eeprom_write_byte(&register_eeprom_array[eepindex], value8);
            break;
        default:
            break;
        }
    } else {
        switch (reg_no) {
        case APP_eReg_Year:
            value16 = value & 0x0000FFFF;
            dt_set_year(value16);
            break;

        case APP_eReg_Month:
            value8 = value & 0x000000FF;
            dt_set_month(value8);
            break;

        case APP_eReg_Day:
            value8 = value & 0x000000FF;
            dt_set_day(value8);
            break;

        case APP_eReg_DayOfWeek:
            value8 = value & 0x000000FF;
            dt_set_day_of_week(value8);
            break;

        case APP_eReg_Hour:
            value8 = value & 0x000000FF;
            dt_set_hour(value8);
            break;

        case APP_eReg_Minute:
            value8 = value & 0x000000FF;
            dt_set_minute(value8);
            break;

        case APP_eReg_Second:
            value8 = value & 0x000000FF;
            dt_set_second(value8);
            break;

        default:
            break;
        }
    }

}

// --- Global functions --------------------------------------------------------

/** @} */
