/**
 * @addtogroup BASIC_ATMEGA328
 * @addtogroup APPREGISTER
 * @brief Registers of the application "basic_app_atmega328".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "basic_app_atmega328".
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

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

bool        app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue)
{
    eRegType_t  regtype;

    if (peRegType == NULL) peRegType = &regtype;
    if (pvValue == NULL) return false;
    *peRegType = eRegType_U8;

    switch (uRegNo) {
    // registers saved in EEProm
    // TODO add handler for with application specific registers here!

    // registers in ROM/RAM
    default:
        return false;
        break;
    }
    return true;
}

void        app_register_set        (uint8_t                uRegNo,
                                     uint32_t               uValue)
{
   /* uint16_t    tempval16;
    uint8_t     tempval, index;

    tempval16 = (uint16_t)(uValue & 0x0000FFFF);
    tempval = (uint8_t)(uValue & 0x000000FF);

    switch (uRegNo) {
    // registers saved in EEProm
    // TODO add handler for with application specific registers here!


    // registers in ROM/RAM

    default:
        break;
    }*/
}

// --- Global functions --------------------------------------------------------

/** @} */
