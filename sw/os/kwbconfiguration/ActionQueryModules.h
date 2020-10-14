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

#include "ActionWithResponse.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class ActionQueryModules : public ActionWithResponse
{
public:
    struct Module {
    public:
        uint16_t nodeId;
        version_info_t version;
    };

    ActionQueryModules(Connection &conn, MsgBroker &broker, uint16_t nodeId = 0);

    std::vector<Module> getModules();
    virtual bool waitForResponse() override;

protected:
    virtual bool formMessage();
    virtual bool filterResponse(const msg_t &message) override;
    virtual void handleResponse(const msg_t &message, void *reference) override;

    std::vector<Module> modules;
};

/** @} */
