/**
 * @addtogroup IOLOOP
 *
 * @{
 * @file    ioloop.c
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

// --- Include section ---------------------------------------------------------

#include <assert.h>
#include <malloc.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <sys/time.h>
  #include <sys/types.h>
#elif defined (PRJCONF_WINDOWS)
  #include <windows.h>
#endif

#include "ioloop.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct ioloop_connection {
    ioloop_connection_t*    next;
    sys_fd_t                fd;
    ioloop_event_type_t     eventtype;
    ioloop_event_func_t     callback;
    void*                   arg;
} ioloop_connection_t;

typedef struct ioloop_timer {
    ioloop_timer_t*         next;
    int32_t                 id;
    uint16_t                interval_ticks;
    uint16_t                last_hit;
    bool                    run_cyclic;
    ioloop_event_type_t     eventtype;
    ioloop_event_func_t     callback;
    void*                   arg;
} ioloop_timer_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static ioloop_connection_t* ioloop_new_connection (ioloop_t* ioloop)
{
    ioloop_connection_t* conn;

    do {
        conn = (ioloop_connection_t*)calloc(1, sizeof(ioloop_connection_t));
        if (conn == NULL) {
            perror("new ioloop connection could not be created");
            break;
        }
    } while (0);

    return conn;
}

static ioloop_timer_t* ioloop_new_timer (ioloop_t* ioloop)
{
    ioloop_timer_t* timer;

    do {
        timer = (ioloop_timer_t*)calloc(1, sizeof(ioloop_timer_t));
        if (timer == NULL) {
            perror("new ioloop connection could not be created");
            break;
        }
    } while (0);

    return timer;
}

static int32_t ioloop_get_id (ioloop_t* ioloop)
{
    int32_t id = -1;

    if (ioloop->next_id >= 0) {
        id = ioloop->next_id;
        if (ioloop->next_id == INT32_MAX) {
            // stop generating ids
            ioloop->next_id = -1;
        } else {
            ioloop->next_id++;
        }
    }
    return id;
}

static void ioloop_update_fd_sets (ioloop_t* ioloop)
{
    ioloop_connection_t* conn;

    conn = ioloop->first_conn;

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    FD_ZERO(&ioloop->read_fd_set);
    FD_ZERO(&ioloop->write_fd_set);
    while (conn != NULL) {
        if (conn->eventtype == eIOLOOP_EV_READ) {
            FD_SET(conn->fd, &ioloop->read_fd_set);
        } else if (conn->eventtype == eIOLOOP_EV_WRITE) {
            FD_SET(conn->fd, &ioloop->write_fd_set);
        }
        conn = conn->next;
    }
#elif defined (PRJCONF_WINDOWS)
    memset(ioloop->list_of_fds, 0, sizeof(ioloop->list_of_fds));
    while (conn != NULL && ioloop->num_list_entries < MAXIMUM_WAIT_OBJECTS) {
        if (conn->eventtype == eIOLOOP_EV_READ || conn->eventtype == eIOLOOP_EV_WRITE) {
            ioloop->list_of_fds[ioloop->num_list_entries] = conn->fd;
            ioloop->list_of_conns[ioloop->num_list_entries] = conn;
            ioloop->num_list_entries++;
        }
    }
#endif
    ioloop->update_required = false;
}

static void ioloop_insert_conn (ioloop_t* ioloop, ioloop_connection_t* conn)
{
    // insert at beginning
    conn->next = ioloop->first_conn;
    ioloop->first_conn = conn;
}

/**
 * Remove connection from active connection's list. Connection is not deleted.
 */
static void ioloop_remove_conn (ioloop_t* ioloop, ioloop_connection_t* conn)
{
    ioloop_connection_t *curr_conn, *prev_conn;

    curr_conn = ioloop->first_conn;
    prev_conn = NULL;
    while (curr_conn != NULL) {
        if (curr_conn == conn) {
            if (prev_conn != NULL) {
                prev_conn->next = curr_conn->next;
            } else {
                ioloop->first_conn = curr_conn->next;
            }
        }
        prev_conn = curr_conn;
        curr_conn = curr_conn->next;
    }
}

/**
 * Remove timer into active timer's list.
 */
static void ioloop_insert_timer (ioloop_t* ioloop, ioloop_timer_t* timer)
{
    ioloop_timer_t *curr_timer, *prev_timer;

    if (ioloop->first_timer == NULL) {
        ioloop->first_timer = timer;
    } else {
        curr_timer = ioloop->first_timer;
        prev_timer = NULL;
        while (curr_timer != NULL) {
            if ((curr_timer->last_hit + curr_timer->interval_ticks) >= (timer->last_hit + timer->interval_ticks)) {
                timer->next = curr_timer;
                if (prev_timer != NULL) {
                    // insert in between
                    prev_timer->next = timer;
                } else {
                    // insert at beginning
                    ioloop->first_timer = timer;
                }
            }
            prev_timer = curr_timer;
            curr_timer = curr_timer->next;
        }
    }
}

