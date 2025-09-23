/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_libkwb_connection_socket.cpp
 * @brief   This module contains the unittests for: libkwb/connection_socket.
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

// libkwb
#include "connection_socket.h"
#include "log.h"
#include "message.h"

// libsystem
#include "syssocket.h"

/// Unittests for the socket connection class
class ConnectionSocketTest : public ::testing::Test
{
protected:
    /// SetUp is called before each test is started
    void SetUp() override
    {
        log_set_mask(0xFFFFFFFF);
        log_add_prefix("[          ] ");
        incomingCallbackCalled = 0;
        closeCallbackCalled = 0;
        ioloop_init(&iol);
    }

    /// TearDown is called after each test is finished
    void TearDown() override
    {
        ioloop_close(&iol);
    }

public:
    /// Handler for incoming message events
    /// @param[in]  message     incoming message
    /// @param[in]  reference   (unused here)
    void incomingCallback(const msg_t &message, void *reference);

    /// Handler for close connection events
    /// Is called when the connection gets closed remotely
    /// @param[in]  uri         (unused here)
    /// @param[in]  reference   (unused here)
    void closeCallback(const std::string &uri, void *reference);

    /// Counter how often the incoming callback has been called
    int incomingCallbackCalled;
    /// Counter how often the close callback has been called
    int closeCallbackCalled;

protected:
    /// IOLoop object where all connections are registered
    ioloop_t iol;

    /// Thread acting as relay comunication partner
    std::shared_ptr<std::thread> echoThread;
    /// Flag to keep recho thread running
    bool echoThreadRunning;

    /// Start the echo thread.
    /// @param[in] server True, starts thread as server otherwise as client.
    void startEchoThread(bool server);

    /// Stop the echo thread.
    void stopEchoThread();

    /// Run ioloop for given time
    /// Timeout may vary due to ioloops own timeout
    /// @param[in] duration Time to run ioloop
    void runIOLoopFor(std::chrono::milliseconds duration);

    /// Echo thread as a server
    /// @param[in]  reference   Reference to the object of this test class.
    static void serverThread(ConnectionSocketTest *reference);

    /// Echo thread as a client
    /// @param[in]  reference   Reference to the object of this test class.
    static void clientThread(ConnectionSocketTest *reference);
};

//----------------------------------------------------------------------------
void ConnectionSocketTest::incomingCallback(const msg_t &message, void *reference)
{
    (void)message;
    (void)reference;
    ConnectionSocketTest::incomingCallbackCalled++;
}

//----------------------------------------------------------------------------
void ConnectionSocketTest::closeCallback(const std::string &uri, void *reference)
{
    (void)uri;
    (void)reference;
    ConnectionSocketTest::closeCallbackCalled++;
}

