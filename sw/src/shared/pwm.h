/**
 * @addtogroup PWM
 * @brief Pulse width modulation driver.
 *
 * Contains functions to do software PWM on several outputs. The number of
 * outputs and which outputs to be used can be configured at compile time.
 *
 * Supports pcbconfig.h for individual PCB settings of the PWM pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    pwm.h
 * @brief   Pulse width modulation driver.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PWM_H_
#define _PWM_H_

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include "ucontroller.h"
#endif

#include "prjtypes.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection PWM_PCBCONFIG
 * Configure PWM pinout.
 * @{
 */
#ifndef PWM_PCBCONFIG
 #define PWM_PCBCONFIG      1
 #define PWM_CHN0_PORT      g_uPort
 #define PWM_CHN1_PORT      g_uPort
 #define PWM_CHN2_PORT      g_uPort

 #define PWM_CHN0_DDR       g_uDDR
 #define PWM_CHN1_DDR       g_uDDR
 #define PWM_CHN2_DDR       g_uDDR

 #define PWM_CHN0_PIN       0
 #define PWM_CHN1_PIN       1
 #define PWM_CHN2_PIN       2
#endif // PWM_PCBCONFIG
/** @} */

/**
 * @subsection PWM_APPCONFIG
 * Configure setiings of the PWM driver. E.g. base frequency.
 * @{
 */
#ifndef PWM_APPCONFIG
 #define PWM_APPCONFIG      1
 #define PWM_NUM_CHANNELS   3
#endif // PWM_APPCONFIG
/** @} */

#define PWM_MAX_COUNT       65535

// --- Type definitions --------------------------------------------------------

//! PWM data type for counters
typedef uint16_t PWM_uCounter_t;

typedef struct {
    PWM_uCounter_t  uCurrentPulseWidth;
    PWM_uCounter_t  uInverseCurrentPulseWidth;

    // irq runtime data
    PWM_uCounter_t  uNextWakeUp;
} sPWMChannel_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void                PWM_vInit               (void);

void                PWM_vSet                (uint8_t uChannel, uint8_t uValue);

void                PWM_vOnIRQ              (void);

#endif // _PWM_H_
/** @} */
