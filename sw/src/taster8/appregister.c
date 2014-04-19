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

#include "ledskeys.h"
#include "register.h"

#include "appconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

BOOL        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue)
{
    eRegType_t  regtype;
    uint8_t index;

    if (peRegType == NULL) peRegType = &regtype;
    if (pvValue == NULL) return FALSE;

    switch (uRegNo) {
    case APP_eReg_0_Mode:
    case APP_eReg_1_Mode:
    case APP_eReg_2_Mode:
    case APP_eReg_3_Mode:
    case APP_eReg_4_Mode:
    case APP_eReg_5_Mode:
    case APP_eReg_6_Mode:
    case APP_eReg_7_Mode:
        index = uRegNo - APP_eReg_0_Mode;
        index += APP_eCfg_0_Mode;
        *(uint8_t*)pvValue = eeprom_read_byte(&register_eeprom_array[index]);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}


void        app_register_set        (uint8_t                uRegNo,
                                     uint32_t               uValue)
{


    switch (uRegNo) {
    // registers saved in EEProm

    // registers in ROM/RAM

    default:
        break;
    }
}

/** @} */
