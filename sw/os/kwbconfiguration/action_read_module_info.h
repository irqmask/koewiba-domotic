/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionQueryModules.h
 * @brief   Action: Query version information of a bus module and wait for the answer.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2019  christian <irqmask@web.de>
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

#include <vector>

#include "prjconf.h"

// include
#include "prjtypes.h"
#include "moddef_common.h"

#include "action_with_response.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Action to query one or multiple modules which are currently online.
 */
class ActionReadModuleInfo : public Action
{
public:
    /**
     * Constructor
     * @param[in]   conn        Reference to established connection to a
     *                          KWB bus os router
     * @param[in]   broker      Reference to message broker.
     * @param[in]   nodeId      Module address aka node id to communicate with.
     */
    ActionReadModuleInfo(Connection &conn, MsgBroker &broker, uint16_t nodeId = 0);

    virtual void start() override;
    virtual void waitFinished() override;
    virtual void cancel() override;

    std::array<uint8_t , 4> getControllerId() { return controllerId; };
    uint16_t getBoardId() { return boardId; };
    uint8_t getBoardRev() { return boardRev; };
    uint16_t getAppId() { return appId; };
    uint8_t getMajorVersion() { return majorVersion; };
    uint8_t getMinorVersion() { return minorVersion; };
    uint8_t getBugfixVersion() { return bugfixVersion; };
    uint32_t getVersionHash() { return versionHash; };

protected:
    uint16_t nodeId;

    std::array<uint8_t, 4> controllerId;
    uint16_t boardId;
    uint8_t boardRev;
    uint16_t appId;
    uint8_t majorVersion;
    uint8_t minorVersion;
    uint8_t bugfixVersion;
    uint32_t versionHash;
};

/** @} */
