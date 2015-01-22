/**
 * @addtogroup SYSTEM
 * @brief Library to cover operating system dependent functions.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    system.h
 * @brief   Library to cover operating system dependent functions.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

// --- Include section ---------------------------------------------------------

#include "error_codes.h"
#include "prjconf.h"

#if defined (PRJCONF_WINDOWS)
#include <windows.h>
#endif

// --- Definitions -------------------------------------------------------------

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #define INVALID_FD (-1)
#elif defined (PRJCONF_WINDOWS)
  #define INVALID_FD INVALID_HANDLE_VALUE
#endif

// --- Type definitions --------------------------------------------------------

typedef enum {
    eSYS_ERR_NONE = eERR_NONE,
    
    eSYS_ERR_SER_UNSUPPORTED = 200,
    eSYS_ERR_SER_CONFIGURE,
} sys_errors_t;

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
typedef int sys_fd_t;
#elif defined (PRJCONF_WINDOWS)
typedef HANDLE sys_fd_t;
#endif

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _SYSTEM_H_ */
/** @} */
