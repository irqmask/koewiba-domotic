/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_libkwb_connection_serial.cpp
 * @brief   This module contains the unittests for: libkwb/connection_serial.
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
#include <thread>

#include <poll.h>

#include "gtest/gtest.h"

#include "prjconf.h"

// include
#include "prjtypes.h"

// libkwb
#include "connection_serial.h"
#include "exceptions.h"
#include "log.h"
#include "message.h"

// libsystem
#include "syssocket.h"

/// Unittests for the serial connection class
class ConnectionSerialTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        log_set_mask(0xFFFFFFFF);
        log_add_prefix("[          ] ");
        incomingCallbackCalled = 0;
        closeCallbackCalled = 0;
        ioloop_init(&iol);
    }

    void TearDown() override
    {
        ioloop_close(&iol);
    }

public:
    void incomingCallback(const msg_t &message, void *reference);
    void closeCallback(const std::string &uri, void *reference);

    /// Counter how often the incoming callback has been called
    int                 incomingCallbackCalled;
    /// Counter how often the close callback has been called
    int                 closeCallbackCalled;

    /// Last received message
    static msg_t        incomingMessage;
    /// Catched sent message
    static char         outgoingMessage[256];

protected:
    /// Name of pipe device used for testing
    static constexpr char PIPE_DEVICE_NAME[] = "/tmp/kwb_test_pipe";

    /// IOLoop object where all connections are registered
    ioloop_t            iol;

    /// Thread acting as relay comunication partner
    std::shared_ptr<std::thread> echoThread;

    /// Flag to keep recho thread running
    bool                echoThreadRunning;

    /// Start the echo thread.
    /// @param[in] server True, starts thread as server otherwise as client.
    void startEchoThread();

    /// Stop the echo thread.
    void stopEchoThread();

    /// Send arbitrary data to the pipe
    /// @param[in]  data    Data to be sent. Needs to be 0 terminated!
    void simpleSend(const char *data);

    /// Run ioloop for given time
    /// Timeout may vary due to ioloops own timeout
    /// @param[in] duration Time to run ioloop
    void runIOLoopFor(std::chrono::milliseconds duration);

    /// Echo thread
    static void pipeRecvThread(ConnectionSerialTest *reference);
};

msg_t ConnectionSerialTest::incomingMessage;
char ConnectionSerialTest::outgoingMessage[256];
constexpr char ConnectionSerialTest::PIPE_DEVICE_NAME[];

void ConnectionSerialTest::incomingCallback(const msg_t &message, void *reference)
{
    this->incomingCallbackCalled++;
    memcpy(&this->incomingMessage, &message, sizeof(msg_t));
}

void ConnectionSerialTest::closeCallback(const std::string &uri, void *reference)
{
    this->closeCallbackCalled++;
}

