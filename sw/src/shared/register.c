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
static void answer_register_request (sBus_t*                psBus,
                                     uint8_t                uRegNo,
                                     uint16_t               uSender)
{
    uint32_t    value;
    eRegType_t  regtype = eRegType_Unkown;

    value = register_get(uRegNo, &regtype, &value);
    switch (regtype) {
    case eRegType_U8:
        register_send_u8(psBus, uSender, uRegNo, (uint8_t)(value & 0x000000FF));
        break;
    case eRegType_U16:
        register_send_u16(psBus, uSender, uRegNo, (uint16_t)(value & 0x0000FFFF));
        break;
    case eRegType_U32:
        register_send_u32(psBus, uSender, uRegNo, value);
        break;
    default:
        break;
    }
}

// --- Module global functions -------------------------------------------------

extern BOOL app_register_get        (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue);
extern void app_register_set        (uint8_t                uRegNo,
                                     uint32_t               uValue);

// --- Global functions --------------------------------------------------------

BOOL        register_get            (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue)
{
    eRegType_t  regtype;

    if (peRegType == NULL) peRegType = &regtype;
    if (pvValue == NULL) return FALSE;

    switch (uRegNo) {
    // registers saved in EEProm
    case MOD_eReg_ModuleID:
        *peRegType = eRegType_U16;
        *(uint16_t*)pvValue = eeprom_read_word((uint16_t*)&register_eeprom_array[MOD_eCfg_ModuleID]);
        break;

    case MOD_eReg_BldFlag:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = eeprom_read_byte(&register_eeprom_array[MOD_eCfg_BldFlag]);
        break;

    // registers in ROM/RAM
    case MOD_eReg_DeviceSignature0:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = boot_signature_byte_get(ADDR_SIGNATURE_BYTE0);
        break;

    case MOD_eReg_DeviceSignature1:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = boot_signature_byte_get(ADDR_SIGNATURE_BYTE1);
        break;

    case MOD_eReg_DeviceSignature2:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = boot_signature_byte_get(ADDR_SIGNATURE_BYTE2);
        break;

    case MOD_eReg_BoardID:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = 0; //TODO CV: implement
        break;

    case MOD_eReg_AppID:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = 0; //TODO CV: implement
        break;

    case MOD_eReg_AppVersionMajor:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = 0; //TODO CV: implement
        break;

    case MOD_eReg_AppVersionMinor:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = 0; //TODO CV: implement
        break;

    case MOD_eReg_AppVersionBugfix:
        *peRegType = eRegType_U8;
        *(uint8_t*)pvValue = 0; //TODO CV: implement
        break;

    // application specific registers
    default:
        return app_register_get(uRegNo, peRegType, pvValue);
    }
    return TRUE;
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
void        register_set            (uint8_t                uRegNo,
                                     uint32_t               uValue)
{
    uint16_t    tempval16;
    uint8_t     tempval;

    switch (uRegNo) {
    // registers saved in EEProm
    case MOD_eReg_ModuleID:
        tempval16 = (uint16_t)(uValue & 0x0000FFFF);
        eeprom_write_word((uint16_t*)&register_eeprom_array[MOD_eCfg_ModuleID], tempval16);
        break;
    case MOD_eReg_BldFlag:
        tempval = (uint8_t)(uValue & 0x000000FF);
        eeprom_write_byte(&register_eeprom_array[MOD_eCfg_BldFlag], tempval);
        break;

    // registers in ROM/RAM

    // application registers
    default:
        app_register_set(uRegNo, uValue);
        break;
    }
}

void        register_set_u8         (uint8_t                uRegNo,
                                     uint8_t                uValue)
{
    register_set(uRegNo, uValue);
}

void        register_set_u16        (uint8_t                uRegNo,
                                     uint16_t               uValue)
{
    register_set(uRegNo, uValue);
}

void        register_set_u32        (uint8_t                uRegNo,
                                     uint32_t               uValue)
{
    register_set (uRegNo, uValue);
}

/**
 * Command interpreter for register commands.
 */
void        register_do_command     (sBus_t*                psBus,
                                     uint8_t*               puMsg,
                                     uint8_t                uMsgLen,
                                     uint16_t               uSender)
{
    uint32_t temp32;
    uint16_t temp16;

    // check command
    switch (puMsg[0]) {
    case eCMD_REQUEST_REG:
        // Format: <Command><Registernumber>
        if (uMsgLen >= 2) {
            answer_register_request(psBus, puMsg[1], uSender);
        }
        break;
    case eCMD_SET_REG_8BIT:
        // Format: <Command><Registernumber><Value>
        if (uMsgLen >= 3) {
            register_set_u8   (puMsg[1], puMsg[2]);
        }
        break;
    case eCMD_SET_REG_16BIT:
        // Format: <Command><Registernumber><LowValue><HighValue>
        if (uMsgLen >= 4) {
            temp16 = puMsg[2];
            temp16 |= ((uint16_t)puMsg[3]<<8);
            register_set_u16(puMsg[1], temp16);
        }
        break;
    case eCMD_SET_REG_32BIT:
        // Format: <Command><Registernumber><LowLowValue><LowValue><HighValue><HighHighValue>
        if (uMsgLen >= 6) {
            temp32 = puMsg[2];
            temp32 |= ((uint32_t)puMsg[3]<<8);
            temp32 |= ((uint32_t)puMsg[4]<<16);
            temp32 |= ((uint32_t)puMsg[5]<<24);
            register_set_u32(puMsg[1], temp32);
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
void        register_send_u8        (sBus_t*                psBus,
                                     uint16_t               uReceiver,
                                     uint8_t                uRegNo,
                                     uint8_t                uValue)
{
    uint8_t msg[3];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = uRegNo;
    msg[2] = uValue;
    bus_send_message(psBus, uReceiver, sizeof(msg), msg);
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
void        register_send_u16       (sBus_t*                psBus,
                                     uint16_t               uReceiver,
                                     uint8_t                uRegNo,
                                     uint16_t               uValue)
{
    uint8_t msg[4];

    msg[0] = eCMD_STATE_16BIT;
    msg[1] = uRegNo;
    msg[2] = uValue & 0x00FF;
    msg[3] = uValue >> 8;
    bus_send_message(psBus, uReceiver, sizeof(msg), msg);
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
void        register_send_u32       (sBus_t*                psBus,
                                     uint16_t               uReceiver,
                                     uint8_t                uRegNo,
                                     uint32_t               uValue)
{
    uint8_t msg[6];

    msg[0] = eCMD_STATE_32BIT;
    msg[1] = uRegNo;
    msg[2] = uValue & 0x000000FF;
    msg[3] = (uValue >> 8) & 0x000000FF;
    msg[4] = (uValue >> 16) & 0x000000FF;
    msg[5] = (uValue >> 24) & 0x000000FF;
    bus_send_message(psBus, uReceiver, sizeof(msg), msg);
}

/**
 * Map incoming stated values to local registers.
 *
 * @param[in] uRemoteModuleAddr Module address of incoming state message.
 * @param[in] uRemoteRegister   Module register of incoming state message.
 * @param[in] uValue            Value to set.
 *
 * @returns TRUE, if a register mapping for the received address and value
 *          was found.
 */
BOOL        register_do_mapping     (uint16_t               uRemoteModuleAddr,
                                     uint8_t                uRemoteRegister,
                                     uint32_t               uValue)
{
    uint16_t    addr;
    uint8_t     ii, reg, target_reg;
    BOOL        retval = FALSE;

    for (ii=0; ii<REG_MAX_MAPPINGS; ii++) {
        if (!register_get(APP_eReg_RemoteAddr00 + ii, 0, &addr)) continue;
        if (addr == uRemoteModuleAddr) {
            if (!register_get(APP_eReg_RemoteReg00 + ii, 0, &reg)) continue;
            if (reg == uRemoteRegister) {
                if (!register_get(APP_eReg_TargetReg00 + ii, 0, &target_reg)) continue;
                register_set(target_reg, uValue);
                retval = TRUE;
            }
        }
    }
    return retval;
}

/** @} */
