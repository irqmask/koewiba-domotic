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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <safe_lib.h>

#include "error_codes.h"
#include "ioloop.h"
#include "message_socket.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void log_hexdump16 (uint8_t* data, uint16_t length)
{
    uint16_t offset = 0;

    assert (data != NULL);

    while (offset < length) {
        if ((offset % 16) == 0) {
            if (offset != 0) printf("\n");
            printf("%04X: ", offset);
        }
        printf("%02X ", data[offset]);
        offset++;
    }
    printf("\n");
}

static void handle_message(msg_t* message, void* reference, void* arg)
{
    printf("Message received: sender %d, receiver %d, len %d\n",
           message->sender, message->receiver, message->length);
    log_hexdump16(message->data, message->length);
    if (message->data[0] == 1 && message->data[1] == 1) {
        if (message->data[2] == 1) {
            system("./relay_on.sh");
        } else {
            system("./relay_off.sh");
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;
    bool            end_application = false;
    ioloop_t        mainloop;
    msg_socket_t    msg_socket;

    do {
        printf("\nkwbtest...\n");
        ioloop_init(&mainloop);

        msg_s_init(&msg_socket);
        msg_s_set_incomming_handler(&msg_socket, handle_message, NULL);
        if ((rc = msg_s_open_client(&msg_socket, &mainloop, "/tmp/kwb.usk", 0)) != eERR_NONE) {
            break;
        }
        printf("entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);
        }
    } while (0);
    return rc;
}

/** @} */
