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

#include "action_read_module_info.h"

#include <memory>

// include
#include "prjconf.h"
#include "prjtypes.h"
#include "moddef_common.h"

// os/libkwb
#include "exceptions.h"

#include "action_helper.hpp"
#include "action_read_register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

ActionReadModuleInfo::ActionReadModuleInfo(Connection   &conn,
                                           MsgBroker    &broker,
                                           uint16_t     nodeId)
    : Action(conn, broker)
    , nodeId(nodeId)
    , controllerId({0, 0, 0, 0})
    , boardId(0)
    , boardRev(0)
    , appId(0)
    , majorVersion(0)
    , minorVersion(0)
    , bugfixVersion(0)
    , versionHash(0)
{
}

void ActionReadModuleInfo::start()
{
    Action::start();
}

void ActionReadModuleInfo::waitFinished()
{
    try {
        checkTimeout();
        runActionBlocking<ActionReadRegister, uint16_t>(this->connection, this->msgBroker, nodeId, boardId, MOD_eReg_BoardID);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, boardRev, MOD_eReg_BoardRev);
        runActionBlocking<ActionReadRegister, uint16_t>(this->connection, this->msgBroker, nodeId, appId, MOD_eReg_AppID);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, majorVersion, MOD_eReg_AppVersionMajor);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, minorVersion, MOD_eReg_AppVersionMinor);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, bugfixVersion, MOD_eReg_AppVersionBugfix);
        runActionBlocking<ActionReadRegister, uint32_t>(this->connection, this->msgBroker, nodeId, versionHash, MOD_eReg_AppVersionHash);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, controllerId[0], MOD_eReg_DeviceSignature0);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, controllerId[1], MOD_eReg_DeviceSignature1);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, controllerId[2], MOD_eReg_DeviceSignature2);
        runActionBlocking<ActionReadRegister, uint8_t>(this->connection, this->msgBroker, nodeId, controllerId[3], MOD_eReg_DeviceSignature3);
    }
    catch (Exception & e) {
        throw OperationFailed(LOC, "Error reading module information of nodeId 0x%04x\n%s", nodeId, e.what());
    }
}

void ActionReadModuleInfo::cancel()
{
}

/** @} */
