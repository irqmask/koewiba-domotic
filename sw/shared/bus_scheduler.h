/**
 * @addtogroup BUS_SCHEDULER
 * @brief Public interface of bus-scheduler wrapper functions.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    bus_scheduler.h
 * @brief   Public interface of bus-scheduler wrapper functions.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _BUS_SCHEDULER_H_
#define _BUS_SCHEDULER_H_

// --- Include section ---------------------------------------------------------

#include "bus.h"
#include "clock.h"
#include "prjtypes.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif
#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

#define BUS_FIRSTNODE       0x01
#define BUS_LASTNODE        0x7F

// --- Type definitions --------------------------------------------------------


//! contains a bitfield (each bit represents one node)
typedef struct schednodes {
    uint8_t        uAddress[16]; // Bitfield for 128 Nodes
    uint8_t        uErrNode;     // Node in error-state
    uint8_t        uErrCnt;      // error-count
} sSchedNodes;

// following data is only used by bus scheduler
typedef struct schd {
    sSchedNodes     asNodeList;       //!< list of configured nodes
    uint8_t         uCurrentNode;                   //!< current processed node
    uint8_t         uDiscoverNode;                  //!< current discovery-address
    uint8_t         uErrorNode;                     //!< current node-address in error-state
    uint8_t         uErrorCount;                    //!< current error-count
    uint8_t         uSleepLoopCnt;                  //!< count till going to sleep.
    uint8_t         auTokenMsg[BUS_TOKEN_MSG_LEN];  //!< pre-compiled token message.
    BOOL            bSchedDiscovery;                //!< bus-discovery mode.
    BOOL            bSchedWaitingForAnswer;         //!< flag, if scheduler is waiting for an answer
    clock_timer_t   sNodeAnsTimeout;                //!< node answer timeout
} sSched_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void    bus_scheduler_initialize            (sBus_t* psBus,
                                             sSched_t* psSched,
                                             uint8_t uUart);

bool    bus_schedule_and_get_message        (sBus_t*        psBus,
                                             sSched_t*      psSched);

void    bus_schedule_check_and_set_sleep    (sBus_t* psBus);

#endif /* _BUS_SCHEDULER_H_ */
/** @} */
