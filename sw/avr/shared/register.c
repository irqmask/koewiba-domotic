/**
 * @addtogroup REGISTER
 * @brief Module to handle register read and write operations.
 *
 * @{
 * @file    register.h
 * @brief   Module to handle register read and write operations.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "prjtypes.h"
#include "cmddef_common.h"
#include "moddef_common.h"

#include "bus.h"
#include "register.h"
#include "ucontroller.h"

#include "appconfig.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

uint8_t register_eeprom_array[APP_eCfg_Last] EEMEM;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * This function answers a register request command.
 *
 * @param[in] uRegNo
 * Register number.
 * @param[in] uSender
 * Sender of the request message.
 */
static void answer_register_request (sBus_t*                bus,
                                     uint8_t                reg_no,
                                     uint16_t               sender)
{
    uint32_t    value;
    eRegType_t  regtype = eRegType_Unkown;

    if (register_get(reg_no, &regtype, &value)) {
        switch (regtype) {
        case eRegType_U8:
            register_send_u8(bus, sender, reg_no, (uint8_t)(value & 0x000000FF));
            break;
        case eRegType_U16:
            register_send_u16(bus, sender, reg_no, (uint16_t)(value & 0x0000FFFF));
            break;
        case eRegType_U32:
            register_send_u32(bus, sender, reg_no, value);
            break;
        default:
            break;
        }
    }
}

// --- Module global functions -------------------------------------------------

extern bool app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            reg_type,
                                     void*                  value);

extern void app_register_set        (uint8_t                reg_no,
                                     uint32_t               value);

// --- Global functions --------------------------------------------------------

bool        register_get            (uint8_t                reg_no,
                                     eRegType_t*            reg_type,
                                     void*                  value)
{
    eRegType_t  l_regtype;

    if (reg_type == NULL) reg_type = &l_regtype;
    if (value == NULL) return false;

    switch (reg_no) {
    // registers saved in EEProm
    case MOD_eReg_ModuleID:
        *reg_type = eRegType_U16;
        *(uint16_t*)value = eeprom_read_word((uint16_t*)&register_eeprom_array[MOD_eCfg_ModuleID]);
        break;

    case MOD_eReg_BldFlag:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = eeprom_read_byte(&register_eeprom_array[MOD_eCfg_BldFlag]);
        break;

    // registers in ROM/RAM
    case MOD_eReg_DeviceSignature0:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = boot_signature_byte_get(ADDR_SIGNATURE_BYTE0);
        break;

    case MOD_eReg_DeviceSignature1:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = boot_signature_byte_get(ADDR_SIGNATURE_BYTE1);
        break;

    case MOD_eReg_DeviceSignature2:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = boot_signature_byte_get(ADDR_SIGNATURE_BYTE2);
        break;

    case MOD_eReg_DeviceSignature3:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = 0;// not used yet // boot_signature_byte_get(ADDR_SIGNATURE_BYTE2);
        break;

    case MOD_eReg_BoardID:
        *reg_type = eRegType_U16;
        *(uint16_t*)value = pgm_read_byte(&app_versioninfo[MOD_eVerBoardIdHigh]);
        *(uint16_t*)value <<= 8;
        *(uint16_t*)value |= pgm_read_byte(&app_versioninfo[MOD_eVerBoardIdLow]);
        //*(uint16_t*)value = app_versioninfo[MOD_eVerBoardIdHigh];
        //*(uint16_t*)value <<= 8;
        //*(uint16_t*)value |= app_versioninfo[MOD_eVerBoardIdLow];
        break;

    case MOD_eReg_BoardRev:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = pgm_read_byte(&app_versioninfo[MOD_eVerBoardRev]);
        //*(uint8_t*)value = app_versioninfo[MOD_eVerBoardRev];
        break;

    case MOD_eReg_AppID:
        *reg_type = eRegType_U16;
        *(uint16_t*)value = pgm_read_byte(&app_versioninfo[MOD_eVerAppIdHigh]);
        *(uint16_t*)value <<= 8;
        *(uint16_t*)value |= pgm_read_byte(&app_versioninfo[MOD_eVerAppIdLow]);
        //*(uint16_t*)value = app_versioninfo[MOD_eVerAppIdHigh];
        //*(uint16_t*)value <<= 8;
        //*(uint16_t*)value |= app_versioninfo[MOD_eVerAppIdLow];
        break;

    case MOD_eReg_AppVersionMajor:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = pgm_read_byte(&app_versioninfo[MOD_eVerAppMajor]);
        //*(uint8_t*)value = app_versioninfo[MOD_eVerAppMajor];
        break;

    case MOD_eReg_AppVersionMinor:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = pgm_read_byte(&app_versioninfo[MOD_eVerAppMinor]);
        //*(uint8_t*)value = app_versioninfo[MOD_eVerAppMinor];
        break;

    case MOD_eReg_AppVersionBugfix:
        *reg_type = eRegType_U8;
        *(uint8_t*)value = pgm_read_byte(&app_versioninfo[MOD_eVerAppBugfix]);
        //*(uint8_t*)value = app_versioninfo[MOD_eVerAppBugfix];
        break;

    // application specific registers
    default:
        return app_register_get(reg_no, reg_type, value);
    }
    return true;
}

/**
 * Set a register value.
 *
 * @param[in] uRegNo    Register number to write to.
 * @param[in] uValue    Value to be set. (up to 32bit)
 *
 * @note If the value provided is greater than register width, e.g. uValue is
 * 16bit, register is 8bit, only the lower byte of uValue is used.
 */
