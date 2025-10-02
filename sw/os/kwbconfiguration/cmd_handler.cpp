/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cmd_handler.cpp
 * @brief   Implamentation of Single or Multi-Threaded usable command dispatcher
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
 */#include "cmd_handler.h"

#include "cmd.hpp"

CommandHandler::CommandHandler()
{

}

CommandHandler::~CommandHandler() {
    clear();
}

void CommandHandler::clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    while (m_cmdQueue.size()) {
        m_cmdQueue.pop();
    }
}

void CommandHandler::add(std::shared_ptr<CommandBase> cmd)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cmdQueue.push(cmd);
}

/** @} */
