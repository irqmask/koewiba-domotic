/**
 * @addtogroup ROOMTHERMOSTATE
 * @brief Configuration of modules used in "roomthermostate" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "roomthermostate" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    APP_eReg_DesiredTempDay1 = MOD_eReg_FirstAppSpecific,
    APP_eReg_DesiredTempNight1,
    APP_eReg_DesiredTempDay2,
    APP_eReg_DesiredTempNight2,
    APP_eReg_DesiredTempAway,
    APP_eReg_DesiredTempWindowOpened,
    APP_eReg_CurrentDesiredTemperature,
    APP_eReg_CurrentTemperature,

    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_DesiredTempDay1 = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_DesiredTempDay1Lo = APP_eCfg_DesiredTempDay1,
    APP_eCfg_DesiredTempDay1High,
    APP_eCfg_DesiredTempNight1,
    APP_eCfg_DesiredTempNight1Lo = APP_eCfg_DesiredTempNight1,
    APP_eCfg_DesiredTempNight1High,
    APP_eCfg_DesiredTempDay2,
    APP_eCfg_DesiredTempDay2Lo = APP_eCfg_DesiredTempDay2,
    APP_eCfg_DesiredTempDay2High,
    APP_eCfg_DesiredTempNight2,
    APP_eCfg_DesiredTempNight2Lo = APP_eCfg_DesiredTempNight2,
    APP_eCfg_DesiredTempNight2High,
    APP_eCfg_DesiredTempAway,
    APP_eCfg_DesiredTempAwayLo = APP_eCfg_DesiredTempAway,
    APP_eCfg_DesiredTempAwayHigh,
    APP_eCfg_DesiredTempWindowOpened,
    APP_eCfg_DesiredTempWindowOpenedLo = APP_eCfg_DesiredTempWindowOpened,
    APP_eCfg_DesiredTempWindowOpenedHigh,
    APP_eCfg_Last
} eConfiguration_t;

#define PWM_LINEAR_TABLE


// bus scheduler and transport special unittest gluecode and settings



// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
