/**
 * @addtogroup CLOCK
 * @brief System clock.
 *
 * @{
 * @file    clock.c
 * @brief   System clock.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include "prjtypes.h"
#include "clock.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

volatile sClkTimer_t* g_asRunningTimers[CLOCK_NUM_TIMER];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Register timer in list.
 */
BOOL bRegisterTimer(sClkTimer_t* psTimer)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii] == NULL) {
            g_asRunningTimers[ii] = psTimer;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Sign off timer in list.
 */
BOOL bDeregisterTimer(sClkTimer_t* psTimer)
{
    uint8_t ii;
    BOOL    found = FALSE;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii] == psTimer) {
            g_asRunningTimers[ii] = NULL;
            found = TRUE;
        }
        else if(found)
        {
        	REG_TIMER1_IRQMSK &= ~(1<<REGBIT_TIMER1_OCIEA); // disable OutputCompareMatchA interrupt
        	// shift following timers, to avoid gaps in the list.
        	g_asRunningTimers[ii-1] = g_asRunningTimers[ii];
        }
    }
    REG_TIMER1_IRQMSK |= (1<<REGBIT_TIMER1_OCIEA); // enable OutputCompareMatchA interrupt
    return found;
}

/**
 * Timer interrupt. Increase clock and iterate through running timers list.
 */
ISR(TIMER1_COMPA_vect)
{
    uint8_t ii;

    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        if (g_asRunningTimers[ii] != NULL) {
            if (g_asRunningTimers[ii]->uTicks == 0) {
                // time elapsed, remove timer from list
                g_asRunningTimers[ii] = NULL;
            } else {
                g_asRunningTimers[ii]->uTicks--;
            }
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize clock module. Reset data and start hardware timer.
 */
void CLK_vInitialize(void)
{
    uint8_t ii;

    // clear running timers list
    for (ii=0; ii<CLOCK_NUM_TIMER; ii++) {
        g_asRunningTimers[ii] = NULL;
    }

    // initialize timer interrupt every 10th second (fOc = 10)
    //                      F_CPU
    // fIrq = 2 * fOC  = ------------
    //                    N * (OC+1)
    CLOCK_TCCRA = 0;
    CLOCK_TCCRB &= ~((1<<CLOCKSELECT2) | (1<<WGM13));
    CLOCK_TCCRB |= ((1<<CLOCKSELECT1) | (1<<CLOCKSELECT0)); // set prescaler to 1/64 (N=64)
    CLOCK_TCCRB |= (1<<WGM12);                                                  // CTC mode (clear timer on compare match)
    CLOCK_OCRA_H = (F_CPU/(64*CLOCK_TICKS_PER_SECOND) - 1) >> 8;
    CLOCK_OCRA_L = (F_CPU/(64*CLOCK_TICKS_PER_SECOND) - 1) & 0x00FF;
    REG_TIMER1_IRQMSK |= (1<<REGBIT_TIMER1_OCIEA); // enable OutputCompareMatchA interrupt
}

/**
 * Start/Stop Clock-Timer
 * 
 * @param[in] start
 * boolean for starting/stopping the timer (TRUE = start)
 */
void CLK_vControl(BOOL start)
{
	static uint8_t tccr1b = 0;

	if(start){
		if (0==tccr1b) return;
		CLOCK_TCCRB = tccr1b;
	}
	else{
		tccr1b = CLOCK_TCCRB;
		CLOCK_TCCRB &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));
	}
}

/**
 * Start a new count-down timer.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 * @param[in] uTime
 * Time in ticks. Convert from milliscons to ticks with 
 * CLOCK_MS_2_TICKS macro.
 *
 * @returns TRUE, if timer has been started, otherwise FALSE.
 */
BOOL CLK_bTimerStart(sClkTimer_t* psTimer, uint16_t uTicks)
{
    // check if timer is still running
    if (psTimer->uTicks != 0) {
        return FALSE;
    }

    psTimer->uTicks = uTicks;
    return bRegisterTimer(psTimer);
}

/**
 * Check if timer elapsed.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if time is over, otherwise false.
 */
BOOL CLK_bTimerIsElapsed(sClkTimer_t* psTimer)
{
    if (psTimer->uTicks == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/** @} */
