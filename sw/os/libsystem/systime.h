/**
 * @addtogroup SYSTIME
 * @brief Socket interface.
 *
 * System independent interface to system time functions.
 *
 * @{
 * @file    systime.h
 * @brief   System independent interface to system time functions.
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
#ifndef _SYSTIME_H_
#define _SYSTIME_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <stdint.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <sys/time.h>
#elif defined (PRJCONF_WINDOWS)
    #include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

/**
 * Sleep a given amount of milliseconds
 *
 * @param[in]   x   Number of milliseconds to sleep.
 */
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
#define sys_sleep_ms(x) do{usleep((x)*1000);}while(0)
#elif defined (PRJCONF_WINDOWS)
#define sys_sleep_ms(x) do{Sleep(x);}while(0)
#endif

// --- Type definitions --------------------------------------------------------

//! Type for timestamps in microseconds
typedef uint64_t sys_time_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * @return time in microseconds
 */
sys_time_t sys_time_get_usecs(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTIME_H_ */
/** @} */
