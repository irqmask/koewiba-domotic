/**
 * @addtogroup PWM-DIMMER-3CHN
 * @addtogroup PWM-DIMMER-3CHN_APPREGISTER
 * @brief Registers of the application roomthermostate.
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application roomthermostate.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/boot.h>
#include <avr/eeprom.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "moddef_common.h"

#include "pwm.h"
#include "register.h"

#include "appconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

bool        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue)
{
    eRegType_t  regtype;
    uint8_t index;

    if (peRegType == NULL) peRegType = &regtype;
    if (pvValue == NULL) return FALSE;
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
    else switch (uRegNo) {
    // registers saved in EEProm

    // registers in ROM/RAM

    default:
        return FALSE;
        break;
    }
    return TRUE;
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
    else switch (uRegNo) {
    // registers saved in EEProm
    case APP_eReg_PWMChn1:
        tempval = (uint8_t)(uValue & 0x000000FF);
        pwm_set(0,tempval);
        break;
    case APP_eReg_PWMChn2:
        tempval = (uint8_t)(uValue & 0x000000FF);
        pwm_set(1,tempval);
        break;
    case APP_eReg_PWMChn3:
        tempval = (uint8_t)(uValue & 0x000000FF);
        pwm_set(2,tempval);
        break;

    // registers in ROM/RAM

    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
