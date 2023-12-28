/**
 * @addtogroup TASTER8
 * @addtogroup APPREGISTER
 * @brief Registers of the application taster8.
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application taster8.
 *
 * @author  Christian Verhalen, Robert Mueller
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/eeprom.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "moddef_common.h"

#include "appconfig.h"
#include "ledskeys.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern app_on_key_set_register_t     g_on_key_set_register[APP_NUM_KEY_STUBS];
extern app_on_msg_received_t         g_on_msg_received[APP_NUM_MSG_STUBS];

// --- Global functions --------------------------------------------------------

void        app_register_load       (void)
{
    uint8_t index, eepindex;

    for (index=0; index<APP_NUM_KEY_STUBS; index++) {
        eepindex = APP_eCfg_K0_Mode         + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].key_mode = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_K0_RemoteAddr + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].receiver = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_K0_RemoteReg  + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].register_id = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_K0_Value      + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].value = eeprom_read_byte(&register_eeprom_array[eepindex]);
    }

    for (index=0; index<APP_NUM_MSG_STUBS; index++) {
        eepindex = APP_eCfg_RecMsg0_RemoteAddr + index * NUM_EEBYTES_PER_STUB;
        g_on_msg_received[index].receiver = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_RecMsg0_Command    + index * NUM_EEBYTES_PER_STUB;
        g_on_msg_received[index].command = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_RecMsg0_Register   + index * NUM_EEBYTES_PER_STUB;
        g_on_msg_received[index].register_id = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_RecMsg0_LEDFunc    + index * NUM_EEBYTES_PER_STUB;
        g_on_msg_received[index].func_id = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_RecMsg0_AddInfo    + index * NUM_EEBYTES_PER_STUB;
        g_on_msg_received[index].add_info = eeprom_read_byte(&register_eeprom_array[eepindex]);
    }
}


bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;
    uint8_t    index;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    if (reg_no >= APP_eReg_K0_Mode &&
        reg_no < (APP_eReg_K0_Mode + APP_NUM_KEY_STUBS * NUM_REGS_PER_KEY))
    {
        index = (reg_no - APP_eCfg_K0_Mode) / NUM_REGS_PER_KEY;

        switch (reg_no - index * NUM_REGS_PER_KEY)
        {
        case APP_eReg_K0_Mode:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_K0_Mode + index * NUM_EEBYTES_PER_KEY]);
            break;

        case APP_eReg_K0_RemoteAddr:
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_K0_RemoteAddr + index * NUM_EEBYTES_PER_KEY]);
            *preg_type = eRegType_U16;
            break;

        case APP_eReg_K0_RemoteReg:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_K0_RemoteReg + index * NUM_EEBYTES_PER_KEY]);
            break;

        case APP_eReg_K0_Value:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_K0_Value + index * NUM_EEBYTES_PER_KEY]);
            break;

        case APP_eReg_K0_Reserved0:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_K0_Reserved0 + index * NUM_EEBYTES_PER_KEY]);
            break;

        case APP_eReg_K0_Reserved1:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_K0_Reserved1 + index * NUM_EEBYTES_PER_KEY]);
            break;

        case APP_eReg_K0_Reserved2:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_K0_Reserved2 + index * NUM_EEBYTES_PER_KEY]);
            break;

        case APP_eReg_K0_Reserved3:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_K0_Reserved3 + index * NUM_EEBYTES_PER_KEY]);
            break;

        default:
            break;
        }
    }
    else if (reg_no >= APP_eReg_RecMsg0_RemoteAddr &&
             reg_no < (APP_eReg_RecMsg0_RemoteAddr + APP_NUM_MSG_STUBS * NUM_REGS_PER_STUB))
    {
        index = (reg_no - APP_eReg_RecMsg0_RemoteAddr) / NUM_REGS_PER_STUB;

        switch (reg_no - index * NUM_REGS_PER_STUB)
        {
        case APP_eReg_RecMsg0_RemoteAddr:
         *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_RecMsg0_RemoteAddr + index * NUM_EEBYTES_PER_STUB]);
         *preg_type = eRegType_U16;
         break;

        case APP_eReg_RecMsg0_Command:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_RecMsg0_Command + index * NUM_EEBYTES_PER_STUB]);
            break;

        case APP_eReg_RecMsg0_Register:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_RecMsg0_Register + index * NUM_EEBYTES_PER_STUB]);
            break;

        case APP_eReg_RecMsg0_LEDFunc:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_RecMsg0_LEDFunc + index * NUM_EEBYTES_PER_STUB]);
            break;

        case APP_eReg_RecMsg0_AddInfo:
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_RecMsg0_AddInfo + index * NUM_EEBYTES_PER_STUB]);
            break;

        default:
            break;
        }
    }
    return true;
}


void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{

    uint16_t    tempval16;
    uint8_t     tempval8, index;

    tempval16 = (uint16_t)(value & 0x0000FFFF);
    tempval8  = (uint8_t) (value & 0x000000FF);

    //TODO implement
    // registers saved in EEProm
    if (reg_no >= APP_eReg_K0_Mode &&
        reg_no < (APP_eReg_K0_Mode + APP_NUM_KEY_STUBS * NUM_REGS_PER_KEY))
    {
        index = (reg_no - APP_eReg_K0_Mode) / NUM_REGS_PER_KEY;

        switch (reg_no - index * NUM_REGS_PER_KEY)
        {
        case APP_eReg_K0_Mode:
            g_on_key_set_register[index].key_mode = tempval8;
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_K0_Mode + index * NUM_EEBYTES_PER_KEY], tempval8);
            break;

        case APP_eReg_K0_RemoteAddr:
            g_on_key_set_register[index].receiver = tempval16;
            eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_K0_RemoteAddr + index * NUM_EEBYTES_PER_KEY], tempval16);
            break;

        case APP_eReg_K0_RemoteReg:
            g_on_key_set_register[index].register_id = tempval8;
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_K0_RemoteReg + index * NUM_EEBYTES_PER_KEY], tempval8);
            break;

        case APP_eReg_K0_Value:
            g_on_key_set_register[index].value = tempval8;
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_K0_Value + index * NUM_EEBYTES_PER_KEY], tempval8);
            break;

        case APP_eReg_K0_Reserved0:
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_K0_Reserved0 + index * NUM_EEBYTES_PER_KEY], tempval8);
            break;

        case APP_eReg_K0_Reserved1:
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_K0_Reserved1 + index * NUM_EEBYTES_PER_KEY], tempval8);
            break;

        case APP_eReg_K0_Reserved2:
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_K0_Reserved2 + index * NUM_EEBYTES_PER_KEY], tempval8);
            break;

        case APP_eReg_K0_Reserved3:
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_K0_Reserved3 + index * NUM_EEBYTES_PER_KEY], tempval8);
            break;

        default:
            break;
        }
    }
    else if (reg_no >= APP_eReg_RecMsg0_RemoteAddr &&
             reg_no < (APP_eReg_RecMsg0_RemoteAddr + APP_NUM_MSG_STUBS * NUM_REGS_PER_STUB))
    {
        index = (reg_no - APP_eReg_RecMsg0_RemoteAddr) / NUM_REGS_PER_STUB;

        switch (reg_no - index * NUM_REGS_PER_STUB)
        {
        case APP_eReg_RecMsg0_RemoteAddr:
            g_on_msg_received[index].receiver = tempval16;
            eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_RecMsg0_RemoteAddr + index * NUM_EEBYTES_PER_STUB], tempval16);
            break;

        case APP_eReg_RecMsg0_Command:
            g_on_msg_received[index].command = tempval8;
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_RecMsg0_Command + index * NUM_EEBYTES_PER_STUB], tempval8);
            break;

        case APP_eReg_RecMsg0_Register:
            g_on_msg_received[index].register_id = tempval8;
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_RecMsg0_Register + index * NUM_EEBYTES_PER_STUB], tempval8);
            break;

        case APP_eReg_RecMsg0_LEDFunc:
            g_on_msg_received[index].func_id = tempval8;
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_RecMsg0_LEDFunc + index * NUM_EEBYTES_PER_STUB], tempval8);
            break;

        case APP_eReg_RecMsg0_AddInfo:
            g_on_msg_received[index].add_info = tempval8;
            eeprom_write_byte(&register_eeprom_array[APP_eCfg_RecMsg0_AddInfo + index * NUM_EEBYTES_PER_STUB], tempval8);
            break;

        default:
            break;
        }
    }
    else
    {
        switch(reg_no)
        {
        case APP_eReg_0_LEDState:
            if(tempval8 == 0) led_switch(0, eLED_off);
            else              led_switch(0, eLED_on);
            break;
        case APP_eReg_1_LEDState:
            if(tempval8 == 0) led_switch(1, eLED_off);
            else              led_switch(1, eLED_on);
            break;
        case APP_eReg_2_LEDState:
            if(tempval8 == 0) led_switch(2, eLED_off);
            else              led_switch(2, eLED_on);
            break;
        case APP_eReg_3_LEDState:
            if(tempval8 == 0) led_switch(3, eLED_off);
            else              led_switch(3, eLED_on);
            break;
        case APP_eReg_4_LEDState:
            if(tempval8 == 0) led_switch(4, eLED_off);
            else              led_switch(4, eLED_on);
            break;
        case APP_eReg_5_LEDState:
            if(tempval8 == 0) led_switch(5, eLED_off);
            else              led_switch(5, eLED_on);
            break;
        case APP_eReg_6_LEDState:
            if(tempval8 == 0) led_switch(6, eLED_off);
            else              led_switch(6, eLED_on);
            break;
        case APP_eReg_7_LEDState:
            if(tempval8 == 0) led_switch(7, eLED_off);
            else              led_switch(7, eLED_on);
            break;
        default:
            break;
        }
    }
}

/** @} */
