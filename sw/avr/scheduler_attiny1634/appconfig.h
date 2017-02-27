/**
 * @addtogroup TESTSCHEDULER_APPCONFIG
 * @brief Configuration of modules used in "testscheduler" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "testscheduler" project.
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
#define BUS_SCHEDULER   1	        //!< This program has scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

// --- Type definitions --------------------------------------------------------

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_TODO00 = MOD_eReg_FirstAppSpecific, //!< Register mapping: remote address 0 (16bit)

    // insert application specific registers here
    APP_eReg_Last
} eRegisters_t;

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_TODO00 = MOD_eCfg_FirstAppSpecific,
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
