/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionWriteRegister.cpp
 * @brief   Action: Query a register of a bus module and wait for the answer. 
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@web.de>
 *
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

#include "prjconf.h"

// include
#include "prjtypes.h"
#include "cmddef_common.h"

#include "ActionWriteRegister.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

ActionWriteRegister::ActionWriteRegister(MsgEndpoint& msgep, 
                                         MsgBroker&   broker, 
                                         uint16_t     nodeId,
                                         uint8_t      registerId) : ActionRequest(msgep, broker, nodeId),
                                                                    registerId(registerId),
                                                                    value(0)
{
}

void ActionWriteRegister::setRegisterId(uint8_t registerId)
{
    this->registerId = registerId;
}

uint8_t ActionWriteRegister::getRegisterId()
{
    return this->registerId;
}
    
bool ActionWriteRegister::formMessage()
{
    if (nodeId == 0) return false;
    messageToSend.receiver = nodeId;
    messageToSend.sender = msgEndpoint.getOwnNodeId();
    messageToSend.data[0] = registerFormat;
    messageToSend.data[1] = registerId;

    switch (registerFormat) {
    case eCMD_SET_REG_8BIT:
        messageToSend.length = 3;
        messageToSend.data[2] = (uint8_t)(value & 0x000000FF);
        break;
    case eCMD_SET_REG_16BIT:
        messageToSend.length = 4;
        messageToSend.data[2] = (uint8_t)((value & 0x0000FF00) >> 8);
        messageToSend.data[3] = (uint8_t)(value & 0x000000FF);
        break;
    case eCMD_SET_REG_32BIT:
        messageToSend.length = 6;
        messageToSend.data[2] = (uint8_t)((value & 0xFF000000) >> 24);
        messageToSend.data[3] = (uint8_t)((value & 0x00FF0000) >> 16);
        messageToSend.data[4] = (uint8_t)((value & 0x0000FF00) >> 8);
        messageToSend.data[5] = (uint8_t)(value & 0x000000FF);
        break;
    default:
        return false;
    }
    return true;
}

void ActionWriteRegister::setValue(int value)
{
    this->value = value;
}

int ActionWriteRegister::getValue()
{
    return value;
}

void ActionWriteRegister::setRegisterFormat(cmd_common_t format)
{
    registerFormat = format;
}

cmd_common_t ActionWriteRegister::getRegisterFormat()
{
    return registerFormat;
}

/** @} */
