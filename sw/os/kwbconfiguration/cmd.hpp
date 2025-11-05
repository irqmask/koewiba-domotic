/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cmd.hpp
 * @brief   Base class for commands to be handled by #CommandHandler
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
#include <condition_variable>
#include <mutex>
#include <thread>

#include "exceptions.h"

class CommandBase
{
public:
    enum Status {
        Unknown = -2,
        Running = -1,
        FinshedOk = 0,
        FinshedWithError = 1,
    };

    /**
     * ctor
     */
    CommandBase()
        : timeout(std::chrono::seconds(5))
        , started(false)
        , shall_run(true)
        , error(false)
        , aborted(false)
        , timeout_occurred(false)
    {}

    /**
     * dtor
     */
    virtual ~CommandBase()
    {}

    uint32_t id()
    {
        return m_id;
    }

    /**
     * Start the action.
     * @throws OperationFailed if action cannot be started.
     */
    virtual void start()
    {
        std::lock_guard<std::mutex> lk(mutex);
        this->start_time = std::chrono::high_resolution_clock::now();
        this->timeout_occurred = false;
        this->started = true;
        this->shall_run = true;
        this->aborted = false;
        this->cond_started.notify_one();
    }

    /**
     * Abort running command
     */
    void abort()
    {
        std::lock_guard<std::mutex> lk(this->mutex);
        this->shall_run = false;
        this->aborted = true;
        this->cond_finished.notify_one();
    }

    /**
     * @returns true if command still shall run.
     * No error occurred, no timeout occurred and no abort occurred.
     */
    bool shallRun()
    {
        std::lock_guard<std::mutex> lk(this->mutex);
        return this->shall_run;
    }

    /**
     * @returns true if command was aborted
     */
    bool isAborted()
    {
        std::lock_guard<std::mutex> lk(this->mutex);
        return this->aborted;
    }

    /**
     * @returns true, if ctimeout occurred running this command, otherwise false.
     */
    bool isTimedOut()
    {
        checkTimeout();
        return timeout_occurred;
    }

    /**
     * @returns true, if command finished with error (including timeouts).
    bool finishedWithError()
    {
        return error;
    }

protected:
    /**
     * Internal function, which must be called, when a command is finished.
     */
    void setFinished()
    {
        std::lock_guard<std::mutex> lk(this->mutex);
        this->shall_run = false;
        this->cond_finished.notify_one();
    }

    /**
     * Internal function, which must be called, when a command is finished.
     */
    void setFinishedWithError()
    {
        std::lock_guard<std::mutex> lk(this->mutex);
        this->error = true;
        this->shall_run = false;
        this->cond_finished.notify_one();
    }

    /**
     * Wait until command was pulled from start queue and start() was executed.
     * @returns true if it was started, false on timeout.
     */
    bool waitStarted(std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lk(this->mutex);
        cond_started.wait_for(lk, timeout);

        if (started) {
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * Wait until the command is marked finished.
     * It can be finished due to completion, error, abort or timeout. Check with shallRun() and isTimedOut()
     */
    void waitFinished(std::chrono::milliseconds timeout)
    {
        std::unique_lock<std::mutex> lk(this->mutex);
        cond_finished.wait_for(lk, timeout);
    }

    /**
     * Measure time since action start and set Timeout flag once the timeout duration exceeds.
     */
    void checkTimeout()
    {
        std::lock_guard<std::mutex> lk(this->mutex);
        auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
        if (shall_run && (elapsed > timeout)) {
            timeout_occurred = true;
            if (started && shall_run) {
                this->error = true;
                this->shall_run = false;
                this->cond_finished.notify_one();
            }
        }
    }

    /**
     * Advance the action until it is finished.
     * If the action throws an Exception it is automatically finished, too.
     *
     * @throws OperationFailed when a action failed during execution
     * @throws Timeout whe the action timed out.
     */
    virtual void runABit() = 0;

protected:
    friend class CommandHandler;

    void setId(uint32_t id)
    {
        m_id = id;
    }

    //! Start timepoint
    std::chrono::system_clock::time_point start_time;
    //! Duration until action times out.
    std::chrono::duration<int>  timeout;
    //! Unique id of the command
    uint32_t m_id = 0;
    //! Flag if command wad started
    bool started;
    //! Flag if action is still running
    bool shall_run;
    //! Flag if action finished with error
    bool error;
    //! Flag if command was aborted
    bool aborted;
    //! Flag if timeout occurred.
    bool timeout_occurred;

private:
    std::condition_variable cond_started;
    std::condition_variable cond_finished;
    std::mutex mutex;
};
