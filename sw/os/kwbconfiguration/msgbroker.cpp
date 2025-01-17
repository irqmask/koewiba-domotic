/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    msgbroker.cpp
 * @brief   Broker which sorts incomming messages to running actions.
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

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

// include
#include "prjtypes.h"

#include "msgbroker.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class implementation  ---------------------------------------------------

MsgBroker::MsgBroker()
{
    this->response_handlers.clear();
}

//----------------------------------------------------------------------------
void MsgBroker::registerForResponse(void *reference, msg_filter_t &filter_func, incom_func_t &handler_func)
{
    msg_filter_data_t filter = { reference, filter_func, handler_func };
    this->response_handlers.push_back(filter);
}

//----------------------------------------------------------------------------
void MsgBroker::unregisterForResponse(void *reference)
{
    std::vector<msg_filter_data_t> &l = this->response_handlers;
    std::vector<msg_filter_data_t>::iterator it = l.begin();
    while (it != l.end()) {
        if (it->reference == reference) {
            it = l.erase(it);
            if (it == l.end()) {
                return;
            }
        }
        else {
            it++;
        }
    }
}

//----------------------------------------------------------------------------
void MsgBroker::handleIncomingMessage(const msg_t &message, void *reference)
{
    (reference);
    for (auto receiver_data : this->response_handlers) {
        if (receiver_data.msg_filter(message)) {
            receiver_data.msg_handler(message, reference);
            unregisterForResponse(receiver_data.reference);
        }
    }
}

/** @} */
