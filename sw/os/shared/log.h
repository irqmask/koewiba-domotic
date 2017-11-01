/**
 * @addtogroup LOG
 * @brief Log output module.
 *
 * Contains functions to log data to vairous channes. (Currently stdout and stderror).
 *
 * @{
 * @file    log.h
 * @brief   Log output module.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _LOG_H_
#define _LOG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

#define LOG_ERROR       0x00000001
#define LOG_WARNING     0x00000002
#define LOG_INFO        0x00000004
#define LOG_STATUS      0x00000008
#define LOG_USERDEFINED 0x00000010
#define LOG_VERBOSE1    0x00000020
#define LOG_VERBOSE2    0x00000040
#define LOG_VERBOSE3    0x00000080
#define LOG_VERBOSE4    0x00000100

#define LOG_MAX_MESSAGE_LENGTH 256

// --- Type definitions --------------------------------------------------------

typedef uint32_t log_mask_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void log_set_mask (log_mask_t logmask);

log_mask_t log_get_mask (void);

void log_add_mask (log_mask_t logmask);

void log_msg (log_mask_t logmask, const char* logmessage, ...);

void log_warning(const char* logmessage, ...);

void log_error (const char* logmessage, ...);

void log_sys_error (const char* logmessage, ...);

#ifdef __cplusplus
}
#endif

#endif /* _LOG_H_ */
/** @} */
