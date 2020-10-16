/**
 * @addtogroup IOLOOP
 * @brief Event management for data input/output and timer for kwb applications.
 *
 * Contains helper functions to register for input/output event on different
 * devices and also for timer routines of functions which have to be called
 * periodically.
 *
 * @{
 * @file    ioloop.h
 * @brief   Event management for data input/output and timer for kwb applications.
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
#ifndef _IOLOOP_H_
#define _IOLOOP_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <sys/types.h>
#elif defined (PRJCONF_WINDOWS)
    #include <windows.h>
#endif

#include "prjtypes.h"
#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Types of different ioloop events.
 */
typedef enum {
    eIOLOOP_EV_UNKNOWN,
    eIOLOOP_EV_READ,
    eIOLOOP_EV_WRITE,
    eIOLOOP_EV_TIMER
} ioloop_event_type_t;

/**
 * Function pointer type for ioloop events.
 */
typedef int32_t (*ioloop_event_func_t)(void *arg);

/**
 * Opaque ioloop connection type.
 */
typedef struct ioloop_connection ioloop_connection_t;

/**
 * Opaque ioloop timer type.
 */
typedef struct ioloop_timer ioloop_timer_t;

/**
 * ioloop runtime data structure.
 */
typedef struct ioloop {
    //! List of opened connections
    ioloop_connection_t    *first_conn;
    //! List of running timers
    ioloop_timer_t         *first_timer;
    //! Flag signing that an update of the file descriptor lists is needed
    bool                    update_required;
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    //! Highest file descriptor
    sys_fd_t                highest_fd;
    //! Linux file descriptor set for read events
    fd_set                  read_fd_set;
    //! Linux file descriptor set for write events
    fd_set                  write_fd_set;
#elif defined (PRJCONF_WINDOWS)
    //! Number of file descriptor entries
    uint32_t                num_list_entries;
    //! List of file descriptors
    sys_fd_t                list_of_fds[MAXIMUM_WAIT_OBJECTS];
    //! List of corresponding ioloop connections
    ioloop_connection_t    *list_of_conns[MAXIMUM_WAIT_OBJECTS];
#endif
    //! Id of next expiring timer
    int32_t                 next_id;
    //! Default timeout ticks of ioloop. At latest ioloop_run_once() will return
    //! after this timeout.
    uint16_t                default_timeout_ticks;
} ioloop_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize IO-loop instance
 * @param[in]   ioloop      IO-loop instance
 */
void ioloop_init(ioloop_t *ioloop);

/**
 * Close IO-loop instance.
 * All timers and connections will be deleted.
 * @param[in]   ioloop      IO-loop instance
 */
void ioloop_close(ioloop_t *ioloop);

/**
 * Register a file descriptor to IO-loop
 * @param[in]   ioloop      IO-loop instance
 * @param[in]   fd          File descriptor
 * @param[in]   eventtype   Set a callback for this event type
 * @param[in]   callback    Callback function to be registered
 * @param[in]   arg         Optional argument passed to the callback function
 */
void ioloop_register_fd(ioloop_t *ioloop,
                        sys_fd_t fd,
                        ioloop_event_type_t eventtype,
                        ioloop_event_func_t callback,
                        void *arg);

/**
 * Unregister a file descriptor from IO-loop
 * @param[in]   ioloop      IO-loop instance
 * @param[in]   fd          File descriptor
 * @param[in]   eventtype   Unset the callback for this event type
 */
void ioloop_unregister_fd(ioloop_t *ioloop,
                          sys_fd_t fd,
                          ioloop_event_type_t eventtype);

/**
 * Set a timeout in ticks for ioloop_run_once.
 * This is the maximum response time for ioloop_run_once().
 * @param[in]   ioloop      IO-loop instance
 * @param[in]   timeout_ticks   Timeout in ticks
 */
void ioloop_set_default_timeout(ioloop_t *ioloop,
                                uint16_t  timeout_ticks);

/**
 * Register a timer in IO-loop
 * @param[in]   ioloop      IO-loop instance
 * @param[in]   interval_ticks  Interval ticks until the timer expires
 * @param[in]   run_cyclic  Flag if the timer runs cyclic.
 * @param[in]   callback    Callback to be called when the timer expires
 * @param[in]   arg         Optional argument passed to the callback function.
 * @returns id of the new timer, otherwise -1.
 */
int32_t ioloop_register_timer(ioloop_t            *ioloop,
                              uint16_t             interval_ticks,
                              bool                 run_cyclic,
                              ioloop_event_func_t  callback,
                              void                *arg);

/**
 * Stop and unregister timer
 * @param[in]   ioloop      IO-loop instance
 * @param[in]   id          Id of the timer.
 */
void ioloop_unregister_timer(ioloop_t *ioloop,
                             int32_t   id);

/**
 * Run the io-loop wait for and handle upcoming events.
 * It will time-out if no event occurrs. Use ioloop_set_default_timeout() to alter the timeout.
 * @param[in]   ioloop      IO-loop instance
 */
void ioloop_run_once(ioloop_t *ioloop);

#ifdef __cplusplus
}
#endif

#endif /* _IOLOOP_H_ */
/** @} */
