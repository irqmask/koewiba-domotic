/**
 * @addtogroup BLINDCONTROL
 * @addtogroup APPREGISTER
 * @brief Registers of the application "blind-control_atmega328p".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "blind-control_atmega328p".
 *
 * The application specific registers are like this:
 *
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@web.de>
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

#include "alarmclock.h"
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

static const uint8_t g_eepreg_updowntimer_block_size = APP_eCfg_TimeDown1_Minute - APP_eCfg_TimeUp1_Weekdays + 1;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void        app_register_load       (void)
{
    uint8_t idx;

    blind_set_reaction_delay(0, eeprom_read_byte(&register_eeprom_array[APP_eCfg_ReactionDelay]));
    blind_set_duration_open (0, eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationOpen]));
    blind_set_duration_close(0, eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationClose]));
    blind_set_mode          (0, eeprom_read_byte(&register_eeprom_array[APP_eCfg_Mode]));

    // load up/down times
    for (idx=0; idx<APP_UPDOWNTIMER_COUNT; idx++) {
        alarm_set_hour             (idx*2,   eeprom_read_byte(&register_eeprom_array[idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeUp1_Hour]));
        alarm_set_minute           (idx*2,   eeprom_read_byte(&register_eeprom_array[idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeUp1_Minute]));
        alarm_set_days_of_week_mask(idx*2,   eeprom_read_byte(&register_eeprom_array[idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeUp1_Weekdays]));
        alarm_set_hour             (idx*2+1, eeprom_read_byte(&register_eeprom_array[idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeDown1_Hour]));
        alarm_set_minute           (idx*2+1, eeprom_read_byte(&register_eeprom_array[idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeDown1_Minute]));
        alarm_set_days_of_week_mask(idx*2+1, eeprom_read_byte(&register_eeprom_array[idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeDown1_Weekdays]));
    }
}

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;
    uint8_t     updown_timerblock_idx=0;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    if (reg_no <= APP_eReg_B0_Unused1) {
        switch (reg_no) {
        // registers in ROM/RAM
        case APP_eReg_B0_PositionCurrent:
            *(uint8_t*)pvalue = blind_get_current_position(0);
            break;

        case APP_eReg_B0_PositionSetPoint:
            *(uint8_t*)pvalue = blind_get_position_setpoint(0);
            break;

        case APP_eReg_B0_PositionDifference:
            *(uint8_t*)pvalue = (blind_get_position_setpoint(0) - blind_get_current_position(0));
            break;

        // registers saved in EEProm
        case APP_eReg_B0_ReactionDelay:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_ReactionDelay]);
            break;

        case APP_eReg_B0_DurationOpen:
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationOpen]);
            *preg_type = eRegType_U16;
            break;

        case APP_eReg_B0_DurationClose:
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationClose]);
            *preg_type = eRegType_U16;
            break;

        case APP_eReg_B0_Mode:
            *(uint8_t*)pvalue = eeprom_read_byte((uint8_t*)&register_eeprom_array[APP_eCfg_Mode]);
            break;

        default:
            return false;
            break;
        }
    } else if (reg_no >= APP_eReg_TimeUp1_Weekday && reg_no <= APP_eReg_TimeDown4_Unused1) {
        updown_timerblock_idx = (reg_no - APP_eReg_TimeUp1_Weekday) / (APP_eReg_TimeDown4_Unused1 - APP_eReg_TimeUp1_Weekday + 1);
        reg_no -= (updown_timerblock_idx * (APP_eReg_TimeDown4_Unused1 - APP_eReg_TimeUp1_Weekday + 1));
        switch (reg_no) {
        // registers saved in EEProm
        case APP_eReg_TimeUp1_Weekday:
            *(uint8_t*)pvalue = alarm_get_days_of_week_mask(updown_timerblock_idx*2);
            break;

        case APP_eReg_TimeUp1_Hour:
            *(uint8_t*)pvalue = alarm_get_hour(updown_timerblock_idx*2);
            break;

        case APP_eReg_TimeUp1_Minute:
            *(uint8_t*)pvalue = alarm_get_minute(updown_timerblock_idx*2);
            break;

        case APP_eReg_TimeDown1_Weekday:
            *(uint8_t*)pvalue = alarm_get_days_of_week_mask(updown_timerblock_idx*2+1);
            break;

        case APP_eReg_TimeDown1_Hour:
            *(uint8_t*)pvalue = alarm_get_hour(updown_timerblock_idx*2+1);
            break;

        case APP_eReg_TimeDown1_Minute:
            *(uint8_t*)pvalue = alarm_get_minute(updown_timerblock_idx*2+1);
            break;

        default:
            return false;
        }
    } else {
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
    uint16_t    value16;
    uint8_t     value8;
    uint8_t     updown_timerblock_idx=0;

    if (reg_no <= APP_eReg_B0_Unused1) {
        switch (reg_no) {
        // registers in ROM/RAM
        case APP_eReg_B0_PositionCurrent:
            // read only
            break;

        case APP_eReg_B0_PositionSetPoint:
            blind_move_to_position(0, value & 0x000000FF);
            break;

        // registers saved in EEProm
        case APP_eReg_B0_ReactionDelay:
            value8 = value & 0x000000FF;
            blind_set_reaction_delay(0, value8);
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_ReactionDelay], value8);
            break;

        case APP_eReg_B0_DurationOpen:
            value16 = value & 0x0000FFFF;
            blind_set_duration_open(0, value16);
            eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationOpen], value16);
            break;

        case APP_eReg_B0_DurationClose:
            value16 = value & 0x0000FFFF;
            blind_set_duration_close(0, value16);
            eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationClose], value16);
            break;

        case APP_eReg_B0_Mode:
            value8 = value & 0x000000FF;
            blind_set_mode(0, value8);
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_Mode], value8);
            break;

        default:
            break;
        }
    } else if (reg_no >= APP_eReg_TimeUp1_Weekday && reg_no <= APP_eReg_TimeDown4_Unused1) {
        updown_timerblock_idx = (reg_no - APP_eReg_TimeUp1_Weekday) / (APP_eReg_TimeDown4_Unused1 - APP_eReg_TimeUp1_Weekday + 1);
        reg_no -= (updown_timerblock_idx * (APP_eReg_TimeDown4_Unused1 - APP_eReg_TimeUp1_Weekday + 1));
        value8 = value & 0x000000FF;
        switch (reg_no) {
        // registers saved in EEProm
        case APP_eReg_TimeUp1_Weekday:
            alarm_set_days_of_week_mask(updown_timerblock_idx*2, value8);
            eeprom_write_byte(&register_eeprom_array[updown_timerblock_idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeUp1_Weekdays], value8);
            break;

        case APP_eReg_TimeUp1_Hour:
            alarm_set_hour(updown_timerblock_idx*2, value8);
            eeprom_write_byte(&register_eeprom_array[updown_timerblock_idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeUp1_Hour], value8);
            break;

        case APP_eReg_TimeUp1_Minute:
            alarm_set_minute(updown_timerblock_idx*2, value8);
            eeprom_write_byte(&register_eeprom_array[updown_timerblock_idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeUp1_Minute], value8);
            break;

        case APP_eReg_TimeDown1_Weekday:
            alarm_set_days_of_week_mask(updown_timerblock_idx*2+1, value8);
            eeprom_write_byte(&register_eeprom_array[updown_timerblock_idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeDown1_Weekdays], value8);
            break;

        case APP_eReg_TimeDown1_Hour:
            alarm_set_hour(updown_timerblock_idx*2+1, value8);
            eeprom_write_byte(&register_eeprom_array[updown_timerblock_idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeDown1_Hour], value8);
            break;

        case APP_eReg_TimeDown1_Minute:
            alarm_set_minute(updown_timerblock_idx*2+1, value8);
            eeprom_write_byte(&register_eeprom_array[updown_timerblock_idx * g_eepreg_updowntimer_block_size + APP_eCfg_TimeDown1_Minute], value8);
            break;

        default:
            break;
        }
    } else {
        switch (reg_no) {
        // registers in ROM/RAM
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
