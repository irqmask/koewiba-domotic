/**
 * @addtogroup MESSAGE
 *
 * @{
 * @file    message.c
 * @brief   Common message handling function.
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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

#include "kwb_defines.h"
#include "log.h"
#include "message.h"
#include "prjtypes.h"
#include "sysconsole.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_log (const char* keyword, msg_t message)
{
    bool first_line = true;
    uint8_t remaining_length, ii, bytes_in_line;
    char logline[256], tmp[256];

    remaining_length = message.length;
    ii = 0;
    bytes_in_line = 0;

    while (remaining_length) {
        if (bytes_in_line == 0) {
            if (first_line == true) {
                first_line = false;
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
            log_msg(KWB_LOG_INTERCOMM, "%s %s", keyword, logline);
            logline[0] = '\0';
        }
    }
}

/** @} */
