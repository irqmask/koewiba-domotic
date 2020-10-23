/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionQueryModules.cpp
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

#include "ActionQueryModules.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

ActionQueryModules::ActionQueryModules(Connection   &conn,
                                       MsgBroker    &broker,
                                       uint16_t     nodeId)
    : ActionWithResponse(conn, broker, nodeId)
{
}

//----------------------------------------------------------------------------
bool ActionQueryModules::waitForResponse()
{
    bool one_message_received = false, rc;


    do {
        this->messageReceived = false;
        rc = ActionWithResponse::waitForResponse();
        if (rc) {
            one_message_received = true;
        }
    } while (rc);

    return one_message_received;
}

//----------------------------------------------------------------------------
std::vector<ActionQueryModules::Module> ActionQueryModules::getModules()
{
    return modules;
}

//----------------------------------------------------------------------------
bool ActionQueryModules::formMessage()
{
    messageToSend.receiver = moduleAddr;
    messageToSend.sender = connection.getOwnNodeId();
    messageToSend.length = 2;
    messageToSend.data[0] = eCMD_REQUEST_INFO_OF_TYPE;
    messageToSend.data[1] = eINFO_VERSION;
    modules.clear();
    return true;
}

//----------------------------------------------------------------------------
bool ActionQueryModules::filterResponse(const msg_t &message)
{
    if (message.length >= (MOD_VERSIONINFO_LEN + 1) &&
        message.data[0] == eCMD_STATE_VERSION) {
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------
void ActionQueryModules::handleResponse(const msg_t &message, void *reference)
{
    Module new_module;

    new_module.nodeId = message.sender;
    new_module.version.controller_id[0] = message.data[1];
    new_module.version.controller_id[1] = message.data[2];
    new_module.version.controller_id[2] = message.data[3];
    new_module.version.controller_id[3] = message.data[4];
    new_module.version.board_id = (message.data[5] << 8) | message.data[6];
    new_module.version.board_rev = message.data[7];
    new_module.version.app_id = (message.data[8] << 8) | message.data[9];
    new_module.version.version[0] = message.data[10];
    new_module.version.version[1] = message.data[11];
    new_module.version.version[2] = message.data[12];

    modules.push_back(new_module);

    messageReceived = true;
}

/** @} */
