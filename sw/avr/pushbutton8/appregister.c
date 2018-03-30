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
#include "application.h"
#include "ledskeys.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t ram_reg_led_state[8];
static uint8_t ram_reg_target_chn_0[8];
static uint8_t ram_reg_target_chn_1[8];
static uint8_t ram_reg_target_chn_2[8];
static uint8_t ram_reg_remote_chn_0[8];
static uint8_t ram_reg_remote_chn_1[8];
static uint8_t ram_reg_remote_chn_2[8];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

bool        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue)
{
    eRegType_t  regtype;
    uint8_t index;

    if (peRegType == NULL) peRegType = &regtype;
    if (pvValue == NULL) return false;
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
    else if (uRegNo >= APP_eReg_0_Mode && uRegNo <= APP_eReg_7_Mode) {
        index = uRegNo - APP_eReg_0_Mode;
        index += APP_eCfg_0_Mode;
        *(uint8_t*)pvValue = eeprom_read_byte(&register_eeprom_array[index]);
    }
    else if (uRegNo >= APP_eReg_0_RemoteAddr0 && uRegNo <= APP_eReg_7_RemoteAddr0) {
        index = uRegNo - APP_eReg_0_RemoteAddr0;
        index += APP_eCfg_0_RemoteAddr0;
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[index]);
        *peRegType = eRegType_U16;
    }
    else if (uRegNo >= APP_eReg_0_RemoteAddr1 && uRegNo <= APP_eReg_7_RemoteAddr1) {
        index = uRegNo - APP_eReg_0_RemoteAddr1;
        index += APP_eCfg_0_RemoteAddr1;
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[index]);
        *peRegType = eRegType_U16;
    }
    //  registers in ROM/RAM
    else if (uRegNo >= APP_eReg_0_LEDState && uRegNo <= APP_eReg_7_LEDState) {
        index = uRegNo - APP_eReg_0_LEDState;
        *(uint8_t*)pvValue = ram_reg_led_state[index];
    }
    else if (uRegNo >= APP_eReg_0_TgtChn0 && uRegNo <= APP_eReg_7_TgtChn0) {
        index = uRegNo - APP_eReg_0_TgtChn0;
        *(uint8_t*)pvValue = ram_reg_target_chn_0[index];
    }
    else if (uRegNo >= APP_eReg_0_TgtChn1 && uRegNo <= APP_eReg_7_TgtChn1) {
        index = uRegNo - APP_eReg_0_TgtChn1;
        *(uint8_t*)pvValue = ram_reg_target_chn_1[index];
    }
    else if (uRegNo >= APP_eReg_0_TgtChn2 && uRegNo <= APP_eReg_7_TgtChn2) {
        index = uRegNo - APP_eReg_0_TgtChn2;
        *(uint8_t*)pvValue = ram_reg_target_chn_2[index];
    }
    else if (uRegNo >= APP_eReg_0_RemChn0 && uRegNo <= APP_eReg_7_RemChn0) {
        index = uRegNo - APP_eReg_0_RemChn0;
        *(uint8_t*)pvValue = ram_reg_remote_chn_0[index];
    }
    else if (uRegNo >= APP_eReg_0_RemChn1 && uRegNo <= APP_eReg_7_RemChn1) {
        index = uRegNo - APP_eReg_0_RemChn1;
        *(uint8_t*)pvValue = ram_reg_remote_chn_1[index];
    }
    else if (uRegNo >= APP_eReg_0_RemChn2 && uRegNo <= APP_eReg_7_RemChn2) {
        index = uRegNo - APP_eReg_0_RemChn2;
        *(uint8_t*)pvValue = ram_reg_remote_chn_2[index];
    }
    else {
        return false;
    }
    return true;
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
    else if (uRegNo >= APP_eReg_0_Mode && uRegNo <= APP_eReg_7_Mode) {
        index = uRegNo - APP_eReg_0_Mode;
        index += APP_eCfg_0_Mode;
        eeprom_write_byte(&register_eeprom_array[index], tempval);
        app_initialize_modes();
    }
    else if (uRegNo >= APP_eReg_0_RemoteAddr0 && uRegNo <= APP_eReg_7_RemoteAddr0) {
        index = uRegNo - APP_eReg_0_RemoteAddr0;
        index += APP_eCfg_0_RemoteAddr0;
        eeprom_write_word((uint16_t*)&register_eeprom_array[index], tempval16);
    }
    else if (uRegNo >= APP_eReg_0_RemoteAddr1 && uRegNo <= APP_eReg_7_RemoteAddr1) {
        index = uRegNo - APP_eReg_0_RemoteAddr1;
        index += APP_eCfg_0_RemoteAddr1;
        eeprom_write_word((uint16_t*)&register_eeprom_array[index], tempval16);
    }

    // registers in ROM/RAM
    else if (uRegNo >= APP_eReg_0_LEDState && uRegNo <= APP_eReg_7_LEDState) {
        index = uRegNo - APP_eReg_0_LEDState;
        ram_reg_led_state[index] = tempval;
        led_switch(index, tempval);
    }
    else if (uRegNo >= APP_eReg_0_TgtChn0 && uRegNo <= APP_eReg_7_TgtChn0) {
        index = uRegNo - APP_eReg_0_TgtChn0;
        ram_reg_target_chn_0[index] = tempval;
    }
    else if (uRegNo >= APP_eReg_0_TgtChn1 && uRegNo <= APP_eReg_7_TgtChn1) {
        index = uRegNo - APP_eReg_0_TgtChn1;
        ram_reg_target_chn_1[index] = tempval;
    }
    else if (uRegNo >= APP_eReg_0_TgtChn2 && uRegNo <= APP_eReg_7_TgtChn2) {
        index = uRegNo - APP_eReg_0_TgtChn2;
        ram_reg_target_chn_2[index] = tempval;
    }
    else if (uRegNo >= APP_eReg_0_RemChn0 && uRegNo <= APP_eReg_7_RemChn0) {
        index = uRegNo - APP_eReg_0_RemChn0;
        ram_reg_remote_chn_0[index] = tempval;
    }
    else if (uRegNo >= APP_eReg_0_RemChn1 && uRegNo <= APP_eReg_7_RemChn1) {
        index = uRegNo - APP_eReg_0_RemChn1;
        ram_reg_remote_chn_1[index] = tempval;
    }
    else if (uRegNo >= APP_eReg_0_RemChn2 && uRegNo <= APP_eReg_7_RemChn2) {
        index = uRegNo - APP_eReg_0_RemChn2;
        ram_reg_remote_chn_2[index] = tempval;
    }
}

/** @} */
