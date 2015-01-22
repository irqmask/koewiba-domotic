/**
 * @addtogroup IOLOOP
 *
 * @{
 * @file    ioloop.c
 * @brief   Event management for data input/output and timer for kwb applications.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

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
    uint16_t                expiration_time;
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

    FD_ZERO(&ioloop->read_fd_set);
    FD_ZERO(&ioloop->write_fd_set);

    conn = ioloop->first_conn;
    while (conn != NULL) {
        if (conn->eventtype == eIOLOOP_EV_READ) {
            FD_SET(conn->fd, &ioloop->read_fd_set);
        } else if (conn->eventtype == eIOLOOP_EV_WRITE) {
            FD_SET(conn->fd, &ioloop->write_fd_set);
        }
        conn = conn->next;
    }
    ioloop->update_required = false;
}

static void ioloop_insert_conn (ioloop_t* ioloop, ioloop_connection_t* conn)
{
    // insert at beginning
    conn->next = ioloop->first_conn;
    ioloop->first_conn = conn;
}

/**
 * Timer is removed from list, not deleted.
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

static void ioloop_insert_timer (ioloop_t* ioloop, ioloop_timer_t* timer)
{
    ioloop_timer_t *curr_timer, *prev_timer;

    if (ioloop->first_timer == NULL) {
        ioloop->first_timer = timer;
    } else {
        curr_timer = ioloop->first_timer;
        prev_timer = NULL;
        while (curr_timer != NULL) {
            if (curr_timer->expiration_time >= timer->expiration_time) {
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

static uint16_t ioloop_get_current_ticks (void)
{
    sys_time_t      now;

    now = sys_time_get_usecs();
    now /= 1000;
    now *= CLOCK_TICKS_PER_SECOND;
    now /= 1000;

    return (uint16_t) now;
}

static uint16_t ioloop_get_next_timeout (ioloop_t* ioloop)
{
    uint16_t now;

    // no timer registered? -> default value 1s
    if (ioloop->first_timer == NULL) {
        return ioloop->default_timeout_ticks;
    }
    
    // get difference time to now from first registered timer
    now = ioloop_get_current_ticks();
    if (now > ioloop->first_timer->expiration_time) return 1;
    else return ioloop->first_timer->expiration_time - now;
}

static void ioloop_check_timer (ioloop_t* ioloop)
{
    uint16_t        new_interval, now;
    ioloop_timer_t* timer;

    now = ioloop_get_current_ticks();
    timer = ioloop->first_timer;

    while (timer != NULL) {
        // iterate over all timers which are expired.
        if (timer->expiration_time <= now) {
            if (timer->callback) {
                new_interval = timer->callback(timer->arg);
            }
            ioloop_remove_timer(ioloop, timer);
            // timer is removed, if callback returns negative interval or if
            // timer is non-run_cyclic.
            // interval time remains unchanged, if return value of callback is 0.
            if (timer->run_cyclic && new_interval >= 0) {
                if (new_interval > 0) timer->interval_ticks = new_interval;
                timer->expiration_time += timer->interval_ticks;
                ioloop_insert_timer(ioloop, timer);
            } else {
                free(timer);
            }
        } else break;
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
        if (fd > ioloop->highest_fd) ioloop->highest_fd = fd;
    } while (0);
}

void ioloop_unregister_fd (ioloop_t* ioloop,
                           sys_fd_t fd)
{
    ioloop_connection_t* conn;

    assert(ioloop != NULL);

    conn = ioloop->first_conn;

    while (conn != NULL) {
        if (conn->fd == fd) {
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
                               ioloop_event_type_t  eventtype,
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
        timer->expiration_time = now + timer->interval_ticks;

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

int ioloop_run_once (ioloop_t* ioloop)
{
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
    timeout_us = (1000000 * ioloop_get_next_timeout(ioloop)) / CLOCK_TICKS_PER_SECOND;
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

    return rc;
}

/** @} */
