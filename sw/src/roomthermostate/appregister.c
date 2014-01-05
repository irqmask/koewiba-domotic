/**
 * @addtogroup ROOMTHERMOSTATE
 * @addtogroup APPREGISTER
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

#include "register.h"

#include "appconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

uint8_t g_uEEProm[APP_eCfg_Last] EEMEM;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

uint32_t REG__uGetRegister              (uint8_t                uRegNo,
                                         eRegType_t*         peRegType)
{
    uint32_t retval = 0;
    eRegType_t regtype;

    if (peRegType == NULL) {
        peRegType = &regtype;
    }
    *peRegType = eRegType_Unkown;

    switch (uRegNo) {
    // registers saved in EEProm
    case MOD_eReg_ModuleID:
        retval = eeprom_read_word((uint16_t*)&g_uEEProm[MOD_eCfg_ModuleID]);
        *peRegType = eRegType_U16;
        break;
    case MOD_eReg_BldFlag:
        retval = eeprom_read_byte(&g_uEEProm[MOD_eCfg_BldFlag]);
        *peRegType = eRegType_U8;
        break;

    case APP_eReg_DesiredTempDay1:
        retval = eeprom_read_word((uint16_t*)&g_uEEProm[APP_eCfg_DesiredTempDay1]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempNight1:
        retval = eeprom_read_word((uint16_t*)&g_uEEProm[APP_eCfg_DesiredTempNight1]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempDay2:
        retval = eeprom_read_word((uint16_t*)&g_uEEProm[APP_eCfg_DesiredTempDay2]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempNight2:
        retval = eeprom_read_word((uint16_t*)&g_uEEProm[APP_eCfg_DesiredTempNight2]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempAway:
        retval = eeprom_read_word((uint16_t*)&g_uEEProm[APP_eCfg_DesiredTempAway]);
        *peRegType = eRegType_U16;
        break;
    case APP_eReg_DesiredTempWindowOpened:
        retval = eeprom_read_word((uint16_t*)&g_uEEProm[APP_eCfg_DesiredTempWindowOpened]);
        *peRegType = eRegType_U16;
        break;

    // registers in ROM/RAM
    case MOD_eReg_DeviceSignature0:
        retval = boot_signature_byte_get(ADDR_SIGNATURE_BYTE0);
        *peRegType = eRegType_U8;
        break;
    case MOD_eReg_DeviceSignature1:
        retval = boot_signature_byte_get(ADDR_SIGNATURE_BYTE1);
        *peRegType = eRegType_U8;
        break;
    case MOD_eReg_DeviceSignature2:
        retval = boot_signature_byte_get(ADDR_SIGNATURE_BYTE2);
        *peRegType = eRegType_U8;
        break;
    case MOD_eReg_BoardID:
        //TODO CV: implement
        *peRegType = eRegType_U8;
        break;
    case MOD_eReg_AppID:
        //TODO CV: implement
        *peRegType = eRegType_U8;
        break;
    case MOD_eReg_AppVersionMajor:
        //TODO CV: implement
        *peRegType = eRegType_U8;
        break;
    case MOD_eReg_AppVersionMinor:
        //TODO CV: implement
        *peRegType = eRegType_U8;
        break;
    case MOD_eReg_AppVersionBugfix:
        //TODO CV: implement
        *peRegType = eRegType_U8;
        break;

    default:
        // nothing (0) will be returned
        break;
    }
    return retval;
}

void REG__vSetRegister              (uint8_t                uRegNo,
                                        uint32_t               uValue)
{
    uint16_t tempval16;
    uint8_t tempval;

    switch (uRegNo) {
    // registers saved in EEProm
    case MOD_eReg_ModuleID:
        tempval16 = (uint16_t)(uValue & 0x0000FFFF);
        eeprom_write_word((uint16_t*)&g_uEEProm[MOD_eCfg_ModuleID], tempval16);
        break;
    case MOD_eReg_BldFlag:
        tempval = (uint8_t)(uValue & 0x000000FF);
        eeprom_write_byte(&g_uEEProm[MOD_eCfg_BldFlag], tempval);
        break;

    case APP_eReg_DesiredTempDay1:
        //TODO CV: implement
        break;
    case APP_eReg_DesiredTempNight1:
        //TODO CV: implement
        break;
    case APP_eReg_DesiredTempDay2:
        //TODO CV: implement
        break;
    case APP_eReg_DesiredTempNight2:
        //TODO CV: implement
        break;
    case APP_eReg_DesiredTempAway:
        //TODO CV: implement
        break;
    case APP_eReg_DesiredTempWindowOpened:
        //TODO CV: implement
        break;

    // registers in ROM/RAM
    case MOD_eReg_DeviceSignature0:
        // read only!
        break;
    case MOD_eReg_DeviceSignature1:
        // read only!
        break;
    case MOD_eReg_DeviceSignature2:
        // read only!
        break;
    case MOD_eReg_BoardID:
        // read only!
        break;
    case MOD_eReg_AppID:
        // read only!
        break;
    case MOD_eReg_AppVersionMajor:
        // read only!
        break;
    case MOD_eReg_AppVersionMinor:
        // read only!
        break;
    case MOD_eReg_AppVersionBugfix:
        // read only!
        break;

    default:
        // nothing (0) will be returned
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */