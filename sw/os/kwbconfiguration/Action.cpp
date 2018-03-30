/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    Action.cpp
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
 
// --- Include section ---------------------------------------------------------

#include <functional>

#include "prjconf.h"

#include "Action.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------
using namespace std::chrono_literals;
Action::Action(MsgEndpoint &msgep, MsgBroker &broker) : msgBroker(broker), 
                                                        msgEndpoint(msgep),
                                                        timeout(5s),
                                                        timeout_elapsed(false)
{
}

bool Action::isFinished()
{
    return true;
}

bool Action::hasTimedOut()
{
    return timeout_elapsed;
}
/** @} */
