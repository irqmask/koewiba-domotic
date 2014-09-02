/**
 * @addtogroup MESSAGE
 * @brief Common datatypes for KoeWiBa messages.
 *
 * @{
 * @file    message.h
 * @brief   Common datatypes for KoeWiBa messages.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#include "bus.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct msg {
    uint16_t    sender;
    uint16_t    receiver;
    uint8_t     length;
    uint8_t     data[BUS_MAXBIGMSGLEN];
    uint16_t    crc;
} msg_t;

typedef void (*msg_incom_func_t)(msg_t* message, void* arg);

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _MESSAGE_H_ */
/** @} */
