/**
 * @addtogroup KWBTEST
 *
 * @{
 * @file    kwbtest_main.c
 * @brief   Test base to develop mainloop and libsystem routine
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

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
#include "ioloop.h"
#include "log.h"
#include "message.h"
#include "message_socket.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

bool   g_end_application = false;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void handle_message(msg_t* message, void* reference, void* arg)
{
    msg_log("RECV", *message);
}

static void on_close_connection(const char* address, uint16_t port, void* reference, void* arg)
{
    g_end_application = true;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;
    ioloop_t        mainloop;
    msg_socket_t    msg_socket;
    msg_endpoint_t* msg_ep;

    do {
        log_msg(KWB_LOG_INFO, "kwbtest...");
        log_set_mask(0xFFFFFFFF);
        ioloop_init(&mainloop);

        msg_s_init(&msg_socket);
        msg_s_set_incomming_handler(&msg_socket, handle_message, NULL);
        if ((rc = msg_s_open_client(&msg_socket, &mainloop, "/tmp/kwbr.usk", 0)) != eERR_NONE) {
            break;
        }
        msg_ep = msg_s_get_endpoint(&msg_socket, 0, 0);
        msg_s_set_closeconnection_handler(msg_ep, on_close_connection, NULL);
        log_msg(KWB_LOG_STATUS, "entering mainloop...");
        while (!g_end_application) {
            ioloop_run_once(&mainloop);
        }
        //msg_s_close_connection(&msg_socket);
    } while (0);
    return rc;
}

/** @} */
