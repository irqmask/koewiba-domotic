/**
 * @addtogroup SYSERROR
 * @brief System errors.
 *
 * Get error information of errors thrown by the operating system.
 *
 * @{
 * @file    syserror.h
 * @brief   System independent interface for operating system error handling.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _SYSERROR_H_
#define _SYSERROR_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)

#elif defined (PRJCONF_WINDOWS)
  #include <windows.h>
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void sys_err_get_as_string (char* buffer, size_t buffersize);
void sys_err_print_last (void);

#endif /* _SYSERROR_H_ */
/** @} */
