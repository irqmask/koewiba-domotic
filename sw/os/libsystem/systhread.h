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

// --- Definitions -------------------------------------------------------------

#if defined (PRJCONF_WINDOWS)
#  define sys_thread_func(name) DWORD WINAPI name( LPVOID p)

#  define sys_thread_start(func)    do{                 \
                                        CreateThread(NULL, 0, (func), NULL, 0, NULL);\
                                    }while(0);
#else
#  define sys_thread_func(name) void *name(void *p)

#  define sys_thread_start(func)    do{                 \
                                        pthread_t tid;   \
                                        pthread_create(&tid, NULL, (func), NULL);\
                                    }while(0);
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _SYSTHREAD_H_ */
/** @} */
