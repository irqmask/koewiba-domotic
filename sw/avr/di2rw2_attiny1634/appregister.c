/**
 * @addtogroup DI2RW2_ATTINY1634
 * @addtogroup APPREGISTER
 * @brief Registers of the application "di2rw2_attiny1634".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "di2rw2_attiny1634".
 *
 * @author  Robert Mueller
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
// --- Include section ---------------------------------------------------------

#include <avr/boot.h>
#include <avr/eeprom.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "moddef_common.h"

#include "appconfig.h"
#include "application.h"
#include "register.h"
#include "pcbconfig.h"
#include "relays_keys.h"


// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern app_on_key_set_register_t g_on_key_set_register[APP_NUM_KEYS];



void        app_register_load       (void)
{
    uint8_t index, eepindex;

    for (index=0; index<APP_NUM_KEYS; index++) {
        eepindex = APP_eCfg_In1_Mode + index * APP_NUM_EEBYTES_PER_INPUT;
        g_on_key_set_register[index].key_mode = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_In1_RemoteAddr + index * APP_NUM_EEBYTES_PER_INPUT;
        g_on_key_set_register[index].receiver = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_In1_RemoteReg + index * APP_NUM_EEBYTES_PER_INPUT;
        g_on_key_set_register[index].register_id = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_In1_Value + index * APP_NUM_EEBYTES_PER_INPUT;
        g_on_key_set_register[index].value = eeprom_read_byte(&register_eeprom_array[eepindex]);
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

	// registers in ROM/RAM
    if (reg_no >= APP_eReg_InputState_Bits && reg_no <= APP_eReg_Output8_State) {
        // TODO add getter functions here
        //*(uint8_t*)pvalue = (blind_get_position_setpoint(index) - blind_get_current_position(index));
    }
    // registers saved in EEProm
    else if (reg_no >= APP_eReg_In1_Mode &&
        reg_no < (APP_eReg_In1_Mode + INPUT_NUM_PINS*APP_NUM_REGS_PER_INPUT)) {

        // calculate index of input and normalize register number
        index = (reg_no - APP_eReg_In1_Mode) / APP_NUM_REGS_PER_INPUT;
        reg_no -= (index * APP_NUM_REGS_PER_INPUT);

        switch (reg_no) {
        case APP_eReg_In1_Mode:
            eepindex = APP_eCfg_In1_Mode + index * APP_NUM_EEBYTES_PER_INPUT;
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[eepindex]);
            break;
        case APP_eReg_In1_RemoteAddr:
            eepindex = APP_eCfg_In1_RemoteAddr + index * APP_NUM_EEBYTES_PER_INPUT;
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);
            *preg_type = eRegType_U16;
            break;
        case APP_eReg_In1_RemoteReg:
            eepindex = APP_eCfg_In1_RemoteReg + index * APP_NUM_EEBYTES_PER_INPUT;
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[eepindex]);
            break;
        case APP_eReg_In1_Value:
            eepindex = APP_eCfg_In1_Value + index * APP_NUM_EEBYTES_PER_INPUT;
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[eepindex]);
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
    uint8_t     value8, index, eepindex;

    value16 = (uint16_t)(value & 0x0000FFFF);
    value8  = (uint8_t) (value & 0x000000FF);

    if (reg_no >= APP_eReg_InputState_Bits && reg_no <= APP_eReg_Output8_State) {
        // TODO add mode dependent function call here.
        app_call_mode_dep_func(reg_no, value8);
    }
    // registers saved in EEProm
    if (reg_no >= APP_eReg_In1_Mode &&
        reg_no < (APP_eReg_In1_Mode + INPUT_NUM_PINS*APP_NUM_REGS_PER_INPUT)) {

        // calculate index of input and normalize register number
        index = (reg_no - APP_eReg_In1_Mode) / APP_NUM_REGS_PER_INPUT;
        reg_no -= (index * APP_NUM_REGS_PER_INPUT);

        switch (reg_no){
            case APP_eReg_In1_Mode:
                // TODO Set value also in RAM
                //blind_set_reaction_delay(index, value8);
                eepindex = APP_eCfg_In1_Mode + index * APP_NUM_EEBYTES_PER_INPUT;
                eeprom_write_byte(&register_eeprom_array[eepindex], value8);
                break;
            case APP_eReg_In1_RemoteAddr:
                eepindex = APP_eCfg_In1_RemoteAddr + index * APP_NUM_EEBYTES_PER_INPUT;
                eeprom_write_word((uint16_t*)&register_eeprom_array[eepindex], value16);
                break;
            case APP_eReg_In1_RemoteReg:
                eepindex = APP_eCfg_In1_RemoteReg + index * APP_NUM_EEBYTES_PER_INPUT;
                eeprom_write_byte(&register_eeprom_array[eepindex], value8);
                break;
            case APP_eReg_In1_Value:
                eepindex = APP_eCfg_In1_Value + index * APP_NUM_EEBYTES_PER_INPUT;
                eeprom_write_byte(&register_eeprom_array[eepindex], value8);
                break;

            default:
                break;
            }
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
