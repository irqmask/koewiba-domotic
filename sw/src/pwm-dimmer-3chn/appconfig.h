/**
 * @addtogroup PWM-DIMMER-3CHN
 * @addtogroup PWM-DIMMER-3CHN_APPCONFIG
 * @brief Configuration of modules used in "pwm-dimmer-3chn" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "pwm-dimmer-3chn" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG       1
#undef BUS_SCHEDULER        //!< This program has no scheduling capabilities.

#define SPI_APPCONFIG       1
#define SPI_WITH_BLOCKING   1
#undef  SPI_WITH_IRQ

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    APP_eReg_PWMChn1 = MOD_eReg_FirstAppSpecific,
    APP_eReg_PWMChn2,
    APP_eReg_PWMChn3,

    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non-volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_Dummy = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_Last
} eConfiguration_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
/** @} */
