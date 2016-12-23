/**
 * @addtogroup BASIC_ATMEGA328
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "basic_atmega328" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "basic_atmega328" project.
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
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_PositionCurrent = MOD_eReg_FirstAppSpecific, //!< Current position of blind
    APP_eReg_PositionSetPoint,  //!< Wanted position of blind.
    APP_eReg_ReactionDelay,     //!< Delay time until movement starts after applying power.
    APP_eReg_DurationOpen,      //!< Duration to fully open the blind.
    APP_eReg_DurationClose,     //!< Duration to fully close the blind.

    // insert application specific registers here
    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_PositionCurrent = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_PositionCurrentLow = APP_eCfg_PositionCurrent,
    APP_eCfg_PositionCurrentHigh,
    APP_eCfg_PositionSetPoint,
    APP_eCfg_PositionSetPointLow = APP_eCfg_PositionSetPoint,
    APP_eCfg_PositionSetPointHigh,

    // insert application specific configuration here
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
