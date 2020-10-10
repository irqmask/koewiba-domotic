/**
 * @addtogroup LOG
 *
 * @{
 * @file    log.c
 * @brief   Log output module.
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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
  #include <unistd.h>
#endif

#include "log.h"
#include "sysconsole.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

#define LOG_MAX_BUFFER_SIZE (LOG_MAX_MESSAGE_LENGTH + 100)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static log_mask_t active_logs = (LOG_ERROR | LOG_WARNING | LOG_STATUS);

static const char* prefix = "";

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Set global log-mask.
 * @param[in] logmask Log-mask to set.
 */
void log_set_mask (log_mask_t logmask)
{
    active_logs = logmask;
}

/**
 * Get global log-mask.
 * @retval Log-mask
 */
log_mask_t log_get_mask (void)
{
    return active_logs;
}

/**
 * Add logmask to global log-mask.
 * @param[in] logmask Log-mask to add.
 */
void log_add_mask (log_mask_t logmask)
{
    active_logs |= logmask;
}

/**
 * Add a prefix to every log message.
 * @param[in]   pref    Prefix o be added. It needs to be a static and constant
 *                      string. It will not be copied!
 */
void log_add_prefix (const char* pref)
{
    prefix = pref;
}

/**
 * Log a message if there are matches between the message logmask and the
 * global active logmask.
 */
void log_msg (log_mask_t logmask, const char* logmessage, ...)
{
    va_list args;

    if ((logmask & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stderr, "%s", prefix);
        vfprintf(stderr, logmessage, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

void log_info(const char* logmessage, ...)
{
    va_list args;

    if ((LOG_INFO & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stderr, "%s", prefix);
        fprintf(stderr, "INFO      ");
        vfprintf(stderr, logmessage, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

void log_warning(const char* logmessage, ...)
{
    va_list args;

    if ((LOG_WARNING & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stderr, "%s", prefix);
        fprintf(stderr, "WARNING   ");
        vfprintf(stderr, logmessage, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

void log_error (const char* logmessage, ...)
{
    va_list args;

    if ((LOG_ERROR & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stderr, "%s", prefix);
        fprintf(stderr, "ERROR     ");
        vfprintf(stderr, logmessage, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

void log_sys_error (const char* logmessage, ...)
{
    int err = errno;

    va_list args;

    if ((LOG_ERROR & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stderr, "%s", prefix);
        fprintf(stderr, "ERROR     ");
        vfprintf(stderr, logmessage, args);
        fprintf(stderr, " %s", strerror(err));
        fprintf(stderr, "\n");
        va_end(args);
    }
}

/** @} */
