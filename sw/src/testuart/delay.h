#ifndef DELAY_H
#define DELAY_H
// delay.h


#define LOOPS_PER_US (F_CPU/1000/1000/4)
#define LOOPS_PER_MS (F_CPU/1000/4)


// spin for us microseconds
void TimerDelayUsec (unsigned int us);

// spin for ms milliseconds
void TimerDelayMsec (unsigned int ms);

#endif // DELAY_H
