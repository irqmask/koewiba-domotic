/**
 * @addtogroup KWBTEST
 *
 * @{
 * @file    kwbtest_main.c
 * @brief   Test base to develop mainloop and libsystem routine
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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

#include <sstream>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <safe_lib.h>
#endif

// include
#include "prjtypes.h"

// os/include
#include "error_codes.h"
#include "kwb_defines.h"

// os/libsystem
#include "sysgetopt.h"
#include "sysserial.h"
#include "systime.h"

// os/shared
#include "connection_socket.h"
#include "exceptions.h"
#include "ioloop.h"
#include "log.h"
#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static bool g_end_application = false;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void handle_message(const msg_t &message, void *reference)
{
    (reference);
    msg_log("RECV", &message);
}

static void send_message(Connection &co, uint16_t sender, uint16_t receiver, uint32_t message_type)
{
    msg_t message;
    message.sender = sender;
    message.receiver = receiver;
    switch (message_type) {
    case 2:
        message.length = 2;
        message.data[0] = 2;
        message.data[1] = 1;
        break;
    default:
        message.length = 1;
        message.data[0] = 0x24; // request version
        break;
    }
    co.send(message);
}

static void on_close_connection(const std::string &uri, void *reference)
{
    (uri);
    (reference);
    g_end_application = true;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Main entry point of kwbtest playground.
 * @param[in]   argc    Number of arguments passed.
 * @param[in]   argv    List of program arguments.
 * @return 0, if successfully executed, otherwise a non-zero value.
 */
int main(int argc, char *argv[])
{
    (argc);
    (argv);
    int             rc = eERR_NONE;
    ioloop_t        mainloop;
    int state = 1;

    log_set_mask(0xFFFFFFFF);
    log_msg(LOG_INFO, "kwbtest...");
    ioloop_init(&mainloop);

    try {
        std::stringstream uriss;
        uriss << "/tmp/kwb.usk:0";
        ConnectionSocket conn(&mainloop, uriss.str());

        using std::placeholders::_1;
        using std::placeholders::_2;
        incom_func_t handle_incoming_message_func = std::bind(&handle_message, _1, _2);
        conn.setIncomingHandler(handle_incoming_message_func);

        conn_func_t handle_connection_func = std::bind(&on_close_connection, _1, _2);
        conn.setConnectionHandler(handle_connection_func);

        log_msg(LOG_STATUS, "entering mainloop...");
        while (!g_end_application) {
            ioloop_run_once(&mainloop);
            if (state == 1) {
                send_message(conn, 0x0501, 0x0110, 2);
                state = 0;
            }
        }
    }
    catch (Exception &e)
    {
        log_error(e.what());
        rc = -1;
    }
    return rc;
}

/** @} */
