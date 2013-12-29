/**
 * @addtogroup TASTER8_APPCONFIG
 * @brief Configuration of modules used in "taster8" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "taster8" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG   1
#undef BUS_SCHEDULER        //!< This program has scheduling capabilities.

//! Application specific layout of registers
typedef enum appregisters {
    APP_eReg_Dummy = MOD_eReg_FirstAppSpecific,
    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_Dummy = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_Last
} eConfiguration_t;

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
