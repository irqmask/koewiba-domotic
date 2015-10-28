/**
 * @addtogroup UNITTESTS_PCBCONFIG
 * @brief PCB configuration of the unittests.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the unittests.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// PCB configuration of module PWM
#define PWM_PCBCONFIG       1
#define PWM_CHN_PORT        g_uPort
#define PWM_CHN_DDR         g_uDDR

#define PWM_CHN0_PIN        0
#define PWM_CHN1_PIN        1
#define PWM_CHN2_PIN        2

#define PWMCOUNT            g_uPWMCount
#define PWMCOMPARE          g_uPWMCompare
#define PWM_INTERRUPT       void PWM_vOnIRQ(void)

#define PWM_MAX_COUNT		255

// prozessor dependent register and defines
#define SLEEP_MODE_IDLE     1
#define REGBIT_TIMER2_CS0   1
#define REGBIT_TIMER2_CS1   2
#define REGBIT_TIMER2_CS2   3
#define REGBIT_TIMER2_OCIEA 1
#define REG_TIMER2_TCCRA    g_uAvrTimer1_TCCRA
#define REG_TIMER2_TCCRB    g_uAvrTimer1_TCCRB
#define REG_TIMER2_IRQMSK   g_uAvrTimer1_IRQMSK

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern uint8_t  g_uPort;
extern uint8_t  g_uDDR;
extern uint8_t  g_uPWMCount;
extern uint8_t  g_uPWMCompare;

extern uint8_t  g_uAvrTimer1_TCCRA;
extern uint8_t  g_uAvrTimer1_TCCRB;
extern uint8_t  g_uAvrTimer1_IRQMSK;


// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
