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

#include "appconfig.h"
#include "blindctrl.h"
#include "moddef_common.h"
#include "prjtypes.h"
#include "register.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

void        app_register_load       (void)
{
    blind_set_reaction_delay(eeprom_read_byte(&register_eeprom_array[APP_eCfg_ReactionDelay]));
    blind_set_duration_open(eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationOpen]));
    blind_set_duration_close(eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationClose]));
}

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    switch (reg_no) {
    // registers in ROM/RAM
    case APP_eReg_PositionCurrent:
        *(uint8_t*)pvalue = blind_get_current_position();
        break;
    case APP_eReg_PositionSetPoint:
        *(uint8_t*)pvalue = blind_get_position_setpoint();
        break;

    // registers saved in EEProm
    case APP_eReg_ReactionDelay:
        *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[APP_eCfg_ReactionDelay]);
        break;
    case APP_eReg_DurationOpen:
        *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationOpen]);
        *preg_type = eRegType_U16;
        break;
    case APP_eReg_DurationClose:
        *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationClose]);
        *preg_type = eRegType_U16;
        break;

    default:
        return false;
        break;
    }
    return true;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t value16;
    uint8_t value8;

    // registers in ROM/RAM
    switch (reg_no) {
    case APP_eReg_PositionCurrent:
        // read only
        break;
    case APP_eReg_PositionSetPoint:
        blind_move_to_position(value & 0x000000FF);
        break;

    // registers saved in EEProm
    case APP_eReg_ReactionDelay:
        value8 = value & 0x000000FF;
        blind_set_reaction_delay(value8);
        eeprom_write_byte(&register_eeprom_array[APP_eCfg_ReactionDelay], value8);
        break;
    case APP_eReg_DurationOpen:
        value16 = value & 0x0000FFFF;
        blind_set_duration_open(value16);
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationOpen], value16);
        break;
    case APP_eReg_DurationClose:
        value16 = value & 0x0000FFFF;
        blind_set_duration_close(value16);
        eeprom_write_word((uint16_t*)&register_eeprom_array[APP_eCfg_DurationClose], value16);
        break;
    default:
        break;
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
