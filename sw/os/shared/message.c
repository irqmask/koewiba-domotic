/**
 * @addtogroup MESSAGE
 *
 * @{
 * @file    message.c
 * @brief   Common message handling function.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "log.h"
#include "message.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_log (msg_t message)
{
    BOOL first_line = TRUE;
    uint8_t remaining_length, ii, bytes_in_line;
    char logline[256], tmp[256];

    remaining_length = message.length;
    ii = 0;
    bytes_in_line = 0;

    while (remaining_length) {
        if (bytes_in_line == 0) {
            if (first_line == TRUE) {
                first_line = FALSE;
                snprintf(logline, sizeof(logline), "%04X %04X %2d ", message.sender, message.receiver, message.length);
            } else {
                snprintf(logline, sizeof(logline), "             ");
            }
        }

        if (remaining_length > 1 && bytes_in_line < 16) {
            snprintf(tmp, sizeof(tmp), "%02X ", message.data[ii]);
        } else {
            snprintf(tmp, sizeof(tmp), "%02X", message.data[ii]);
        }
        strcat_s(logline, sizeof(logline), tmp);
        bytes_in_line++;
        remaining_length--;
        ii++;
        if (bytes_in_line == 16 || remaining_length == 0) {
            bytes_in_line = 0;
            log_msg(LOG_VERBOSE1, "%s", logline);
            logline[0] = '\0';
        }
    }
}

/** @} */
