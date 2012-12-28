#ifndef _SYSTIME_H_
#define _SYSTIME_H_

#include "system.h"
#include <stdint.h>

#ifdef _SYS_LINUX_
# define SYS_vSleepMs(x) do{usleep((x)*1000);}while(0)
#endif

#ifdef _SYS_WINDOWS_
# include <windows.h>

# define SYS_vSleepMs(x) do{Sleep(x);}while(0)
#endif

typedef uint64_t uSysTime_t;

uSysTime_t SYS_uGetTimeUSecs(void);

#endif
