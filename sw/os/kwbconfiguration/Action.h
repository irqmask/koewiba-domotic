/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    Action.h
 * @brief   Base-class of an action to be performed with a bus-module. 
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018  christian <irqmask@gmx.de>
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

#include <chrono>

// include
#include "prjtypes.h"
#include "MsgBroker.h"
#include "MsgEndpoint.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class Action {
public:
    Action(MsgEndpoint &msgep, MsgBroker &broker);

    virtual bool start() = 0;
    virtual void cancel() = 0;
    virtual bool isFinished();
    virtual bool hasTimedOut();
    
protected:
    MsgBroker                   &msgBroker;
    MsgEndpoint                 &msgEndpoint;
    std::chrono::duration<int>  timeout;
    bool                        timeout_elapsed;
};

/** @} */
