#include "delay.h"


//--------------------------------------------------------------------------
//
//  Function:    TimerDelayUsec
//
//  Description: Will take up execution time for the number of microseconds
//               requested.
//
//  Inputs:      Number of milliseconds to delay
//
//--------------------------------------------------------------------------
// spin for us microseconds
void TimerDelayUsec (unsigned int us)
{
  if (!us)
    return;

  /* the inner loop takes 4 cycles per iteration */
  __asm__ __volatile__ (
      "1:                     \n"
      "       ldi r26, %3     \n"
      "       ldi r27, %2     \n"
      "2:     sbiw r26, 1     \n"
      "       brne 2b         \n"
      "       sbiw %0, 1      \n"
      "       brne 1b         \n"
      : "=w" (us)
        : "w" (us), "i" (LOOPS_PER_US >> 8), "i" (0xff & LOOPS_PER_US)
  );
}



//--------------------------------------------------------------------------
//
//  Function:    TimerDelayMsec
//
//  Description: Will take up execution time for the number of milliseconds
//               requested.
//
//  Inputs:      Number of milliseconds to delay
//
//--------------------------------------------------------------------------
// spin for ms milliseconds
void TimerDelayMsec (unsigned int ms)
{
        if (!ms)
                return;

        // the inner loop takes 4 cycles per iteration
        __asm__ __volatile__ (
                "1:                     \n"
                "       ldi r26, %3     \n"
                "       ldi r27, %2     \n"
                "2:     sbiw r26, 1     \n"
                "       brne 2b         \n"
                "       sbiw %0, 1      \n"
                "       brne 1b         \n"
                : "=w" (ms)
                : "w" (ms), "i" (LOOPS_PER_MS >> 8), "i" (0xff & LOOPS_PER_MS)
                );
}
