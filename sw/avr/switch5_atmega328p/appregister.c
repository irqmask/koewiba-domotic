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
#include "output.h"
#include "register.h"

#include "appconfig.h"
#include "application.h"
#include "pcbconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void        app_register_load       (void)
{
    uint8_t v;
    for (uint8_t chn=0; chn<APP_NUM_CHANNEL; chn++) {
        //eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_Mode + chn * APP_CFG_BYTES_PER_CHN], 0);
        v = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_Mode + chn * APP_CFG_BYTES_PER_CHN]);
        app_chn_mode[chn] = v;
    }
    for (uint8_t chn=0; chn<OUTPUT_NUM_PINS; chn++) {
        uint8_t chn_offset = chn * APP_CFG_BYTES_PER_CHN;
        //eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOff + chn_offset], 64);
        //eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOn + chn_offset], 128);
        v = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOff + chn_offset]);
        output_set_threshold_off(chn, v);
        v = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOn + chn_offset]);
        output_set_threshold_on(chn, v);

        for (uint8_t a=0; a<APP_ONOFFTIMER_COUNT * 2; a++) {
            // same for off and on
            v = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Weekday + a * APP_REGS_PER_ALARM + chn_offset]);
            alarm_set_days_of_week_mask(a + chn * APP_ONOFFTIMER_COUNT * 2, v);
            v = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Hour + a * APP_REGS_PER_ALARM + chn_offset]);
            alarm_set_hour(a + chn * APP_ONOFFTIMER_COUNT * 2, v);
            v = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Minute + a * APP_REGS_PER_ALARM + chn_offset]);
            alarm_set_minute(a + chn * APP_ONOFFTIMER_COUNT * 2, v);
        }
    }

}

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;
    uint8_t chn, index;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    // registers saved in EEProm
    if (reg_no >= APP_eReg_Chn0_SwitchCurrent && reg_no <= APP_eReg_Chn4_Unused0) {
        chn = (reg_no - APP_eReg_Chn0_SwitchCurrent) / APP_REGS_PER_CHN;
        index = reg_no - (chn * APP_REGS_PER_CHN);
        switch (index) {
        case APP_eReg_Chn0_SwitchSetPoint:
            // fallthrough
        case APP_eReg_Chn0_SwitchCurrent:
            *(uint8_t*)pvalue = output_get_value(chn);
            break;

        case APP_eReg_Chn0_ThresholdOff:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOff + chn * APP_CFG_BYTES_PER_CHN]);
            break;

        case APP_eReg_Chn0_ThresholdOn:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOn + chn * APP_CFG_BYTES_PER_CHN]);
            break;

        case APP_eReg_Chn0_Mode:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_Mode + chn * APP_CFG_BYTES_PER_CHN]);
            break;

        default:
            break;
        }

        if (index >= APP_eReg_Chn0_TimeOn0_Weekday && index <= APP_eReg_Chn0_TimeOff3_Minute) {
            // calculate alarmidx per channel
            uint8_t alarmidx = (index - APP_eReg_Chn0_TimeOn0_Weekday) / APP_REGS_PER_ALARM;
            // calculate offset to alarm block in register space
            uint8_t offset = alarmidx * APP_REGS_PER_ALARM;
            index -= offset;
            switch (index) {
            // same for on and off
            case APP_eReg_Chn0_TimeOn0_Weekday:
                *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Weekday + (chn * APP_CFG_BYTES_PER_CHN) + offset]);
                break;
            case APP_eReg_Chn0_TimeOn0_Hour:
                *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Hour + (chn * APP_CFG_BYTES_PER_CHN) + offset]);
                break;
            case APP_eReg_Chn0_TimeOn0_Minute:
                *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Minute + (chn * APP_CFG_BYTES_PER_CHN) + offset]);
                break;
            default:
                break;
            }
        }
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
    uint16_t    value16;
    uint8_t     value8, chn, index;

    value16 = (uint16_t)(value & 0x0000FFFF);
    value8 = (uint8_t)(value & 0x000000FF);

    // channel specific registers
    if (reg_no >= APP_eReg_Chn0_SwitchCurrent && reg_no <= APP_eReg_Chn4_Unused0) {
        chn = (reg_no - APP_eReg_Chn0_SwitchCurrent) / APP_REGS_PER_CHN;
        index = reg_no - (chn * APP_REGS_PER_CHN);
        switch (index) {
        case APP_eReg_Chn0_SwitchSetPoint:
            output_set(chn, value8);
            app_send_state(chn, output_get_value(chn));
            break;

        case APP_eReg_Chn0_ThresholdOff:
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOff + chn * APP_CFG_BYTES_PER_CHN], value8);
            output_set_threshold_off(chn, value8);
            break;

        case APP_eReg_Chn0_ThresholdOn:
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_ThresholdOn + chn * APP_CFG_BYTES_PER_CHN], value8);
            output_set_threshold_on(chn, value8);
            break;

        case APP_eReg_Chn0_Mode:
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_Mode + chn * APP_CFG_BYTES_PER_CHN], value8);
            app_set_mode(chn, value8);
            break;

        default:
            break;
        }

        if (index >= APP_eReg_Chn0_TimeOn0_Weekday && index <= APP_eReg_Chn0_TimeOff3_Minute) {
            // calculate alarmidx per channel
            uint8_t alarmidx = (index - APP_eReg_Chn0_TimeOn0_Weekday) / APP_REGS_PER_ALARM;
            // calculate offset to alarm block in register space
            uint8_t offset = alarmidx * APP_REGS_PER_ALARM;
            index -= offset;
            switch (index) {
            // same for on and off
            case APP_eReg_Chn0_TimeOn0_Weekday:
                eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Weekday + chn * APP_CFG_BYTES_PER_CHN + offset], value8);
                alarm_set_days_of_week_mask(alarmidx + chn * APP_ONOFFTIMER_COUNT * 2, value8);
                break;

            case APP_eReg_Chn0_TimeOn0_Hour:
                eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Hour + chn * APP_CFG_BYTES_PER_CHN + offset], value8);
                alarm_set_hour(alarmidx + chn * APP_ONOFFTIMER_COUNT * 2, value8);
                break;

            case APP_eReg_Chn0_TimeOn0_Minute:
                eeprom_write_byte(&register_eeprom_array[APP_eCfg_Chn0_TimeOn0_Minute + chn * APP_CFG_BYTES_PER_CHN + offset], value8);
                alarm_set_minute(alarmidx + chn * APP_ONOFFTIMER_COUNT * 2, value8);
                break;

            default:
                break;
            }
        }
    }
    else {
        switch (reg_no) {
        // registers in ROM/RAM
        case APP_eReg_Year:
            dt_set_year(value16);
            break;

        case APP_eReg_Month:
            dt_set_month(value8);
            break;

        case APP_eReg_Day:
            dt_set_day(value8);
            break;

        case APP_eReg_DayOfWeek:
            dt_set_day_of_week(value8);
            break;

        case APP_eReg_Hour:
            dt_set_hour(value8);
            break;

        case APP_eReg_Minute:
            dt_set_minute(value8);
            break;

        case APP_eReg_Second:
            dt_set_second(value8);
            break;

        default:
            break;
        }
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
