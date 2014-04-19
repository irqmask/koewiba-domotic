/**
 * @addtogroup REGISTER
 * @brief Common module for register handling.
 *
 * @{
 * @file    register.h
 * @brief   Common module for register handling.
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

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

extern uint32_t REG__uGetRegister       (uint8_t                uRegNo,
                                            eRegType_t*         peRegType);

extern void     REG__vSetRegister       (uint8_t                uRegNo,
                                         uint32_t               uValue);

/**
 * This function answers a register request command.
 *
 * @param[in] uRegNo
 * Register number.
 * @param[in] uSender
 * Sender of the request message.
 */
static void vCmdRequestRegister(sBus_t*                         psBus,
                                uint8_t                         uRegNo,
                                uint16_t                        uSender)
{
    uint32_t    value;
    eRegType_t  regtype = eRegType_Unkown;

    value = REG__uGetRegister(uRegNo, &regtype);
    switch (regtype) {
    case eRegType_U8:
        REG_vSendStateU8(psBus, uSender, uRegNo, (uint8_t)(value & 0x000000FF));
        break;
    case eRegType_U16:
        REG_vSendStateU16(psBus, uSender, uRegNo, (uint16_t)(value & 0x0000FFFF));
        break;
    case eRegType_U32:
        REG_vSendStateU32(psBus, uSender, uRegNo, value);
        break;
    default:
        break;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

uint8_t         REG_uGetU8Register      (uint8_t                uRegNo)
{
    return (uint8_t)(REG__uGetRegister(uRegNo, NULL) & 0x000000FF);
}

uint16_t        REG_uGetU16Register     (uint8_t                uRegNo)
{
    return (uint16_t)(REG__uGetRegister(uRegNo, NULL) & 0x0000FFFF);
}

uint32_t        REG_uGetU32Register     (uint8_t                uRegNo)
{
    return REG__uGetRegister (uRegNo, NULL);
}

void            REG_vSetU8Register      (uint8_t                uRegNo,
                                            uint8_t                uValue)
{
    REG__vSetRegister(uRegNo, uValue);
}

void            REG_vSetU16Register   (uint8_t                  uRegNo,
                                         uint16_t               uValue)

{
    REG__vSetRegister(uRegNo, uValue);
}

void            REG_vSetU32Register   (uint8_t                uRegNo,
                                         uint32_t               uValue)
{
    REG__vSetRegister (uRegNo, uValue);
}

/**
 * Command interpreter for register commands.
 */
void            REG_vDoCommand          (sBus_t*                psBus,
                                         uint8_t*               puMsg,
                                         uint8_t                uMsgLen,
                                         uint16_t               uSender)
{
    uint32_t temp32;
    uint16_t temp16;

    // check command
    switch (puMsg[0]) {
    case CMD_eRequestRegister:
        // Format: <Command><Registernumber>
        if (uMsgLen >= 2) {
            vCmdRequestRegister(psBus, puMsg[1], uSender);
        }
        break;
    case CMD_eSetRegister8bit:
        // Format: <Command><Registernumber><Value>
        if (uMsgLen >= 3) {
            REG_vSetU8Register(puMsg[1], puMsg[2]);
        }
        break;
    case CMD_eSetRegister16bit:
        // Format: <Command><Registernumber><LowValue><HighValue>
        if (uMsgLen >= 4) {
            temp16 = puMsg[2];
            temp16 |= ((uint16_t)puMsg[3]<<8);
            REG_vSetU16Register(puMsg[1], temp16);
        }
        break;
    case CMD_eSetRegister32bit:
        // Format: <Command><Registernumber><LowLowValue><LowValue><HighValue><HighHighValue>
        if (uMsgLen >= 6) {
            temp32 = puMsg[2];
            temp32 |= ((uint32_t)puMsg[3]<<8);
            temp32 |= ((uint32_t)puMsg[3]<<16);
            temp32 |= ((uint32_t)puMsg[3]<<24);
            REG_vSetU32Register(puMsg[1], temp32);
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
void            REG_vSendStateU8        (sBus_t*                psBus,
                                         uint16_t               uReceiver,
                                         uint8_t                uRegNo,
                                         uint8_t                uValue)
{
    uint8_t msg[3];

    msg[0] = CMD_eState8bit;
    msg[1] = uRegNo;
    msg[2] = uValue;
    bus_send_message(psBus, 0x0a, sizeof(msg), msg);
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
void            REG_vSendStateU16       (sBus_t*                psBus,
                                         uint16_t               uReceiver,
                                         uint8_t                uRegNo,
                                         uint16_t               uValue)
{
    uint8_t msg[4];

    msg[0] = CMD_eState16bit;
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
void            REG_vSendStateU32       (sBus_t*                psBus,
                                         uint16_t               uReceiver,
                                         uint8_t                uRegNo,
                                         uint32_t               uValue)
{
    uint8_t msg[6];

    msg[0] = CMD_eState32bit;
    msg[1] = uRegNo;
    msg[2] = uValue & 0x000000FF;
    msg[3] = (uValue >> 8) & 0x000000FF;
    msg[4] = (uValue >> 16) & 0x000000FF;
    msg[5] = (uValue >> 24) & 0x000000FF;
    bus_send_message(psBus, uReceiver, sizeof(msg), msg);
}

/** @} */
