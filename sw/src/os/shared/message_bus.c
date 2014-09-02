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

#include "ioloop.h"
#include "message_bus.h"
#include "sysserial.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

msg_bus_t* g_uarts[BUSPHY_MAX_DEVICES] = {
    NULL, NULL, NULL, NULL
};

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void msg_receive (void* arg)
{
    msg_bus_t*  msg_bus = (msg_bus_t*)arg;
    msg_t       message;
    
    if (bus_get_message(&msg_bus->bus)) {
        memset(&message, 0, sizeof(msg_t));
        
        if (bus_read_message(&msg_bus->bus, 
                             &message.sender,
                             message.data,
                             &message.length)) {
        
            if (msg_bus->incomming_handler != NULL) {
                msg_bus->incomming_handler(&message,
                                           msg_bus->incomming_arg);
            }
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

sys_fd_t msg_b_get_uart (uint8_t uart_index)
{
    if (uart_index < BUSPHY_MAX_DEVICES) {
        if (g_uarts[uart_index] != NULL) {
            return g_uarts[uart_index]->fd;
        }
    }
    return INVALID_FD;
}

void msg_b_init (msg_bus_t* msg_bus, uint8_t uart_index)
{
    assert(msg_bus != NULL);
    
    msg_bus->own_address = 0;
    msg_bus->fd = INVALID_FD;
    msg_bus->ioloop = NULL;
    msg_bus->incomming_handler = NULL;
    msg_bus->incomming_arg = NULL;
    
    memset(&msg_bus->bus, 0, sizeof(sBus_t));

    if (uart_index < BUSPHY_MAX_DEVICES) {
        g_uarts[uart_index] = msg_bus;
    }       
}

int msg_b_open (msg_bus_t*     msg_bus,
                ioloop_t*      ioloop,
                uint16_t       own_address,
                const char*    device)
{
    int             rc = eSYS_ERR_NONE;
    sys_fd_t        fd;
    
    do {
        assert(msg_bus != NULL);
        assert(ioloop != NULL);
        
        msg_bus->ioloop = ioloop;
        msg_bus->own_address = own_address;
        
        msg_bus->fd = sys_serial_open(device);
        if (fd <= INVALID_FD) {
            rc = eSYS_ERR_INVALID_FD;
            break;
        }
        
        if ((rc = sys_serial_set_params(fd, 
                                        eSYS_SER_BR_38400, 
                                        eSYS_SER_DB_8, 
                                        eSYS_SER_P_NONE, 
                                        eSYS_SER_SB_1)) != eSYS_ERR_NONE) {
            break;
        }
        
        if ((rc = sys_serial_set_blocking(fd, 0)) != eSYS_ERR_NONE) {
            break;
        }
        
        ioloop_register_fd(ioloop, fd, eIOLOOP_EV_READ, msg_receive, (void*)msg_bus);
        
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
