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

#if (BUS_MAXSENDMSGLEN > BUS_MAXRECVMSGLEN)
 #define MAX_MSG_SIZE   BUS_MAXSENDMSGLEN
#else
 #define MAX_MSG_SIZE   BUS_MAXRECVMSGLEN
#endif

// --- Type definitions --------------------------------------------------------

typedef enum msg_error {
    eMSG_ERR_SIZE = 500,
    eMSG_ERR_BUSY,          // send or receive buffer full
} msg_error_t;

typedef struct msg {
    uint16_t    sender;
    uint16_t    receiver;
    uint8_t     length;
    uint8_t     data[MAX_MSG_SIZE];
    uint16_t    crc;
} msg_t;

typedef void (*msg_incom_func_t)(msg_t* message, void* reference, void* arg);

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_log (msg_t message);

#endif /* _MESSAGE_H_ */
/** @} */
