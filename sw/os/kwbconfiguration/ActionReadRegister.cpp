/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionReadRegister.cpp
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

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

#include "ActionReadRegister.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

ActionReadRegister::ActionReadRegister(Connection   &msgep,
                                       MsgBroker    &broker,
                                       uint16_t     nodeId,
                                       uint8_t      registerId)
    : ActionWithResponse(msgep, broker, nodeId)
    , registerId(registerId)
{
}

void ActionReadRegister::setRegisterId(uint8_t registerId)
{
    this->registerId = registerId;
}

uint8_t ActionReadRegister::getRegisterId()
{
    return this->registerId;
}

bool ActionReadRegister::formMessage()
{
    if (nodeId == 0) {
        return false;
    }
    messageToSend.receiver = nodeId;
    messageToSend.sender = connection.getOwnNodeId();
    messageToSend.length = 2;
    messageToSend.data[0] = eCMD_REQUEST_REG;
    messageToSend.data[1] = registerId;
    return true;
}

bool ActionReadRegister::filterResponse(const msg_t &message)
{
    if (message.sender == nodeId &&
        message.length >= 3 &&
        (message.data[0] == eCMD_STATE_TYPELESS ||
         message.data[0] == eCMD_STATE_BITFIELDS ||
         message.data[0] == eCMD_STATE_8BIT ||
         message.data[0] == eCMD_STATE_16BIT ||
         message.data[0] == eCMD_STATE_32BIT ||
         message.data[0] == eCMD_STATE_DATE_TIME) &&
        message.data[1] == registerId) {
        return true;
    }
    return false;
}

void ActionReadRegister::handleResponse(const msg_t &message, void *reference)
{
    receivedMessage = message;
    messageReceived = true;
}

int ActionReadRegister::getValue()
{
    int value = 0;

    switch (receivedMessage.data[0]) {
    case eCMD_STATE_8BIT:
        value = receivedMessage.data[2];
        break;
    case eCMD_STATE_16BIT:
        value = receivedMessage.data[2];
        value <<= 8;
        value |= receivedMessage.data[3];
        break;
    case eCMD_STATE_32BIT:
        value = receivedMessage.data[2];
        value <<= 8;
        value |= receivedMessage.data[3];
        value <<= 8;
        value |= receivedMessage.data[4];
        value <<= 8;
        value |= receivedMessage.data[5];
        break;
    default:
        break;
    }
    return value;
}

/** @} */
