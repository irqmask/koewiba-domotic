/**
 * @addtogroup TODO_MODULE_NAME_TODO
 *
 * @{
 * @file    main.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include "comm.h"

// --- Definitions -------------------------------------------------------------


// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------



void main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[COMM_MAXMSGSIZE];

    // Initialize port pins
    UART_DDR |= (UART_DRIVER | UART_RECVSTOP);

    COMM_Initialize();

    while (1) {
        COMM_Communicate();
        if (COMM_RecvMessage(msg, &msglen)) {
            COMM_AcknowledgeMessage();
            //send copy of message back message
            COMM_SendMessage(msg, msglen);
        }
    }
}

/** @} */
