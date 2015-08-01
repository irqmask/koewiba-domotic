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

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG   1
#define BUS_SCHEDULER   1	        //!< This program has scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
