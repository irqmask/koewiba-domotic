/**
 * @addtogroup ELEKTOR_BUS-NODE
 * @addtogroup APPREGISTER
 * @brief Registers of the application "elektor_bus-node".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "elektor_bus-node".
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
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

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
    // registers in ROM/RAM
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
    // registers in ROM/RAM
}

/** @} */
