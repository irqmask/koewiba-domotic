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
#ifndef _SYSCONSOLE_H_
#define _SYSCONSOLE_H_

#include "prjconf.h"

#include <stdio.h>

#if defined (PRJCONF_WINDOWS)
  #include <conio.h>
  #include <stdarg.h>
#endif

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #define sys_con_getch() getch()
#elif defined (PRJCONF_WINDOWS) 
  #define sys_con_getch() _getch()
#endif

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
char getch(void);
char getche(void);
#endif

#endif
