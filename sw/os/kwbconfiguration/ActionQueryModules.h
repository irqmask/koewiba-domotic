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

/**
 * Action to query one or multiple modules which are currently online.
 */
class ActionQueryModules : public ActionWithResponse
{
public:
    //! Structure to store queried bus module information
    struct Module {
    public:
        //! Module address
        uint16_t nodeId;
        //! Module type and version information of hard and software.
        version_info_t version;
    };

    /**
     * Constructor
     * @param[in]   conn        Reference to established connection to a
     *                          KWB bus os router
     * @param[in]   broker      Reference to message broker.
     * @param[in]   moduleAddr  (optional, default = 0) Module address to communicate with.
     */
    ActionQueryModules(Connection &conn, MsgBroker &broker, uint16_t moduleAddr = 0);

    /**
     * Get the result of the query.
     * @return list of queried bus modules.
     */
    std::vector<Module> getModules();

    virtual bool waitForResponse() override;

protected:
    virtual bool formMessage() override;
    virtual bool filterResponse(const msg_t &message) override;
    virtual void handleResponse(const msg_t &message, void *reference) override;

    //! List of module information of modules which responed during this action.
    std::vector<Module> modules;
};

/** @} */
