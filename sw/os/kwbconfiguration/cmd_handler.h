/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cmd_handler.h
 * @brief   Command Handler
 * Single or Multi-Threaded usable command dispatcher
 * @todo replace id with weak_ptr
 * @todo replace result enum in Cmd
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

#include <chrono>
#include <memory>
#include <mutex>
#include <list>
#include <vector>

#include "cmd.hpp"

class CommandHandler
{
public:
    /// ctor
    CommandHandler();
    /// dtor
    virtual ~CommandHandler();

    /// Clear command queue, abort all running
    void clear();

    // Command starters

    /// Start a command
    /// @tparam TCmd
    /// @returns Id of started command
    /// @throws InvalidParameter, if parameters or command was invalid
    /// @throws OperationFailed, if starting was not possible
    template <class TCmd, typename... Types>
    std::shared_ptr<TCmd> start(Types... params)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        auto cmd = std::make_shared<TCmd>(params...);

        cmd->setId(nextId++);
        startQueue.push_back(cmd);
        return cmd;
    }

    /// Check if command is finished.
    /// @param[in]  id      Command id
    /// @returns ture, if command is finished, otherwise false.
    bool isFinished(uint32_t id);

    /// Wait until command is finished. Will abort command on timeout
    /// @param[in]  id      Command id
    /// @returns Run-status of command.
    CommandBase::Status waitFinished(uint32_t id, std::chrono::milliseconds timeout);

    /// @param[in]  id      Id of command to abort.
    /// Does nothing if command does not exist. Blocks until command is aborted.
    void abort(uint32_t id);

    // Command executor methods

    /// @returns the number of running commands
    uint32_t numRunningCommands();

    /// @returns the number of not started and running commands
    uint32_t numPendingCommands();

    /// Check if a command is in the queue and start it
    void fetchAndStart();

protected:
    /// Find a command by its ID, which is waiting to be started
    /// @param[in]  id  ID of the command to find
    /// @returns Command if found otherwise nullptr
    std::shared_ptr<CommandBase> findWaitingCommand(uint32_t id);

    /// Find a running command by its ID
    /// @param[in]  id  ID of the command to find
    /// @returns Command if found otherwise nullptr
    std::shared_ptr<CommandBase> findRunningCommand(uint32_t id);

    uint32_t maxRunning;
    uint32_t nextId;
    std::recursive_mutex mutex;
    std::list<std::shared_ptr<CommandBase>> startQueue;
    std::vector<std::shared_ptr<CommandBase>> runQueue;
};
