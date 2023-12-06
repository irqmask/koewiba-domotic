/**
 * @addtogroup TASTER8
 * @addtogroup APPREGISTER
 * @brief Registers of the application taster8.
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application taster8.
 *
 * @author  Christian Verhalen
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

extern app_on_key_set_register_t g_on_key_set_register[APP_NUM_KEYS];

// --- Global functions --------------------------------------------------------

void        app_register_load       (void)
{
    uint8_t index, eepindex;

    for (index=0; index<APP_NUM_KEYS; index++) {
        eepindex = APP_eCfg_K0_Mode + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].key_mode = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_K0_RemoteAddr + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].receiver = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_K0_RemoteReg + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].register_id = eeprom_read_byte(&register_eeprom_array[eepindex]);

        eepindex = APP_eCfg_K0_Value + index * NUM_EEBYTES_PER_KEY;
        g_on_key_set_register[index].value = eeprom_read_byte(&register_eeprom_array[eepindex]);
    }
}

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            reg_type,
                                     void*                  value)
{
    eRegType_t  rt;
//    uint8_t index;

    if (reg_type == NULL) reg_type = &rt;
    *reg_type = eRegType_U8;
    if (value == NULL) return false;

    if (reg_no >= APP_eCfg_K0_Mode &&
        reg_no < (APP_eCfg_K0_Mode + APP_NUM_KEYS * NUM_REGS_PER_KEY)) {
//        index = (reg_no - APP_eCfg_K0_Mode) / NUM_REGS_PER_KEY;

        //switch (reg_no)
    }


#if 0
    // registers saved in EEProm
    if (uRegNo >= APP_eReg_RemoteAddr00 && uRegNo <= APP_eReg_RemoteAddr31) {
        index = (uRegNo - APP_eReg_RemoteAddr00) * 2;
        index += APP_eCfg_RemoteAddr00;
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[index]);
        *peRegType = eRegType_U16;
    }
#endif
    return true;
}


void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
#if 0
    uint16_t    tempval16;
    uint8_t     tempval, index;

    tempval16 = (uint16_t)(uValue & 0x0000FFFF);
    tempval = (uint8_t)(uValue & 0x000000FF);

    //TODO implement
    // registers saved in EEProm
    if (uRegNo >= APP_eReg_RemoteAddr00 && uRegNo <= APP_eReg_RemoteAddr31) {
        index = (uRegNo - APP_eReg_RemoteAddr00) * 2;
        index += APP_eCfg_RemoteAddr00;
        eeprom_write_word((uint16_t*)&register_eeprom_array[index], tempval16);
    }
#endif

    switch(reg_no)
    {
    case 17:
    	if(value == 0)	led_switch(0, eLED_off);
    	else 			led_switch(0, eLED_blink_slow);
    	break;
    case 25:
    	if(value == 0)	led_switch(1, eLED_off);
    	else 			led_switch(1, eLED_on);
    	break;
    case 33:
    	if(value == 0)	led_switch(2, eLED_off);
    	else 			led_switch(2, eLED_blink_fast);
    	break;
    case 41:
    	if(value == 0)	led_switch(3, eLED_off);
    	else 			led_switch(3, eLED_on);
    	break;
    default:
    	break;
    }
}

/** @} */
