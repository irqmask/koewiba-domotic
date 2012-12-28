#include "system.h"

#ifdef _SYS_LINUX
#include <sys/time.h>
#endif
#ifdef _SYS_WINDOWS_
#include <windows.h>
#endif

#include "systime.h"

#ifdef _SYS_LINUX

uSysTime_t SYS_uGetTimeUSecs(void)
{
    uSysTime_t usecs;
    struct timeval timestamp;

    gettimeofday(&timestamp, 0);

    usecs = timestamp.tv_sec * 1000000;
    usecs += timestamp.tv_usec;
    return usecs;
}
#endif

#ifdef _SYS_WINDOWS_
uSysTime_t SYS_uGetTimeUSecs(void)
{
    LARGE_INTEGER     usecs, freq;
	double            temp;

	usecs.QuadPart = 0;
	freq.QuadPart = 0;

	QueryPerformanceCounter(&usecs);
	
	if (QueryPerformanceFrequency(&freq) && freq.QuadPart > 0) {
		temp = usecs.QuadPart * 1000000;
		temp /= freq.QuadPart;
		usecs.QuadPart = (uSysTime_t)temp;
	}
	
	return (uSysTime_t)usecs.QuadPart;
}
#endif
