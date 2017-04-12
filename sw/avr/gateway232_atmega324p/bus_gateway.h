/**
 * @addtogroup BUS_GATEWAY
 * @brief Gateway to transmit messages from bus to serial line and vice versa.
 *
 * @{
 * @file    bus_gateway.c
 * @brief   Gateway to transmit messages from bus to serial line and vice versa.

 * @author  Robert Mueller, Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef BUS_GATEWAY_H_
#define BUS_GATEWAY_H_

// --- Include section ---------------------------------------------------------

#include "bus.h"
#include "serialcomm.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void bgw_forward_serial_msg (sBus_t* bus, scomm_phy_t* phy);

bool bgw_forward_bus_msg (sBus_t *bus, scomm_phy_t *serial);

#endif /* BUS_GATEWAY_H_ */
