/**
 * @addtogroup QUEUE    
 * @brief Interrupt safe queue for syncronizing data.
 *
 * This module contains functions to initialize the queue, enqueue a byte or
 * dequeue a byte.
 *
 * @{
 * @file    queue.h
 * @brief   Functions for queuing data.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _QUEUE_H_
#define _QUEUE_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define QUEUE_HEADER_SIZE 3

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            QUE_vInit           (uint8_t*               puQueue,
                                     uint8_t                uSize);
                                     
uint8_t         QUE_uPut            (uint8_t*               puQueue, 
                                     uint8_t                uVal);
                                     
uint16_t        QUE_uGet            (uint8_t*               puQueue);

#endif /* _QUEUE_H_ */
/** @} */
