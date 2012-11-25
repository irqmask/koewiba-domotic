/**
 * @addtogroup BUS
 * @addtogroup BUS_SCHEDUL
 * @brief Message scheduler of bus essages. Used by the master on the bus.
 *
 * @{
 * @file    bus_schedul.c
 * @brief   Bus message scheduler.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "bus_intern.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Schedule nodes on bus.
 * 
 * @param[in] psBus
 * Handle of the bus.
 */
void BUS_vSchedule(sBus_t* psBus)
{
    psBus->uCurrentNode++;
    if (psBus->uCurrentNode >= BUS_MAXNODES) {
        psBus->uCurrentNode = 0;
    }
}

/** @} */
/** @} */
