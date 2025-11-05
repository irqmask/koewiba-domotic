/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_libkwb_connection.cpp
 * @brief   This module contains the unittests for: libkwb/connection.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2020  christian <irqmask@gmx.de>
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

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

// libkwb
#include "log.h"
// kwbconfiguration
#include "../kwbconfiguration/cmd_handler.h"
#include "../kwbconfiguration/cmd_handler.cpp"

static int g_cmd1_called = 0;
static int g_cmd2_called = 0;
static int g_cmd3_called = 0;

/// Unittests for the connection class
class kwbconfiguration_CommandHandlerTest : public ::testing::Test
{
protected:
    /// SetUp is called before each test is started
    void SetUp() override
    {
        log_set_mask(0xFFFFFFFF);
        log_add_prefix("[          ] ");
        g_cmd1_called = 0;
        g_cmd2_called = 0;
        g_cmd3_called = 0;
    }

    void TearDown() override {}

public:
};

using Clock = std::chrono::system_clock;
using Ms = std::chrono::milliseconds;

class TestCmd1 : public CommandBase
{
public:
    TestCmd1()
        : CommandBase()
    {}

    virtual ~TestCmd1()
    {}

    virtual void processResponse()
    {
        // nothing to do
    }

    virtual void runABit()
    {
        auto nowInMS = std::chrono::time_point_cast<Ms>(Clock::now());
        if ((nowInMS - start_time) < delay_time)
        {
            if (shall_run) std::this_thread::yield();
        }
        else
        {
            setFinished();
        }
    }

private:
    std::chrono::milliseconds delay_time = Ms(200);
};


/// @test Test if command can be started and aborted in a single thread
TEST_F(kwbconfiguration_CommandHandlerTest, cmdSingleTreadAbort)
{
    auto cmd = std::make_shared<TestCmd1>();

    /// start and abort
    EXPECT_NO_THROW(cmd->start());
    EXPECT_TRUE(cmd->shallRun());
    EXPECT_NO_THROW(cmd->abort());
    EXPECT_FALSE(cmd->shallRun());
}


/// @test Test if command can be started and finished
TEST_F(kwbconfiguration_CommandHandlerTest, cmdSingleTreadRun)
{
    auto cmd = std::make_shared<TestCmd1>();

    /// start and abort
    EXPECT_NO_THROW(cmd->start());
    EXPECT_TRUE(cmd->shallRun());

    bool shallRun;
    do {
        cmd->runABit();
        shallRun = cmd->shallRun();
    } while (shallRun);

    EXPECT_FALSE(cmd->shallRun());
}


/// @test Test if command can be started and aborted from a second thread
TEST_F(kwbconfiguration_CommandHandlerTest, cmdDualTreadAbort)
{
    auto cmd = std::make_shared<TestCmd1>();

    auto workerFunc = [&]() {
        // wait for start
        while (!cmd->shallRun()) {
            std::this_thread::yield();
        }

        // started
        while (cmd->shallRun()) {
            cmd->runABit();
        }
    };

    std::thread worker(workerFunc);

    cmd->start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    cmd->abort();

    EXPECT_FALSE(cmd->shallRun());
    EXPECT_FALSE(cmd->isTimedOut());
    EXPECT_TRUE(cmd->isAborted());
    worker.join();
}


/// @test Test if command can be started from a second thread and it waits for the command to be finished
TEST_F(kwbconfiguration_CommandHandlerTest, cmdDualTreadRun)
{
    auto cmd = std::make_shared<TestCmd1>();

    auto workerFunc = [&]() {
        cmd->start();
        while (cmd->shallRun()) {
            cmd->runABit();
        }
    };

    std::thread worker(workerFunc);
    EXPECT_TRUE(cmd->shallRun());
    worker.join();
    EXPECT_FALSE(cmd->shallRun());
    EXPECT_FALSE(cmd->isTimedOut());
}


/// @test Test if command handler object can be instantiated.
TEST_F(kwbconfiguration_CommandHandlerTest, instantiate)
{
    auto ch = std::make_shared<CommandHandler>();
}


/// @test Add commands to the command handler
TEST_F(kwbconfiguration_CommandHandlerTest, addSingleThreadMode)
{
    auto ch = std::make_shared<CommandHandler>();

    EXPECT_NO_THROW(ch->start<TestCmd1>());
    EXPECT_NO_THROW(ch->fetchAndStart());
    auto running = ch->numRunningCommands();
    EXPECT_EQ(running, 1);
}


/// @test Add commands to the command handler
TEST_F(kwbconfiguration_CommandHandlerTest, addDualThreadMode)
{
    auto ch = std::make_shared<CommandHandler>();
    CommandBase::Status threadSawFinishedCommand = CommandBase::Status::Unknown;

    // a second thread starts commands
    auto workerFunc = [&]() {
        auto cmd = ch->start<TestCmd1>();
        threadSawFinishedCommand = ch->waitFinished(cmd->id(), std::chrono::milliseconds(300));
        EXPECT_FALSE(cmd->isTimedOut());
        EXPECT_FALSE(cmd->isAborted());
    };
    std::thread worker(workerFunc);

    // wait until command is started
    int retries = 10;
    while (ch->numPendingCommands() == 0 && retries > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        retries--;
    }
    EXPECT_GT(retries, 0) << "Command not started withing 500ms";

    // the main handles the commands
    while (ch->numPendingCommands() > 0) {
        EXPECT_NO_THROW(ch->fetchAndStart());
        std::this_thread::yield();
    }

    worker.join();
    EXPECT_EQ(threadSawFinishedCommand, CommandBase::Status::FinshedOk);
}

/** @} */