//----------------------------------------------------------------------------
void ConnectionSocketTest::startEchoThread(bool server)
{
    echoThreadRunning = true;
    if (server) {
        echoThread = std::make_shared<std::thread>(serverThread, this);
    }
    else {
        echoThread = std::make_shared<std::thread>(clientThread, this);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

//----------------------------------------------------------------------------
void ConnectionSocketTest::stopEchoThread()
{
    echoThreadRunning = false;
    if (echoThread != nullptr) {
        echoThread->join();
    }
}

//----------------------------------------------------------------------------
void ConnectionSocketTest::runIOLoopFor(std::chrono::milliseconds duration)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto current = start;
    do {
        ioloop_run_once(&iol);
        current = std::chrono::high_resolution_clock::now();
    } while (std::chrono::duration_cast<std::chrono::seconds>(current - start) < duration);
}

//----------------------------------------------------------------------------
void ConnectionSocketTest::serverThread(ConnectionSocketTest *reference)
{
    char address[256] = {'\0'};
    uint16_t port = 0;
    sys_fd_t server_fd = sys_socket_open_server_tcp(12345);
    sys_fd_t conn_fd = INVALID_FD;
    struct pollfd pfd = {server_fd, POLLIN, 0};

    while (reference->echoThreadRunning && conn_fd == INVALID_FD) {
        if (poll(&pfd, 1, 100) > 0) {
            conn_fd = sys_socket_accept(server_fd, address, sizeof(address), &port);
        }
    }

    msg_t message;
    pfd = {conn_fd, POLLIN, 0};
    while (reference->echoThreadRunning) {

        if (poll(&pfd, 1, 100) > 0) {
            if (sys_socket_recv(pfd.fd, &message, sizeof(message)) == sizeof(message)) {
                uint16_t temp;
                temp = message.receiver;
                message.receiver = message.sender;
                message.sender = temp;
                ssize_t written = sys_socket_send(pfd.fd, &message, sizeof(message));
                if (written != sizeof(message))
                {
                    log_error("message not (completely) written %zd", written);
                }
            }
        }
    }

    if (conn_fd != INVALID_FD) {
        sys_socket_close(conn_fd);
    }

    if (server_fd != INVALID_FD) {
        sys_socket_close(server_fd);
    }
}

//----------------------------------------------------------------------------
void ConnectionSocketTest::clientThread(ConnectionSocketTest *reference)
{
    (void)reference;
}

/// @test Test splitting an URI string into IPv4 address and port number.
TEST_F(ConnectionSocketTest, parseURI)
{
    std::string address = "";
    uint16_t port = 0;

    ConnectionSocket::getAddressAndPort("1.1.1.1:1", address, port);
    ASSERT_STREQ("1.1.1.1", address.c_str());
    ASSERT_EQ(1, port);

    ConnectionSocket::getAddressAndPort("0.0.0.0:65535", address, port);
    ASSERT_STREQ("0.0.0.0", address.c_str());
    ASSERT_EQ(65535, port);

    ConnectionSocket::getAddressAndPort("127.0.0.1:1000", address, port);
    ASSERT_STREQ("127.0.0.1", address.c_str());
    ASSERT_EQ(1000, port);

    ConnectionSocket::getAddressAndPort("192.168.178.100:2000", address, port);
    ASSERT_STREQ("192.168.178.100", address.c_str());
    ASSERT_EQ(2000, port);
}

/// @test Test opening and closing a connection
TEST_F(ConnectionSocketTest, connect_and_close)
{
    std::shared_ptr<ConnectionSocket> conn;
    startEchoThread(true);
    ASSERT_NO_THROW(conn = std::make_shared<ConnectionSocket>(&iol, "127.0.0.1:12345"));
    ASSERT_NE(nullptr, conn);

    using std::placeholders::_1;
    using std::placeholders::_2;
    conn_func_t closeCallbackFunc = std::bind(&ConnectionSocketTest::closeCallback, this, _1, _2);
    conn->setConnectionHandler(closeCallbackFunc);

    ASSERT_STREQ("127.0.0.1:12345", conn->getName().c_str());
    runIOLoopFor(std::chrono::milliseconds(100));
    conn.reset();
    runIOLoopFor(std::chrono::milliseconds(500));
    stopEchoThread();
    ASSERT_EQ(0, closeCallbackCalled);
    ASSERT_EQ(0, incomingCallbackCalled);
}

/// @test Test if socket connecion object recognizes a remotely closed connection.
TEST_F(ConnectionSocketTest, remote_close)
{
    std::shared_ptr<ConnectionSocket> conn;
    startEchoThread(true);
    ASSERT_NO_THROW(conn = std::make_shared<ConnectionSocket>(&iol, "127.0.0.1:12345"));
    ASSERT_NE(nullptr, conn);

    using std::placeholders::_1;
    using std::placeholders::_2;
    conn_func_t closeCallbackFunc = std::bind(&ConnectionSocketTest::closeCallback, this, _1, _2);
    conn->setConnectionHandler(closeCallbackFunc);

    runIOLoopFor(std::chrono::milliseconds(100));
    stopEchoThread();
    runIOLoopFor(std::chrono::milliseconds(100));
    ASSERT_EQ(1, closeCallbackCalled);
    ASSERT_EQ(0, incomingCallbackCalled);
    conn.reset();
}

/// @test Test sending and receiving over a socket connection
TEST_F(ConnectionSocketTest, send_and_receive)
{
    std::shared_ptr<ConnectionSocket> conn;
    startEchoThread(true);
    ASSERT_NO_THROW(conn = std::make_shared<ConnectionSocket>(&iol, "127.0.0.1:12345"));
    ASSERT_NE(nullptr, conn);

    using std::placeholders::_1;
    using std::placeholders::_2;
    incom_func_t incomingCallbackFunc = std::bind(&ConnectionSocketTest::incomingCallback, this, _1, _2);
    conn->setIncomingHandler(incomingCallbackFunc);

    msg_t message;
    message.sender = 0x0001;
    message.receiver = 0x0002;
    message.length = 4;
    message.data[0] = 42;
    message.data[1] = 43;
    message.data[2] = 44;
    message.data[3] = 45;

    conn->send(message);

    runIOLoopFor(std::chrono::milliseconds(100));

    conn.reset();
    runIOLoopFor(std::chrono::milliseconds(100));
    stopEchoThread();

    ASSERT_EQ(1, incomingCallbackCalled);
}

/** @} */
