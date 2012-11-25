/**
 * @addtogroup TEST_PROTOCOL
 *
 * @{
 * @file    main_testprotocol.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "bus.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t g_sBus;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGSIZE];

    BUS_vConfigure(&g_sBus, 2); // configure a bus node with address 2
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0

    while (1) {
        // check for message and read it
        if (BUS_bGetMessage(&g_sBus)) {
            BUS_bReadMessage(&g_sBus, &msglen, &msglen, msg);
        }
    }
    return 0;
}

/** @} */