/**
 * Timer is removed from list, not deleted.
 */
static void ioloop_remove_timer (ioloop_t* ioloop, ioloop_timer_t* timer)
{
    ioloop_timer_t *curr_timer, *prev_timer;

    curr_timer = ioloop->first_timer;
    prev_timer = NULL;
    while (curr_timer != NULL) {
        if (curr_timer == timer) {
            if (prev_timer != NULL) {
                prev_timer->next = curr_timer->next;
            } else {
                ioloop->first_timer = curr_timer->next;
            }
        }
        prev_timer = curr_timer;
        curr_timer = curr_timer->next;
    }
}

/**
 * Get current time and convert it into ticks.
 * @returns current time in ticks.
 */
static uint16_t ioloop_get_current_ticks (void)
{
    sys_time_t      now;

    now = sys_time_get_usecs();
    now /= 1000;
    now *= CLOCK_TICKS_PER_SECOND;
    now /= 1000;

    return (uint16_t) now;
}

/**
 * Calculate number of ticks from now until next timeout
 * @param[in]   ioloop      ioloop handle
 * @returns time in ticks until next timer timeout.
 */
static uint16_t ioloop_get_next_timeout (ioloop_t* ioloop)
{
    uint16_t now, diff;

    // no timer registered? -> default value 1s
    if (ioloop->first_timer == NULL) {
        return ioloop->default_timeout_ticks;
    }

    // get difference time to now from first registered timer
    now = ioloop_get_current_ticks();
    diff = now - ioloop->first_timer->last_hit;
    if (diff > ioloop->first_timer->interval_ticks) return 1;
    else return (ioloop->first_timer->interval_ticks - diff);
}

/**
 * Loop over registered timers, check timeout, call registered callbacks on timeout.
 * @param[in]   ioloop      ioloop handle
 */
