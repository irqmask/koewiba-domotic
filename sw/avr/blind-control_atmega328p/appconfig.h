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

#include "cmddef_common.h"
#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG   1
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

#define APP_eSLEEPMASK_MOTOR    (1<<0)  //!< Mask used for sleep_prevent() function.
                                        //!< Prevent the controller to fall asleep if motor is moving.
// --- Type definitions --------------------------------------------------------

//! Application specific commands
typedef enum appcommands {
    APP_eCmd_Stop = eCMD_FIRST_APP_SPECIFIC,    //!< Stop blind movement
} eCommands_t;

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_PositionCurrent = MOD_eReg_FirstAppSpecific, //!< Current position of blind
    APP_eReg_PositionSetPoint,  //!< Wanted position of blind. 100 = completely closed. 0 = completely open
    APP_eReg_ReactionDelay,     //!< Delay time in timer ticks (1/100sec) until movement starts after applying power.
    APP_eReg_DurationOpen,      //!< Duration in timer ticks (1/100sec) to fully open the blind.
    APP_eReg_DurationClose,     //!< Duration in timer ticks (1/100sec) to fully close the blind.
    APP_eReg_WindowState,       //!< State of the window
    // insert application specific registers here
    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_ReactionDelay = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_DurationOpen,
    APP_eCfg_DurationOpenLow = APP_eCfg_DurationOpen,
    APP_eCfg_DurationOpenHigh,
    APP_eCfg_DurationClose,
    APP_eCfg_DurationCloseLow = APP_eCfg_DurationClose,
    APP_eCfg_DurationCloseHigh,

    // insert application specific configuration here
    APP_eCfg_Last
} eConfiguration_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
