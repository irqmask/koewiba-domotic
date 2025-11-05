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
 */
#include "cmd_handler.h"

#include <condition_variable>
#include <mutex>

// libkwb
#include "exceptions.h"
#include "log.h"

// kwbconfiguration
#include "cmd.hpp"

//----------------------------------------------------------------------------
CommandHandler::CommandHandler()
    : maxRunning(1)
    , nextId(0)
{
}

//----------------------------------------------------------------------------
CommandHandler::~CommandHandler()
{
    clear();
}

//----------------------------------------------------------------------------
void CommandHandler::clear()
{
    std::lock_guard<std::recursive_mutex> lock(mutex);

    while (startQueue.size()) {
        startQueue.pop_back();
    }
}

//----------------------------------------------------------------------------
bool CommandHandler::isFinished(uint32_t id)
{
    std::lock_guard<std::recursive_mutex> lock(mutex);

    // search command
    auto cmd = findRunningCommand(id);
    if (cmd == nullptr) {
        // returns false for not found commands even when those have been started and finished.
        return false;
    }

    // command is finished, if it has been started and finished
    // returns also false for commands waiting in the start queue
    return (cmd->shall_run == false);
}

//----------------------------------------------------------------------------
CommandBase::Status CommandHandler::waitFinished(uint32_t id, std::chrono::milliseconds timeout)
{
    std::shared_ptr<CommandBase> cmd;

    {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        cmd = findWaitingCommand(id);
        if (cmd == nullptr) {
            cmd = findRunningCommand(id);
        }
    }

    if (cmd == nullptr) {
        return CommandBase::Status::Unknown;
    }

    cmd->waitFinished(timeout);
    if (cmd->isTimedOut()) {
        throw Timeout(LOC, "Command not finished within timeout!");
    }
    if (cmd->isAborted()) {
        throw OperationFailed(LOC, "Command aborted!");
    }

    return CommandBase::Status::FinshedOk;
}

//----------------------------------------------------------------------------
void CommandHandler::abort(uint32_t id)
{
    auto cmd = findRunningCommand(id);

    if (cmd != nullptr) {
        cmd->abort();
    }
}

//----------------------------------------------------------------------------
uint32_t CommandHandler::numRunningCommands()
{
    std::lock_guard<std::recursive_mutex> lock(mutex);

    return runQueue.size();
}

//----------------------------------------------------------------------------
uint32_t CommandHandler::numPendingCommands()
{
    std::lock_guard<std::recursive_mutex> lock(mutex);

    return startQueue.size() + runQueue.size();
}

//----------------------------------------------------------------------------
void CommandHandler::fetchAndStart()
{
    if (runQueue.size() < maxRunning) {
        std::lock_guard<std::recursive_mutex> lock(mutex);
        auto cmd = startQueue.front();
        if (cmd != nullptr) {
            try {
                startQueue.pop_back();
                // check if command still shall run, e.g. timeout happened in the meantime
                if (cmd->shallRun()) {
                    cmd->start();
                    runQueue.emplace_back(cmd);
                }
            }
            catch (Exception &e) {
                log_error("Unable to start action: %s", e.what());
                cmd->setFinishedWithError();
            }
        }
    }

    for (std::vector<std::shared_ptr<CommandBase>>::iterator it = runQueue.begin(); it != runQueue.end();) {
        auto cmd = *it;
        cmd->runABit();

        if (cmd->shallRun() == false || cmd->isTimedOut()) {
            it = runQueue.erase(it);
        }
        else {
            ++it;
        }
    }
}

//----------------------------------------------------------------------------
std::shared_ptr<CommandBase> CommandHandler::findWaitingCommand(uint32_t id)
{
    std::shared_ptr<CommandBase> cmd;
    for (auto &c : startQueue) {
        if (c->id() == id) {
            cmd = c;
            break;
        }
    }
    return cmd;
}

//----------------------------------------------------------------------------
std::shared_ptr<CommandBase> CommandHandler::findRunningCommand(uint32_t id)
{
    std::shared_ptr<CommandBase> cmd;
    for (auto &c : runQueue) {
        if (c->id() == id) {
            cmd = c;
            break;
        }
    }
    return cmd;
}

/** @} */
