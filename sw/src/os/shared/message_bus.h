/**
 * @addtogroup MESSAGE_BUS
 * @brief Transmit KoeWiBa messages over a serial connection.
 *
 * Bus messages are transmitted or received via serial connection (e.g. RS485 bus)
 *
 * @{
 * @file    message_bus.h
 * @brief   Transmit KoeWiBa messages over a serial connection.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _MESSAGE_BUS_H_
#define _MESSAGE_BUS_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#include "bus.h"
#include "bus_scheduler.h"
#include "ioloop.h"
#include "message.h"
#include "system.h"
#include "vos.h"

// --- Definitions -------------------------------------------------------------

#define BUSPHY_MAX_DEVICES 4

// --- Type definitions --------------------------------------------------------

typedef struct msg_bus {
    uint16_t            own_address;
    vos_t               vos;
    ioloop_t*           ioloop;
    msg_incom_func_t    incomming_handler;
    void*               incomming_arg;

    sBus_t              bus;
    sSched_t            scheduler;
} msg_bus_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

vos_t* msg_b_get_uart (uint8_t uart_index);

void msg_b_init (msg_bus_t* msg_bus, uint8_t uart_index);

int msg_b_open (msg_bus_t*     msg_bus,
                ioloop_t*      ioloop,
                uint16_t       own_address,
                bool           open_serial,
                const char*    device_or_address,
                int            baudrate_or_port);

void msg_b_set_incomming_handler (msg_bus_t* msg_bus, msg_incom_func_t func, void* arg);

void msg_b_send (msg_bus_t* msg_bus, msg_t* message);

#endif /* _MESSAGE_BUS_H_ */
/** @} */
