/**
 * @addtogroup SYSTHREAD
 * @brief Thread interface.
 *
 * Start and manage threads.
 *
 * @{
 * @file    systhread.h
 * @brief   Thread interface.
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
#ifndef _SYSTHREAD_H_
#define _SYSTHREAD_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_WINDOWS
#  include <windows.h>
#else
#  include <pthread.h>
#  include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

#if defined (PRJCONF_WINDOWS)
#  define sys_thread_func(name) DWORD WINAPI name( LPVOID p)

#  define sys_thread_start(func)    do{                 \
                                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(func), NULL, 0, NULL);\
                                    }while(0);
#else
#  define sys_thread_func(name) void *name(void *p)

#  define sys_thread_start(func)    do{                 \
                                        pthread_t tid;   \
                                        pthread_create(&tid, NULL, func, NULL);\
                                    }while(0);
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* _SYSTHREAD_H_ */
/** @} */
