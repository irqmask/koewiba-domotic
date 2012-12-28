#include <sys/time.h>
#include <stdint.h>
#include "systime.h"

uSysTime_t SYS_uGetTimeUSecs(void)
{
    uSysTime_t usecs;
    struct timeval timestamp;

    gettimeofday(&timestamp, 0);

    usecs = timestamp.tv_sec * 1000000;
    usecs += timestamp.tv_usec;
    return usecs;
}