void ConnectionSerialTest::startEchoThread()
{
    char command[256] = {0};
    int rc;

    snprintf(command, sizeof(command), "rm %s", PIPE_DEVICE_NAME);
    command[sizeof(command) - 1] = '\0';
    rc = system(command);
    if (rc != 0) {
        log_sys_error("Unable to execute command %s!", command);
    }

    snprintf(command, sizeof(command), "mkfifo %s", PIPE_DEVICE_NAME);
    command[sizeof(command) - 1] = '\0';
    rc = system(command);
    if (rc != 0) {
        log_sys_error("Unable to execute command %s!", command);
    }

    echoThreadRunning = true;
    echoThread = std::make_shared<std::thread>(pipeRecvThread, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void ConnectionSerialTest::stopEchoThread()
{
    echoThreadRunning = false;
    if (echoThread != nullptr) {
        echoThread->join();
    }
}

void ConnectionSerialTest::simpleSend(const char *data)
{
    int fd = sys_serial_open(PIPE_DEVICE_NAME);
    if (fd != INVALID_FD) {
        sys_serial_send(fd, data, strlen(data));
        sys_serial_close(fd);
    }
}

void ConnectionSerialTest::runIOLoopFor(std::chrono::milliseconds duration)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto current = start;
    do {
        ioloop_run_once(&iol);
        current = std::chrono::high_resolution_clock::now();
    } while (std::chrono::duration_cast<std::chrono::seconds>(current - start) < duration);
}

void ConnectionSerialTest::pipeRecvThread(ConnectionSerialTest *reference)
{
    char        c;
    uint8_t     idx = 0;
    bool        complete = false;

    struct pollfd pfd = {INVALID_FD, POLLIN, 0};

    pfd.fd = sys_serial_open(PIPE_DEVICE_NAME);

    while (reference->echoThreadRunning && !complete) {
        if (poll(&pfd, 1, 100) > 0) {
            while (sys_serial_recv(pfd.fd, &c, 1) == 1) {
                outgoingMessage[idx++] = c;
                if (c == '\n') {
                    outgoingMessage[idx++] = '\0';
                    complete = true;
                    idx = 0;
                    break;
                }
            }
        }
    }

    if (pfd.fd != INVALID_FD) {
        sys_serial_close(pfd.fd);
    }
}

TEST_F(ConnectionSerialTest, fail_to_connect)
{
    std::shared_ptr<ConnectionSerial> conn;
    ASSERT_THROW(conn = std::make_shared<ConnectionSerial>(&iol, "/unknown/device/", false), ConnectionFailed);
    ASSERT_EQ(nullptr, conn);
}

TEST_F(ConnectionSerialTest, connect_and_close)
{
    std::shared_ptr<ConnectionSerial> conn;
    startEchoThread();
    ASSERT_NO_THROW(conn = std::make_shared<ConnectionSerial>(&iol, PIPE_DEVICE_NAME, false));
    ASSERT_NE(nullptr, conn);

    using std::placeholders::_1;
    using std::placeholders::_2;
    conn_func_t closeCallbackFunc = std::bind(&ConnectionSerialTest::closeCallback, this, _1, _2);
    conn->setConnectionHandler(closeCallbackFunc);

    ASSERT_STREQ(PIPE_DEVICE_NAME, conn->getName().c_str());
    runIOLoopFor(std::chrono::milliseconds(100));
    conn.reset();
    runIOLoopFor(std::chrono::milliseconds(500));
    stopEchoThread();
    ASSERT_EQ(0, closeCallbackCalled);
    ASSERT_EQ(0, incomingCallbackCalled);
}
/*
TEST_F(ConnectionSerialTest, remote_close)
{
    std::shared_ptr<ConnectionSerial> conn;
    startEchoThread();
    ASSERT_NO_THROW(conn = std::make_shared<ConnectionSerial>(&iol, PIPE_DEVICE_NAME, false));
    ASSERT_NE(nullptr, conn);

    conn_func_t closeCallbackFunc = std::bind(&ConnectionSerialTest::closeCallback, this, _1, _2);
    conn->setConnectionHandler(closeCallbackFunc);

    runIOLoopFor(std::chrono::milliseconds(100));
    stopEchoThread();
    runIOLoopFor(std::chrono::milliseconds(500));
    ASSERT_EQ(1, closeCallbackCalled);
    ASSERT_EQ(0, incomingCallbackCalled);
    conn.reset();
}
*/

TEST_F(ConnectionSerialTest, send)
{
    std::shared_ptr<ConnectionSerial> conn;
    startEchoThread();
    ASSERT_NO_THROW(conn = std::make_shared<ConnectionSerial>(nullptr, PIPE_DEVICE_NAME, false));
    ASSERT_NE(nullptr, conn);

    using std::placeholders::_1;
    using std::placeholders::_2;
    incom_func_t incomingCallbackFunc = std::bind(&ConnectionSerialTest::incomingCallback, this, _1, _2);
    conn->setIncomingHandler(incomingCallbackFunc);

    msg_t message;
    message.sender = 0x0001;
    message.receiver = 0x0002;
    message.length = 8;
    message.data[0] = 0x40;
    message.data[1] = 0x41;
    message.data[2] = 0x42;
    message.data[3] = 0x43;
    message.data[4] = 0x44;
    message.data[5] = 0x45;
    message.data[6] = 0x46;
    message.data[7] = 0x47;

    conn->send(message);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_STREQ("00010002084041424344454647\n", outgoingMessage);

    conn.reset();
    stopEchoThread();
}

TEST_F(ConnectionSerialTest, receive)
{
    std::shared_ptr<ConnectionSerial> conn;
    ASSERT_NO_THROW(conn = std::make_shared<ConnectionSerial>(&iol, PIPE_DEVICE_NAME, false));
    ASSERT_NE(nullptr, conn);

    using std::placeholders::_1;
    using std::placeholders::_2;
    incom_func_t incomingCallbackFunc = std::bind(&ConnectionSerialTest::incomingCallback, this, _1, _2);
    conn->setIncomingHandler(incomingCallbackFunc);

    simpleSend("0002000108BFBEBDBCBBBAB9B8\n");
    runIOLoopFor(std::chrono::milliseconds(100));

    ASSERT_EQ(1, incomingCallbackCalled);
    ASSERT_EQ(0x0002, incomingMessage.sender);
    ASSERT_EQ(0x0001, incomingMessage.receiver);
    ASSERT_EQ(8, incomingMessage.length);
    ASSERT_EQ(0xBF, incomingMessage.data[0]);
    ASSERT_EQ(0xBE, incomingMessage.data[1]);
    ASSERT_EQ(0xBD, incomingMessage.data[2]);
    ASSERT_EQ(0xBC, incomingMessage.data[3]);
    ASSERT_EQ(0xBB, incomingMessage.data[4]);
    ASSERT_EQ(0xBA, incomingMessage.data[5]);
    ASSERT_EQ(0xB9, incomingMessage.data[6]);
    ASSERT_EQ(0xB8, incomingMessage.data[7]);
    conn.reset();
}
/** @} */
