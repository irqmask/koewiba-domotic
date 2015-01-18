/**
 * @addtogroup KWBFIRMWARE
 *
 * @{
 * @file    kwbfirmware_main.c
 * @brief   Update firmware of a module.
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
#include "firmwareupdate.h"
#include "message_serial.h"
#include "systime.h"

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
    int                 rc = eERR_NONE;
    bool                end_application = false;
    ioloop_t            mainloop;
    firmwareupdate_t    firmware;

    do {
        printf("\nkwbfirmware...\n");
        ioloop_init(&mainloop);
        ioloop_set_default_timeout(&mainloop, 1);
        
        firmware_update_init(&firmware, &mainloop, "/dev/ttyUSB0", 57600);
        firmware_update_start(&firmware, "test.hex", 5);

        printf("entering mainloop...\n");
        while (!end_application) {
            ioloop_run_once(&mainloop);
            rc = firmware_update_run(&firmware);
            if (rc != eRUNNING) {
                if (rc == eMSG_ERR_BUSY) {
                    sys_sleep_ms(100);
                } else {
                    // end or error
                    end_application = true;
                }
            }
        }
        firmware_update_close(&firmware);
    } while (0);
    return rc;
}

/** @} */
