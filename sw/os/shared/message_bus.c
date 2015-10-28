/**
 * @addtogroup MESSAGE_BUS
 *
 * @{
 * @file    message_bus.c
 * @brief   Transmit KoeWiBa messages over a serial connection.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "ioloop.h"
#include "message_bus.h"
#include "vos.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

msg_bus_t* g_uarts[BUSPHY_MAX_DEVICES] = {
    NULL, NULL, NULL, NULL
};

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static int32_t msg_receive (void* arg)
{
    msg_bus_t*  msg_bus = (msg_bus_t*)arg;
    msg_t       message;

    if (bus_get_message(&msg_bus->bus)) {
        memset(&message, 0, sizeof(msg_t));

        if (bus_read_message_verbose(&msg_bus->bus,
                                     &message.sender,
                                     &message.receiver,
                                     &message.length,
                                     message.data,
                                     &message.crc)) {

            if (msg_bus->incomming_handler != NULL) {
                msg_bus->incomming_handler(&message, &msg_bus,
                                           msg_bus->incomming_arg);
            }
        }
    }
    return clk_timers_next_expiration();
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

vos_t* msg_b_get_uart (uint8_t uart_index)
{
    if (uart_index < BUSPHY_MAX_DEVICES) {
        if (g_uarts[uart_index] != NULL) {
            return &g_uarts[uart_index]->vos;
        }
    }
    return NULL;
}

void msg_b_init (msg_bus_t* msg_bus, uint8_t uart_index)
{
    assert(msg_bus != NULL);

    memset(msg_bus, 0, sizeof(msg_bus_t));

    if (uart_index < BUSPHY_MAX_DEVICES) {
        g_uarts[uart_index] = msg_bus;
    }
}

int msg_b_open (msg_bus_t*     msg_bus,
                ioloop_t*      ioloop,
                uint16_t       own_address,
                bool           open_serial,
                const char*    device_or_address,
                int            baudrate_or_port)
{
    int             rc = eSYS_ERR_NONE;

    do {
        assert(msg_bus != NULL);
        assert(ioloop != NULL);

        msg_bus->ioloop = ioloop;
        msg_bus->own_address = own_address;

        if (open_serial) {
            rc = vos_open_serial(&msg_bus->vos, device_or_address, baudrate_or_port);
        } else {
            rc = vos_open_vbusd(&msg_bus->vos, device_or_address, (uint16_t)baudrate_or_port);
        }
        if (rc != eERR_NONE) {
            break;
        }

        ioloop_register_fd(ioloop, msg_bus->vos.fd, eIOLOOP_EV_READ, msg_receive, (void*)msg_bus);
        ioloop_register_timer(ioloop, 10, true, eIOLOOP_EV_TIMER, msg_receive, (void*)msg_bus);

        clk_initialize();
        bus_configure(&msg_bus->bus, own_address);
        bus_initialize(&msg_bus->bus, 0);

        bus_flush_bus(&msg_bus->bus);
    } while (0);
    return rc;
}

void msg_b_set_incomming_handler (msg_bus_t* msg_bus, msg_incom_func_t func, void* arg)
{
    assert(msg_bus != NULL);

    msg_bus->incomming_handler = func;
    msg_bus->incomming_arg = arg;
}

void msg_b_send (msg_bus_t* msg_bus, msg_t* message)
{
    assert(msg_bus != NULL);
    assert(message != NULL);

    bus_send_message(&msg_bus->bus,
                     message->receiver,
                     message->length,
                     message->data);
}

/** @} */
