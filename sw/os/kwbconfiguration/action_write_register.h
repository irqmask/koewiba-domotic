/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    action_write_register.h
 * @brief   Action: Write a register of a bus module.
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
#include "cmddef_common.h"

#include "action_request.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

/**
 * Action to write a regiter value to a bus module.
 */
class ActionWriteRegister : public ActionRequest
{
public:
    /**
     * Constructor
     * @param[in]   conn        Reference to established connection to a
     *                          KWB bus os router
     * @param[in]   moduleAddr  (optional, default = 0) Module address to communicate with.
     * @param[in]   registerId  Id of register to write to.
     */
    ActionWriteRegister(std::shared_ptr<Connection> conn, uint16_t moduleAddr = 0, uint8_t registerId = 0, int32_t val = 0);

    /**
     * Set the id of the register to write to.
     * @param[in]   registerId  Register id to write to.
     */
    void setRegisterId(uint8_t registerId);

    /**
     * Get id of the reqister to write to.
     * @return register id
     */
    uint8_t getRegisterId();

    /**
     * Set the value which shall be written to the register
     * @param[in]   value       Value to be written.
     */
    void setValue(int value);

    /**
     * Get the value which shall be written to the register
     * @return Value to be written.
     */
    int getValue();

    /**
     * Set the register format to select between 8, 16 and 32bit values.
     * @param[in]   format      Register format being one of cmd_common_t.
     */
    void setRegisterFormat(cmd_common_t format);

    /**
     * Get the register format to write 8, 16 or 32bit values.
     * @return register format beeing one of cmd_common_t.
     */
    cmd_common_t getRegisterFormat();

protected:
    virtual bool formMessage() override;
    virtual void runABit() override;

    //! Id of register to write to.
    uint8_t         registerId;
    //! Format of the register to write.
    cmd_common_t    registerFormat;
    //! Value to write into the register.
    int32_t         value;
};

/** @} */
