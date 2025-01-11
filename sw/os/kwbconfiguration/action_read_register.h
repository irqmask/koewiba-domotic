/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    action_read_register.h
 * @brief   Action: Query a register of a bus module and wait for the answer.
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

// include
#include "prjtypes.h"

#include "action_with_response.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Action to read a regiter value from a bus module.
 */
class ActionReadRegister : public ActionWithResponse
{
public:
    /**
     * Constructor
     * @param[in]   conn        Reference to established connection to a
     *                          KWB bus os router
     * @param[in]   broker      Reference to message broker.
     * @param[in]   moduleAddr  (optional, default = 0) Module address to communicate with.
     * @param[in]   registerId  Id of register to query.
     */
    ActionReadRegister(Connection &conn, MsgBroker &broker, uint16_t moduleAddr = 0, uint8_t registerId = 0);

    /**
     * Set register id to query.
     * @param[in]   registerId  register id to query.
     */
    void setRegisterId(uint8_t registerId);

    /**
     * Get reqister id to query
     * @return register id
     */
    uint8_t getRegisterId();

    /**
     * Return the result of this query.
     * @return Queried register value.
     */
    int getValue();

protected:
    virtual bool formMessage() override;
    virtual bool filterResponse(const msg_t &message) override;
    virtual void handleResponse(const msg_t &message, void *reference) override;

    //! Id of register to query.
    uint8_t registerId;
};

/** @} */
