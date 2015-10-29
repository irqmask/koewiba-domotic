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

#ifndef _IOLOOP_H_
#define _IOLOOP_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <sys/types.h>
#elif defined (PRJCONF_WINDOWS)
#endif

#include "prjtypes.h"
#include "system.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum {
    eIOLOOP_EV_UNKNOWN,
    eIOLOOP_EV_READ,
    eIOLOOP_EV_WRITE,
    eIOLOOP_EV_TIMER
} ioloop_event_type_t;

typedef int32_t (*ioloop_event_func_t)(void* arg);

typedef struct ioloop_connection ioloop_connection_t;

typedef struct ioloop_timer ioloop_timer_t;

typedef struct ioloop {
    ioloop_connection_t*    first_conn;
    ioloop_timer_t*         first_timer;
    bool                    update_required;
    sys_fd_t                highest_fd;
    fd_set                  read_fd_set;
    fd_set                  write_fd_set;
    int32_t                 next_id;
    uint16_t                default_timeout_ticks;
} ioloop_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void ioloop_init (ioloop_t* ioloop);

void ioloop_register_fd (ioloop_t* ioloop,
                         sys_fd_t fd,
                         ioloop_event_type_t eventtype,
                         ioloop_event_func_t callback,
                         void* arg);

void ioloop_unregister_fd (ioloop_t* ioloop,
                           sys_fd_t fd);

void ioloop_set_default_timeout (ioloop_t* ioloop,
                                 uint16_t  timeout_ticks);

int32_t ioloop_register_timer (ioloop_t*            ioloop,
                               uint16_t             interval_ticks,
                               bool                 run_cyclic,
                               ioloop_event_type_t  eventtype,
                               ioloop_event_func_t  callback,
                               void*                arg);

void ioloop_unregister_timer (ioloop_t* ioloop,
                              int32_t   id);

int ioloop_run_once (ioloop_t* ioloop);

#endif /* _IOLOOP_H_ */
/** @} */