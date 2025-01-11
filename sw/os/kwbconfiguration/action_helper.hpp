/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    action_helper.hpp
 * @brief   Actions: Manage and run actions of a module.
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@web.de>
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
#pragma once

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <string>

// include
#include "prjtypes.h"
#include "moddef_common.h"

// os/libkwb
#include "connection.h"
#include "exceptions.h"
#include "log.h"

#include "msgbroker.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global methods ----------------------------------------------------------

template <typename ActionType, typename T, typename...  Args>
void runActionBlocking(Connection &connection, MsgBroker &msgBroker, uint16_t nodeId, T &result, Args... args)
{
    try {
        ActionType a(connection, msgBroker, nodeId, args...);
        a.start();
        a.waitFinished();
        result = a.getValue();
    }
    catch (Exception &e) {
        throw OperationFailed(LOC, "Action %s for node 0x%04x failed\n%s", typeid(ActionType).name(), nodeId, e.what());
    }
}

/** @} */
