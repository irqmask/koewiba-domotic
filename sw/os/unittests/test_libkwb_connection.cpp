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

#include "gtest/gtest.h"

#include "prjconf.h"

// include
#include "prjtypes.h"
// libkwb
#include "connection.h"
#include "log.h"
#include "message.h"

/// Unittests for the connection class
class ConnectionTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        log_set_mask(0xFFFFFFFF);
        log_add_prefix("[          ] ");
        incomingCallbackCalled = 0;
        closeCallbackCalled = 0;
    }

    // void TearDown() override {}

public:
    void incomingCallback(const msg_t &message, void *reference);
    void closeCallback(const std::string &uri, void *reference);

    int incomingCallbackCalled;
    int closeCallbackCalled;
};

void ConnectionTest::incomingCallback(const msg_t &message, void *reference)
{
    incomingCallbackCalled++;
}

void ConnectionTest::closeCallback(const std::string &uri, void *reference)
{
    closeCallbackCalled++;
}

TEST_F(ConnectionTest, instantiate)
{
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(nullptr, std::string("test1"));
    ASSERT_STREQ("test1", conn->getName().c_str());
    conn.reset();
}

TEST_F(ConnectionTest, callbacks)
{
    std::shared_ptr<Connection> conn = std::make_shared<Connection>(nullptr, std::string("test2"));
    msg_t message = {0}; // content not important right now
    conn->onIncomingMessage(message);
    ASSERT_EQ(0, incomingCallbackCalled);

    using std::placeholders::_1;
    using std::placeholders::_2;
    incom_func_t incomingCallbackFunc = std::bind(&ConnectionTest::incomingCallback, this, _1, _2);
    conn->setIncomingHandler(incomingCallbackFunc);
    conn->onIncomingMessage(message);
    ASSERT_EQ(1, incomingCallbackCalled);

    conn->clearIncomingHandler();
    conn->onIncomingMessage(message);
    ASSERT_EQ(1, incomingCallbackCalled);

    conn->onConnectionClosed();
    ASSERT_EQ(0, closeCallbackCalled);

    conn_func_t closeCallbackFunc = std::bind(&ConnectionTest::closeCallback, this, _1, _2);
    conn->setConnectionHandler(closeCallbackFunc);
    conn->onConnectionClosed();
    ASSERT_EQ(1, closeCallbackCalled);

    conn->clearConnectionHandler();
    conn->onConnectionClosed();
    ASSERT_EQ(1, closeCallbackCalled);
    conn.reset();
}
/** @} */
