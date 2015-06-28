/*
 * bus_gateway.h
 *
 *  Created on: 17.01.2015
 *      Author: mue
 */

#ifndef BUS_GATEWAY_H_
#define BUS_GATEWAY_H_

#include "bus.h"
#include "serialcomm.h"

void bgw_forward_serial_msg (sBus_t* bus, scomm_phy_t* phy);

#endif /* BUS_GATEWAY_H_ */
