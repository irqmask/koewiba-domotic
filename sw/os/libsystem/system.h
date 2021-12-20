/**
 * @addtogroup SYSTEM
 * @brief Library to cover operating system dependent functions.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    system.h
 * @brief   Library to cover operating system dependent functions.
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
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <safe_lib.h>
#elif defined (PRJCONF_WINDOWS)
    #include <windows.h>
#endif

#include "error_codes.h"
#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

//! Invalid handle value
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #define INVALID_FD (-1)
#elif defined (PRJCONF_WINDOWS)
    #define INVALID_FD INVALID_HANDLE_VALUE
#endif

// --- Type definitions --------------------------------------------------------

//! operating-system specific error codes
typedef enum {
    eSYS_ERR_NONE = eERR_NONE,

    eSYS_ERR_SER_UNSUPPORTED = 200,
    eSYS_ERR_SER_CONFIGURE,
} sys_errors_t;

//! Operating system independent handle value.
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    typedef int sys_fd_t;
#elif defined (PRJCONF_WINDOWS)
    typedef HANDLE sys_fd_t;
#endif

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _SYSTEM_H_ */
/** @} */
