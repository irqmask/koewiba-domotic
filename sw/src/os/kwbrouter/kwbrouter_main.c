/**
 * @addtogroup KWBROUTER
 *
 * @{
 * @file    kwbrouter_main.c
 * @brief   Router for commands received from bus or different nets.
 * kwbrouter routs commands comming from a source to one or more destinations.
 * Source and destination could be kwbrouters on a different IP or serial RS485
 * connections or local UNIX sockets for IPC.
 * Other programs eg a webserver or kwbupload a firmware uploader can connect
 * to kwbrouter to receive or send messages.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "bus.h"
#include "ioloop.h"
#include "message_socket.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;
    bool            end_application = false;
    ioloop_t        mainloop;
    msg_socket_t    msg_socket;
    msg_endpoint_t* ep;
    msg_t msg;

    do {
        printf("\nkwbrouter...\n");
        ioloop_init (&mainloop);

        msg_s_init(&msg_socket);
        if ((rc = msg_s_open_server(&msg_socket, &mainloop, "/tmp/kwb.usk", 0)) != eERR_NONE) {
            break;
        }

        msg.receiver = 15;
        msg.sender = 1;
        msg.length = 8;
        msg.data[0] = 'H';
        msg.data[1] = 'a';
        msg.data[2] = 'l';
        msg.data[3] = 'l';
        msg.data[4] = 'o';
        msg.data[5] = 'M';
        msg.data[6] = 's';
        msg.data[7] = 'g';

        printf("entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);

            ep = msg_s_get_endpoint(&msg_socket, 0, 1<<eMSG_EP_COMM);
            if (ep != NULL) {
                msg_s_send(&msg_socket, ep, &msg);
                msg.sender++;
            }
        }
    } while (0);
    return rc;
}

/** @} */
