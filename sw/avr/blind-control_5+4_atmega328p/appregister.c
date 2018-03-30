/**
 * @addtogroup BLINDCONTROL_5+4_ATMEGA328
 * @addtogroup APPREGISTER
 * @brief Registers of the application "blindcontrol_5+4_atmega328p".
 *
 * @{
 * @file    appregister.h
 * @brief   Registers of the application "blindcontrol_5+4_atmega328p".
 *
 * The application specific registers are like this:
 * APP_eReg_B0_PositionCurrent     | Current position of blind 0. 100 = completely closed. 0 = completely open
 * APP_eReg_B0_PositionSetPoint    | Wanted position of blind. 100 = completely closed. 0 = completely open
 * APP_eReg_B0_PositionDifference  | Difference between wanted and current position.
 * APP_eReg_B0_ReactionDelay       | Delay time in timer ticks (1/100sec) until movement starts after applying power.
 * APP_eReg_B0_DurationOpen        | Duration in timer ticks (1/100sec) to fully open the blind.
 * APP_eReg_B0_DurationClose       | Duration in timer ticks (1/100sec) to fully close the blind.
 * APP_eReg_B0_Reserve2            | unused.
 * APP_eReg_B0_Reserve1            | unused.
 *
 * APP_eReg_B1_PositionCurrent     | Current position of blind 1. 100 = completely closed. 0 = completely open
 * APP_eReg_B1_PositionSetPoint    | Wanted position of blind. 100 = completely closed. 0 = completely open
 * APP_eReg_B1_PositionDifference  | Difference between wanted and current position.
 * APP_eReg_B1_ReactionDelay       | Delay time in timer ticks (1/100sec) until movement starts after applying power.
 * APP_eReg_B1_DurationOpen        | Duration in timer ticks (1/100sec) to fully open the blind.
 * APP_eReg_B1_DurationClose       | Duration in timer ticks (1/100sec) to fully close the blind.
 * APP_eReg_B1_Reserve2            | unused.
 * APP_eReg_B1_Reserve1            | unused.
 *
 * APP_eReg_B2_PositionCurrent     | Current position of blind 2. 100 = completely closed. 0 = completely open
 * APP_eReg_B2_PositionSetPoint    | Wanted position of blind. 100 = completely closed. 0 = completely open
 * APP_eReg_B2_PositionDifference  | Difference between wanted and current position.
 * APP_eReg_B2_ReactionDelay       | Delay time in timer ticks (1/100sec) until movement starts after applying power.
 * APP_eReg_B2_DurationOpen        | Duration in timer ticks (1/100sec) to fully open the blind.
 * APP_eReg_B2_DurationClose       | Duration in timer ticks (1/100sec) to fully close the blind.
 * APP_eReg_B2_Reserve2            | unused.
 * APP_eReg_B2_Reserve1            | unused.
 *
 * APP_eReg_B3_PositionCurrent     | Current position of blind 2. 100 = completely closed. 0 = completely open
 * APP_eReg_B3_PositionSetPoint    | Wanted position of blind. 100 = completely closed. 0 = completely open
 * APP_eReg_B3_PositionDifference  | Difference between wanted and current position.
 * APP_eReg_B3_ReactionDelay       | Delay time in timer ticks (1/100sec) until movement starts after applying power.
 * APP_eReg_B3_DurationOpen        | Duration in timer ticks (1/100sec) to fully open the blind.
 * APP_eReg_B3_DurationClose       | Duration in timer ticks (1/100sec) to fully close the blind.
 * APP_eReg_B3_Reserve2            | unused.
 * APP_eReg_B3_Reserve1            | unused.
 *
 * APP_eReg_B4_PositionCurrent     | Current position of blind 2. 100 = completely closed. 0 = completely open
 * APP_eReg_B4_PositionSetPoint    | Wanted position of blind. 100 = completely closed. 0 = completely open
 * APP_eReg_B4_PositionDifference  | Difference between wanted and current position.
 * APP_eReg_B4_ReactionDelay       | Delay time in timer ticks (1/100sec) until movement starts after applying power.
 * APP_eReg_B4_DurationOpen        | Duration in timer ticks (1/100sec) to fully open the blind.
 * APP_eReg_B4_DurationClose       | Duration in timer ticks (1/100sec) to fully close the blind.
 * APP_eReg_B4_Reserve2            | unused.
 * APP_eReg_B4_Reserve1            | unused.
 *
 * APP_eReg_B0_TimeUp1_Weekday
 * APP_eReg_B0_TimeUp1_Hour
 * APP_eReg_B0_TimeUp1_Minute
 * APP_eReg_B0_TimeDown1_Weekday
 * APP_eReg_B0_TimeDown1_Hour
 * APP_eReg_B0_TimeDown1_Minute
 * APP_eReg_B0_TimeUp2_Weekday
 * APP_eReg_B0_TimeUp2_Hour
 * APP_eReg_B0_TimeUp2_Minute
 * APP_eReg_B0_TimeDown2_Weekday
 * APP_eReg_B0_TimeDown2_Hour
 * APP_eReg_B0_TimeDown2_Minute
 *
 * APP_eReg_B1_TimeUp1_Weekday
 * APP_eReg_B1_TimeUp1_Hour
 * APP_eReg_B1_TimeUp1_Minute
 * APP_eReg_B1_TimeDown1_Weekday
 * APP_eReg_B1_TimeDown1_Hour
 * APP_eReg_B1_TimeDown1_Minute
 * APP_eReg_B1_TimeUp2_Weekday
 * APP_eReg_B1_TimeUp2_Hour
 * APP_eReg_B1_TimeUp2_Minute
 * APP_eReg_B1_TimeDown2_Weekday
 * APP_eReg_B1_TimeDown2_Hour
 * APP_eReg_B1_TimeDown2_Minute
 *
 * APP_eReg_B2_TimeUp1_Weekday
 * APP_eReg_B2_TimeUp1_Hour
 * APP_eReg_B2_TimeUp1_Minute
 * APP_eReg_B2_TimeDown1_Weekday
 * APP_eReg_B2_TimeDown1_Hour
 * APP_eReg_B2_TimeDown1_Minute
 * APP_eReg_B2_TimeUp2_Weekday
 * APP_eReg_B2_TimeUp2_Hour
 * APP_eReg_B2_TimeUp2_Minute
 * APP_eReg_B2_TimeDown2_Weekday
 * APP_eReg_B2_TimeDown2_Hour
 * APP_eReg_B2_TimeDown2_Minute
 *
 * APP_eReg_B3_TimeUp1_Weekday
 * APP_eReg_B3_TimeUp1_Hour
 * APP_eReg_B3_TimeUp1_Minute
 * APP_eReg_B3_TimeDown1_Weekday
 * APP_eReg_B3_TimeDown1_Hour
 * APP_eReg_B3_TimeDown1_Minute
 * APP_eReg_B3_TimeUp2_Weekday
 * APP_eReg_B3_TimeUp2_Hour
 * APP_eReg_B3_TimeUp2_Minute
 * APP_eReg_B3_TimeDown2_Weekday
 * APP_eReg_B3_TimeDown2_Hour
 * APP_eReg_B3_TimeDown2_Minute
 *
 * APP_eReg_B4_TimeUp1_Weekday
 * APP_eReg_B4_TimeUp1_Hour
 * APP_eReg_B4_TimeUp1_Minute
 * APP_eReg_B4_TimeDown1_Weekday
 * APP_eReg_B4_TimeDown1_Hour
 * APP_eReg_B4_TimeDown1_Minute
 * APP_eReg_B4_TimeUp2_Weekday
 * APP_eReg_B4_TimeUp2_Hour
 * APP_eReg_B4_TimeUp2_Minute
 * APP_eReg_B4_TimeDown2_Weekday
 * APP_eReg_B4_TimeDown2_Hour
 * APP_eReg_B4_TimeDown2_Minute
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
    uint8_t index, eepindex;

    for (index=0; index<BLIND_COUNT; index++) {
        eepindex = APP_eCfg_B0_ReactionDelay + index * NUM_EEBYTES_PER_BLIND;
        blind_set_reaction_delay(index, eeprom_read_byte(&register_eeprom_array[eepindex]));
        eepindex = APP_eCfg_B0_DurationOpen + index * NUM_EEBYTES_PER_BLIND;
        blind_set_duration_open(index, eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]));
        eepindex = APP_eCfg_B0_DurationClose + index * NUM_EEBYTES_PER_BLIND;
        blind_set_duration_close(index, eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]));
    }
}

bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            preg_type,
                                     void*                  pvalue)
{
    eRegType_t  regtype;
    uint8_t index=0, eepindex=0;

    if (preg_type == NULL) preg_type = &regtype;
    if (pvalue == NULL) return false;
    *preg_type = eRegType_U8;

    if (reg_no >= APP_eReg_B0_PositionCurrent &&
        reg_no < (APP_eReg_B0_PositionCurrent + BLIND_COUNT*APP_NUM_REGS_PER_BLIND)) {

        // calculate index of blind and normalize register number
        index = (reg_no - APP_eReg_B0_PositionCurrent) / APP_NUM_REGS_PER_BLIND;
        reg_no -= (index * APP_NUM_REGS_PER_BLIND);

        switch (reg_no) {
        // registers in ROM/RAM
        case APP_eReg_B0_PositionCurrent:
            *(uint8_t*)pvalue = blind_get_current_position(index);
            break;
        case APP_eReg_B0_PositionSetPoint:
            *(uint8_t*)pvalue = blind_get_position_setpoint(index);
            break;

        // registers saved in EEProm
        case APP_eReg_B0_ReactionDelay:
            eepindex = APP_eCfg_B0_ReactionDelay + index * NUM_EEBYTES_PER_BLIND;
            *(uint8_t*)pvalue = eeprom_read_byte(&register_eeprom_array[eepindex]);
            break;
        case APP_eReg_B0_DurationOpen:
            eepindex = APP_eCfg_B0_DurationOpen + index * NUM_EEBYTES_PER_BLIND;
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);
            *preg_type = eRegType_U16;
            break;
        case APP_eReg_B0_DurationClose:
            eepindex = APP_eCfg_B0_DurationClose + index * NUM_EEBYTES_PER_BLIND;
            *(uint16_t*)pvalue = eeprom_read_word((uint16_t*)&register_eeprom_array[eepindex]);
            *preg_type = eRegType_U16;
            break;

        default:
            return false;
            break;
        }
        return true;
    }
    return false;
}

void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t value16;
    uint8_t value8, index=0, eepindex=0;

    if (reg_no >= APP_eReg_B0_PositionCurrent &&
        reg_no < (APP_eReg_B0_PositionCurrent + BLIND_COUNT*APP_NUM_REGS_PER_BLIND)) {

        // calculate index of blind and normalize register number
        index = (reg_no - APP_eReg_B0_PositionCurrent) / APP_NUM_REGS_PER_BLIND;
        reg_no -= (index * APP_NUM_REGS_PER_BLIND);

        // registers in ROM/RAM
        switch (reg_no) {
        case APP_eReg_B0_PositionCurrent:
            // read only
            break;
        case APP_eReg_B0_PositionSetPoint:
            blind_move_to_position(index, value & 0x000000FF);
            break;

        // registers saved in EEProm
        case APP_eReg_B0_ReactionDelay:
            value8 = value & 0x000000FF;
            blind_set_reaction_delay(index, value8);
            eepindex = APP_eCfg_B0_ReactionDelay + index * NUM_EEBYTES_PER_BLIND;
            eeprom_write_byte(&register_eeprom_array[eepindex], value8);
            break;
        case APP_eReg_B0_DurationOpen:
            value16 = value & 0x0000FFFF;
            blind_set_duration_open(index, value16);
            eepindex = APP_eCfg_B0_DurationOpen + index * NUM_EEBYTES_PER_BLIND;
            eeprom_write_word((uint16_t*)&register_eeprom_array[eepindex], value16);
            break;
        case APP_eReg_B0_DurationClose:
            value16 = value & 0x0000FFFF;
            blind_set_duration_close(index, value16);
            eepindex = APP_eCfg_B0_DurationClose + index * NUM_EEBYTES_PER_BLIND;
            eeprom_write_word((uint16_t*)&register_eeprom_array[eepindex], value16);
            break;
        default:
            break;
        }
    }
}

// --- Global functions --------------------------------------------------------

/** @} */
