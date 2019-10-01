/**
 * @addtogroup LED2_ATMEGA324P
 * @addtogroup APPREGISTER
 * @brief Registers of the application "led2_atmega324p".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "led2_atmega324p".
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

#include "register.h"

#include "appconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    switch (reg_no) {
    // registers saved in EEProm
    // TODO add handler for with application specific registers here!

    // registers in ROM/RAM
    default:
        return false;
        break;
    }
    return true;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t    tempval16;
    uint8_t     tempval;

    tempval16 = (uint16_t)(value & 0x0000FFFF);
    tempval = (uint8_t)(value & 0x000000FF);

    switch (reg_no) {
    // registers saved in EEProm
    // TODO add handler for with application specific registers here!


    // registers in ROM/RAM

    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
