/**
 * @addtogroup BUS
 * @addtogroup BUS_INTERN
 * @brief Internal definitions for bus modules.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    bus_intern.h
 * @brief   Internal definitions for bus modules.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _BUS_INTERN_H_
#define _BUS_INTERN_H_

// --- Include section ---------------------------------------------------------

#include "bus.h"

// --- Definitions -------------------------------------------------------------

#define BUS_BITTIME            10 //TODO
#define BUS_INITWAIT_TIME  (24 * COMM_BITTIME)
#define BUS_RECOVERY       (12 * COMM_BITTIME)
#define BUS_INTERBYTE_TIMEOUT  (8 * COMM_BITTIME)

// --- Type definitions --------------------------------------------------------

typedef struct busphy {
    uint8_t         uUart;
    uint8_t         uCurrentBytesToSend;
    const uint8_t*  puSendPtr;
} sBusPhy_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void BUS__vPhyInitialize        (sBusPhy_t*     psPhy,
                                 uint8_t        uUart);

void BUS__vPhyActivateSender    (sBusPhy_t*     psPhy,
                                 BOOL           bActivate);

void BUS__vPhyActivateReceiver  (sBusPhy_t*     psPhy,
                                 BOOL           bActivate);

BOOL BUS__bPhySend              (sBusPhy_t*     psPhy, 
                                 const uint8_t* puMsg, 
                                 uint8_t        uLen);

#endif /* _BUS_INTERN_H_ */
/** @} */
/** @} */
