/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    ActionWriteRegister.h
 * @brief   Action: Write a register of a bus module. 
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
#pragma once 

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

// include
#include "prjtypes.h"
#include "cmddef_common.h"

#include "ActionRequest.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class ActionWriteRegister : public ActionRequest {
public:
    ActionWriteRegister(Connection & conn, MsgBroker & broker, uint16_t nodeId = 0, uint8_t registerId = 0);
    
    void setRegisterId(uint8_t registerId);
    uint8_t getRegisterId();
    
    void setValue(int value);
    int getValue();
    
    void setRegisterFormat(cmd_common_t format);
    cmd_common_t getRegisterFormat();
    
protected:
    uint8_t         registerId;
    cmd_common_t    registerFormat;
    int             value;
    
    virtual bool formMessage();
};

/** @} */
