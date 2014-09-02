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

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct ioloop_connection {
    ioloop_connection_t*    next;
    sys_fd_t                fd;
    ioloop_event_type_t     eventtype;
    ioloop_event_func_t     callback;
    void*                   arg;
} ioloop_connection_t;

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

        conn->next = ioloop->first_conn;
        ioloop->first_conn = conn;
    } while (0);

    return conn;
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

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void ioloop_init (ioloop_t* ioloop)
{
    assert(ioloop != NULL);

    memset(ioloop, 0, sizeof(ioloop_t));
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

        ioloop->update_required = true;
        if (fd > ioloop->highest_fd) ioloop->highest_fd = fd;
    } while (0);
}

void ioloop_unregister_fd (ioloop_t* ioloop,
                           sys_fd_t fd)
{
    ioloop_connection_t* conn;
    ioloop_connection_t* conn_to_delete;

    conn = ioloop->first_conn;

    if (conn == NULL) return;
    if (conn->fd == fd) {
        ioloop->first_conn = conn->next;
        free(conn);
    } else {
        while (conn->next != NULL && conn->next->fd != fd) {
            conn = conn->next;
        }
        if (conn->next != NULL && conn->next->fd == fd) {
            conn_to_delete = conn->next;
            conn->next = conn->next->next;
            free(conn_to_delete);
        }
    }
    ioloop->update_required = true;
}

int ioloop_run_once (ioloop_t* ioloop)
{
    int                     rc;
    ioloop_connection_t*    conn;
    struct timeval          timeout;
    sys_fd_t                highest_fd;
    fd_set                  read_fds, write_fds;

    assert(ioloop != NULL);

    // update list of file descriptors for select
    if (ioloop->update_required) {
        ioloop_update_fd_sets(ioloop);
    }

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    memcpy(&read_fds, &ioloop->read_fd_set, sizeof(fd_set));
    memcpy(&write_fds, &ioloop->write_fd_set, sizeof(fd_set));

    highest_fd = ioloop->highest_fd > 0 ? ioloop->highest_fd : FD_SETSIZE;
    if (highest_fd < FD_SETSIZE) highest_fd += 1;
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

    return rc;
}

/** @} */
