/*
 * bus_gateway.h
 *
 *  Created on: 17.01.2015
 *      Author: mue
 */

#ifndef BUS_GATEWAY_H_
#define BUS_GATEWAY_H_

#include "bus.h"
#include "queue.h"

BOOL    busgateway_forward_serial_message   (queue_t    *psRecQ,
                                             sBus_t     *psBus  );

#endif /* BUS_GATEWAY_H_ */
