/**
 * @addtogroup PRJCONF
 * @brief Define project environment configuration.
 *
 * Is this a windows, linux, AVR microcontroller etc environment?
 * @{
 * @file    prjconf.h
 * @brief   Define project environment configuration.
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

#ifndef _PRJCONF_H__
#define _PRJCONF_H__

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#if defined (_WIN32) || defined (WIN32) // note the underscore: without it, it's not msdn official!
    // Windows (x64 and x86)
    #define PRJCONF_WINDOWS 1
#elif __unix__ // all unixes
    // Unix
    #define PRJCONF_UNIX    1
#elif __posix__
    // POSIX
    #define PRJCONF_POSIX   1
#elif __linux__
    // linux
    #define PRJCONF_LINUX   1
#elif __APPLE__
    // Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
    #define PRJCONF_APPLE   1
#elif defined (__AVR_ATmega8__)     || \
      defined (__AVR_ATmega88__)    || \
      defined (__AVR_ATmega88A__)   || \
      defined (__AVR_ATmega88P__)   || \
      defined (__AVR_ATmega328__)   || \
      defined (__AVR_ATmega328P__)  || \
      defined (__AVR_ATmega328PB__) || \
      defined (__AVR_ATmega324P__)  || \
      defined (__AVR_ATmega324PA__) || \
      defined (__AVR_ATmega324A__)  || \
      defined (__AVR_ATtiny1634__)
    #define PRJCONF_UC_AVR  1
#else
    #define PRJCONF_OTHER
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _PRJCONF_H__ */
/** @} */
