/**
 * @addtogroup SYSTIME
 *
 * @{
 * @file    systime.c
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
 
// --- Include section ---------------------------------------------------------

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <sys/time.h>
#elif defined (PRJCONF_WINDOWS)
  #include <windows.h>
#endif

#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)

sys_time_t sys_time_get_usecs (void)
{
    sys_time_t usecs;
    struct timeval timestamp;

    gettimeofday(&timestamp, 0);

    usecs = timestamp.tv_sec;
    usecs *= 1000000;
    usecs += timestamp.tv_usec;
    return usecs;
}
#endif

#if defined (PRJCONF_WINDOWS)
sys_time_t sys_time_get_usecs (void)
{
    LARGE_INTEGER     usecs, freq;
    double            temp;

    usecs.QuadPart = 0;
    freq.QuadPart = 0;

    QueryPerformanceCounter(&usecs);

    if (QueryPerformanceFrequency(&freq) && freq.QuadPart > 0) {
        temp = (double)usecs.QuadPart * 1000000;
        temp /= (double)freq.QuadPart;
        usecs.QuadPart = (sys_time_t)temp;
    }

    return (sys_time_t)usecs.QuadPart;
}
#endif

/** @} */
