/**
 * @addtogroup SYSERROR
 *
 * @{
 * @file    syserror.c
 * @brief   Get error information of errors thrown by the operating system.
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

#include <stdio.h>
#include "syserror.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#if defined (PRJCONF_WINDOWS)
void sys_err_get_as_string(char *buffer, size_t buffersize)
{
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer,
                  buffersize - 1, NULL);
    buffer[buffersize - 1] = '\0';
}
#else
void sys_err_get_as_string(char *buffer, size_t buffersize)
{
    snprintf(buffer, buffersize - 1, "sys_err_get_as_string() not implemented!");
    buffer[buffersize - 1] = '\0';
}
#endif

void sys_err_print_last(void)
{
    char buffer[256];
    sys_err_get_as_string(buffer, sizeof(buffer));
    fprintf(stderr, "SYSTEM ERROR: %s\n", buffer);
}
/** @} */