void        register_set            (uint8_t                reg_no,
                                     uint32_t               value)
{
    uint16_t    tempval16;
    uint8_t     tempval;

    switch (reg_no) {
    // registers saved in EEProm
    case MOD_eReg_ModuleID:
        tempval16 = (uint16_t)(value & 0x0000FFFF);
        eeprom_write_word((uint16_t*)&register_eeprom_array[MOD_eCfg_ModuleID], tempval16);
        break;
    case MOD_eReg_BldFlag:
        tempval = (uint8_t)(value & 0x000000FF);
        eeprom_write_byte(&register_eeprom_array[MOD_eCfg_BldFlag], tempval);
        break;

    // registers in ROM/RAM

    // application registers
    default:
        app_register_set(reg_no, value);
        break;
    }
}

void        register_set_u8         (uint8_t                reg_no,
                                     uint8_t                value)
{
    register_set(reg_no, value);
}

void        register_set_u16        (uint8_t                reg_no,
                                     uint16_t               value)
{
    register_set(reg_no, value);
}

void        register_set_u32        (uint8_t                reg_no,
                                     uint32_t               value)
{
    register_set (reg_no, value);
}

/**
 * Command interpreter for register commands.
 */
void        register_do_command     (sBus_t*                bus,
                                     uint16_t               sender,
                                     uint8_t                msglen,
                                     uint8_t*               msg)
{
    uint32_t temp32;
    uint16_t temp16;

    // check command
    switch (msg[0]) {
    case eCMD_REQUEST_REG:
        // Format: <Command><Registernumber>
        if (msglen >= 2) {
            answer_register_request(bus, msg[1], sender);
        }
        break;
    case eCMD_SET_REG_8BIT:
        // Format: <Command><Registernumber><Value>
        if (msglen >= 3) {
            register_set_u8   (msg[1], msg[2]);
        }
        break;
    case eCMD_SET_REG_16BIT:
        // Format: <Command><Registernumber><HighValue><LowValue>
        if (msglen >= 4) {
            temp16 = (uint16_t)msg[2] << 8;
            temp16 |= msg[3];
            register_set_u16(msg[1], temp16);
        }
        break;
    case eCMD_SET_REG_32BIT:
        // Format: <Command><Registernumber><HighHighValue><HighValue><LowValue><LowLowValue>
        if (msglen >= 6) {
            temp32 = ((uint32_t)msg[2]<<24);
            temp32 |= ((uint32_t)msg[3]<<16);
            temp32 |= ((uint32_t)msg[4]<<8);
            temp32 |= msg[5];
            register_set_u32(msg[1], temp32);
        }
        break;
    default:
        break;
    }
}

/**
 * Send 8bit state command.
 *
 * @param[in] psBus
 * Pointer to bus structure.
 * @param[in] uReceiver
 * Receiver of the message.
 * @param[in] uRegNo
 * Register number.
 * @param[in] uValue
 * Value to encode to the message.
 */
void        register_send_u8        (sBus_t*                bus,
                                     uint16_t               receiver,
                                     uint8_t                reg_no,
                                     uint8_t                value)
{
    uint8_t msg[3];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = reg_no;
    msg[2] = value;
    bus_send_message(bus, receiver, sizeof(msg), msg);
}

/**
 * Send 16bit state command.
 *
 * @param[in] psBus
 * Pointer to bus structure.
 * @param[in] uReceiver
 * Receiver of the message.
 * @param[in] uRegNo
 * Register number.
 * @param[in] uValue
 * Value to encode to the message.
 */
void        register_send_u16       (sBus_t*                bus,
                                     uint16_t               receiver,
                                     uint8_t                reg_no,
                                     uint16_t               value)
{
    uint8_t msg[4];

    msg[0] = eCMD_STATE_16BIT;
    msg[1] = reg_no;
    msg[2] = value >> 8;
    msg[3] = value & 0x00FF;
    bus_send_message(bus, receiver, sizeof(msg), msg);
}

/**
 * Send 32bit state command.
 *
 * @param[in] psBus
 * Pointer to bus structure.
 * @param[in] uReceiver
 * Receiver of the message.
 * @param[in] uRegNo
 * Register number.
 * @param[in] uValue
 * Value to encode to the message.
 */
void        register_send_u32       (sBus_t*                bus,
                                     uint16_t               receiver,
                                     uint8_t                reg_no,
                                     uint32_t               value)
{
    uint8_t msg[6];

    msg[0] = eCMD_STATE_32BIT;
    msg[1] = reg_no;
    msg[2] = (value >> 24) & 0x000000FF;
    msg[3] = (value >> 16) & 0x000000FF;
    msg[4] = (value >> 8) & 0x000000FF;
    msg[5] = value & 0x000000FF;
    bus_send_message(bus, receiver, sizeof(msg), msg);
}

/**
 * Map incoming stated values to local registers.
 *
 * @param[in] uRemoteModuleAddr Module address of incoming state message.
 * @param[in] uRemoteRegister   Module register of incoming state message.
 * @param[in] uValue            Value to set.
 *
 * @returns true, if a register mapping for the received address and value
 *          was found.
 */
/*bool        register_do_mapping     (uint16_t               uRemoteModuleAddr,
                                     uint8_t                uRemoteRegister,
                                     uint32_t               uValue)
{
    uint16_t    addr;
    uint8_t     ii, reg, target_reg;
    bool        retval = false;

    for (ii=0; ii<REG_MAX_MAPPINGS; ii++) {
        if (!register_get(APP_eReg_RemoteAddr00 + ii, 0, &addr)) continue;
        if (addr == uRemoteModuleAddr) {
            if (!register_get(APP_eReg_RemoteReg00 + ii, 0, &reg)) continue;
            if (reg == uRemoteRegister) {
                if (!register_get(APP_eReg_TargetReg00 + ii, 0, &target_reg)) continue;
                register_set(target_reg, uValue);
                retval = true;
            }
        }
    }
    return retval;
}*/

/** @} */
