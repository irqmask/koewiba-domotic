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

#define LOG_STANDARD_MASK (LOG_ERROR | LOG_WARNING | LOG_INFO)

#define LOG_MAX_MESSAGE_LENGTH 256

// --- Type definitions --------------------------------------------------------

typedef uint32_t log_mask_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Set global log-mask.
 * @param[in]   logmask     Log-mask to set.
 */
void log_set_mask(log_mask_t logmask);

/**
 * Get global log-mask.
 * @retval Log-mask
 */
log_mask_t log_get_mask(void);

/**
 * Add logmask to global log-mask.
 * @param[in]   logmask     Log-mask to add.
 */
void log_add_mask(log_mask_t logmask);

/**
 * Add a prefix to every log message.
 * @param[in]   pref        Prefix o be added. It needs to be a static and constant
 *                          string. It will not be copied!
 */
void log_add_prefix(const char *pref);

/**
 * Log a message if there are matches between the message logmask and the
 * global active logmask.
 */
void log_msg(log_mask_t logmask, const char *logmessage, ...);

void log_info(const char *logmessage, ...);

void log_warning(const char *logmessage, ...);

void log_error(const char *logmessage, ...);

void log_sys_error(const char *logmessage, ...);

#ifdef __cplusplus
}
#endif

#endif /* _LOG_H_ */
/** @} */
