/**
 * @addtogroup SYSSTRING
 * @brief Various sting functions.
 *
 * This module offers system independent functions for string handling.
 *
 * @{
 * @file    sysstring.h
 * @brief   Various sting functions.
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
#ifndef _SYSSTRING_H_
#define _SYSSTRING_H_

// --- Include section ---------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "system.h"

// --- Definitions -------------------------------------------------------------

//! Compare two strings case-insensitive
#if defined (PRJCONF_WINDOWS)
    #define sys_strcasecmp  _stricmp
#else
    #define sys_strcasecmp  strcasecmp
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#if defined (PRJCONF_WINDOWS) && (_MSC_VER) && _MSC_VER < 1900
    //int vsnprintf(char *outBuf, size_t size, const char *format, va_list ap);
    int snprintf(char *outBuf, size_t size, const char *format, ...);
#endif

#endif /* _SYSSTRING_H_ */
/** @} */
