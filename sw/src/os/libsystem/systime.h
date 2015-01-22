/**
 * @addtogroup SYSTIME
 * @brief Socket interface.
 *
 * System independent interface to system time functions.
 *
 * @{
 * @file    systime.h
 * @brief   System independent interface to system time functions.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _SYSTIME_H_
#define _SYSTIME_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <stdint.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <sys/time.h>
#elif defined (PRJCONF_WINDOWS)
  #include <windows.h>
#endif

// --- Definitions -------------------------------------------------------------

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #define sys_sleep_ms(x) do{usleep((x)*1000);}while(0)
#elif defined (PRJCONF_WINDOWS)
  #define sys_sleep_ms(x) do{Sleep(x);}while(0)
#endif

// --- Type definitions --------------------------------------------------------

typedef uint64_t sys_time_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

sys_time_t sys_time_get_usecs (void);

#endif /* _SYSTIME_H_ */
/** @} */
