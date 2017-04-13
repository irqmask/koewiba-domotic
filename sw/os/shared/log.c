/**
 * @addtogroup LOG
 *
 * @{
 * @file    log.c
 * @brief   Log output module.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

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
 * Log a message if there are matches between the message logmask and the
 * global active logmask.
 */
void log_msg (log_mask_t logmask, const char* logmessage, ...)
{
    va_list args;

    if ((logmask & active_logs) != 0) {
        va_start(args, logmessage);
        if (logmask & LOG_ERROR) {
            vfprintf(stderr, logmessage, args);
            fprintf(stderr, "\n");
        } else {
            vfprintf(stdout, logmessage, args);
            fprintf(stdout, "\n");
        }
        va_end(args);
    }
}

void log_warning(const char* logmessage, ...)
{
    va_list args;

    if ((LOG_WARNING & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stdout, "WARNING   ");
        vfprintf(stdout, logmessage, args);
        fprintf(stdout, "\n");
        va_end(args);
    }
}

void log_error (const char* logmessage, ...)
{
    va_list args;

    if ((LOG_ERROR & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stderr, "ERROR     ");
        vfprintf(stderr, logmessage, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

void log_sys_error (const char* logmessage, ...)
{
    va_list args;

    if ((LOG_ERROR & active_logs) != 0) {
        va_start(args, logmessage);
        fprintf(stderr, "ERROR     ");
        vfprintf(stderr, logmessage, args);
        fprintf(stderr, " %s", strerror(errno));
        fprintf(stderr, "\n");
        va_end(args);
    }
}

/** @} */