static void ioloop_check_timer (ioloop_t* ioloop)
{
    int32_t         new_interval;
    uint16_t        now, diff;
    ioloop_timer_t* timer;

    now = ioloop_get_current_ticks();
    timer = ioloop->first_timer;

    while (timer != NULL) {
        // iterate over all timers which are expired.
        diff = now - timer->last_hit;
        if (diff >= timer->interval_ticks) {
            new_interval = 0;
            if (timer->callback) {
                new_interval = timer->callback(timer->arg);
            }
            ioloop_remove_timer(ioloop, timer);
            // timer is removed, if callback returns negative interval or if
            // timer is non-run_cyclic.
            // interval time remains unchanged, if return value of callback is 0.
            if (timer->run_cyclic && new_interval >= 0) {
                if (new_interval > 0) {
                    if (new_interval > INT16_MAX) new_interval = INT16_MAX;
                    timer->interval_ticks = new_interval;
                }
                timer->last_hit += timer->interval_ticks;
                ioloop_insert_timer(ioloop, timer);
            } else {
                free(timer);
            }
            // iterate over whole list again
            timer = ioloop->first_timer;
            continue;
        }
        timer = timer->next;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void ioloop_init (ioloop_t* ioloop)
{
    assert(ioloop != NULL);

    memset(ioloop, 0, sizeof(ioloop_t));
    ioloop->default_timeout_ticks = 100; // 100 * 1/100second
    ioloop->update_required = true;
}

void ioloop_register_fd (ioloop_t*              ioloop,
                         sys_fd_t               fd,
                         ioloop_event_type_t    eventtype,
                         ioloop_event_func_t    callback,
                         void*                  arg)
{
    ioloop_connection_t* conn;

    assert(ioloop != NULL);

    do {
        conn = ioloop_new_connection(ioloop);
        if (conn == NULL) break;

        conn->fd        = fd;
        conn->eventtype = eventtype;
        conn->callback  = callback;
        conn->arg       = arg;

        ioloop_insert_conn(ioloop, conn);
        ioloop->update_required = true;
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
        if (fd > ioloop->highest_fd) ioloop->highest_fd = fd;
#endif
    } while (0);
}

void ioloop_unregister_fd (ioloop_t* ioloop,
                           sys_fd_t fd,
                           ioloop_event_type_t eventtype)
{
    ioloop_connection_t* conn;

    assert(ioloop != NULL);

    conn = ioloop->first_conn;

    while (conn != NULL) {
        if (conn->fd == fd && 
            (eventtype == eIOLOOP_EV_UNKNOWN || conn->eventtype == eventtype)) {
            ioloop_remove_conn(ioloop, conn);
            free(conn);
            break;
        }
        conn = conn->next;
    }
    ioloop->update_required = true;
}

void ioloop_set_default_timeout (ioloop_t* ioloop,
                                 uint16_t  timeout_ticks)
{
    assert(ioloop != NULL);

    ioloop->default_timeout_ticks = timeout_ticks;
}

int32_t ioloop_register_timer (ioloop_t*            ioloop,
                               uint16_t             interval_ticks,
                               bool                 run_cyclic,
                               ioloop_event_func_t  callback,
                               void*                arg)
{
    int32_t         id = -1;
    ioloop_timer_t* timer;
    uint16_t        now;

    assert(ioloop != NULL);

    do {
        timer = ioloop_new_timer(ioloop);
        if (timer == NULL) break;

        id = ioloop_get_id(ioloop);
        if (id < 0) {
            free(timer);
            break;
        }
        timer->id = id;
        timer->interval_ticks = interval_ticks;
        timer->run_cyclic = run_cyclic;
        timer->eventtype = eIOLOOP_EV_TIMER;
        timer->callback = callback;
        timer->arg = arg;

        now = ioloop_get_current_ticks();
        timer->last_hit = now;

        ioloop_insert_timer(ioloop, timer);
    } while (0);
    return id;
}

void ioloop_unregister_timer (ioloop_t* ioloop,
                              int32_t   id)
{
    ioloop_timer_t* timer;

    assert(ioloop != NULL);

    timer = ioloop->first_timer;

    if (timer == NULL) return;

    while (timer->next != NULL) {
        if (timer->next->id == id) {
            ioloop_remove_timer(ioloop, timer);
            free(timer);
            break;
        }
        timer = timer->next;
    }
}

void ioloop_run_once (ioloop_t* ioloop)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)

    int                     rc;
    ioloop_connection_t*    conn;
    sys_time_t              timeout_us;
    struct timeval          timeout;
    sys_fd_t                highest_fd;
    fd_set                  read_fds, write_fds;

    assert(ioloop != NULL);

    // update list of file descriptors for select
    if (ioloop->update_required) {
        ioloop_update_fd_sets(ioloop);
    }

    // check for elapsed timers
    ioloop_check_timer(ioloop);

    // calculate highest fd for select
    if (ioloop->highest_fd > 0) {
        highest_fd = ioloop->highest_fd;
        if (highest_fd < FD_SETSIZE) highest_fd += 1;
    } else {
        highest_fd = FD_SETSIZE;
    }

    memcpy(&read_fds, &ioloop->read_fd_set, sizeof(fd_set));
    memcpy(&write_fds, &ioloop->write_fd_set, sizeof(fd_set));

    // calculate next timeout
    timeout_us = ioloop_get_next_timeout(ioloop);
    timeout_us *= 1000000;
    timeout_us /= CLOCK_TICKS_PER_SECOND;

    // if both tv_sec and tv_usec are zero select returns immediately
    timeout.tv_sec = timeout_us / 1000000;
    timeout.tv_usec = timeout_us % 1000000;

    // wait for action on fds or timeout
    rc = select(highest_fd,
                &read_fds,
                &write_fds,
                NULL, &timeout);

    // execute callbacks for every fd which has been set
    if (rc > 0) {
        conn = ioloop->first_conn;
        while (conn != NULL) {
            if (conn->eventtype == eIOLOOP_EV_READ) {
                if (FD_ISSET(conn->fd, &read_fds) &&
                    conn->callback != NULL) {
                    conn->callback(conn->arg);
                }
            } else if (conn->eventtype == eIOLOOP_EV_WRITE) {
                if (FD_ISSET(conn->fd, &write_fds) &&
                    conn->callback != NULL) {
                    conn->callback(conn->arg);
                }
            }

            conn = conn->next;
        }
    }

    // check for elapsed timers again
    ioloop_check_timer(ioloop);
#elif defined (PRJCONF_WINDOWS)
    uint32_t                rc, timeout_ms;
    ioloop_connection_t*    conn;

    assert(ioloop != NULL);

    // update list of file descriptors for select
    if (ioloop->update_required) {
        ioloop_update_fd_sets(ioloop);
    }

    // check for elapsed timers
    ioloop_check_timer(ioloop);

    timeout_ms = (1000 * ioloop_get_next_timeout(ioloop)) / CLOCK_TICKS_PER_SECOND;

    rc = WaitForMultipleObjects(ioloop->num_list_entries, ioloop->list_of_fds, FALSE, timeout_ms);
    if (rc >= WAIT_OBJECT_0 && rc < WAIT_OBJECT_0 + ioloop->num_list_entries) {
        conn = ioloop->list_of_conns[rc - WAIT_OBJECT_0];
        if (conn->callback != NULL) {
            conn->callback(conn->arg);
        }
    }

    // check for elapsed timers again
    ioloop_check_timer(ioloop);
#endif
}

/** @} */
