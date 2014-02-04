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
#define PWM_CHN0_PORT       g_uPort
#define PWM_CHN1_PORT       g_uPort
#define PWM_CHN2_PORT       g_uPort

#define PWM_CHN0_DDR        g_uDDR
#define PWM_CHN1_DDR        g_uDDR
#define PWM_CHN2_DDR        g_uDDR

#define PWM_CHN0_PIN        0
#define PWM_CHN1_PIN        1
#define PWM_CHN2_PIN        2

#define PWMCOUNT            g_uPWMCount
#define PWMCOMPARE          g_uPWMCompare


// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern uint8_t  g_uPort;
extern uint8_t  g_uDDR;
extern uint16_t g_uPWMCount;
extern uint16_t g_uPWMCompare;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
