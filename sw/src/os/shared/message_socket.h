/**
 * @addtogroup MESSAGE_SOCKET
 * @brief Transmit bus-messages between KWB applications via unix or inet
 * sockets.
 *
 * Bus messages are transmitted or received via unix or inet sockets between
 * applications or over the network.
 *
 * @{
 * @file    message_socket.h
 * @brief   Transmit bus-messages between KWB applications.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _MESSAGE_SOCKET_H_
#define _MESSAGE_SOCKET_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum msg_ep_type {
    eMSG_EP_SERVER = 0,
    eMSG_EP_COMM,
} msg_ep_type_t;

typedef struct msg_endpoint msg_endpoint_t;

typedef void (*msg_newconn_func_t)(char* address, uint16_t port, void* reference, void* arg);

typedef struct msg_socket {
    sys_fd_t            well_known_fd;
    msg_endpoint_t*     first_ep;
    ioloop_t*           ioloop;
    char                address[256];
    msg_newconn_func_t  new_connection_handler;
    void*               new_connection_arg;
    msg_incom_func_t    incomming_handler;
    void*               incomming_arg;
} msg_socket_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_s_init (msg_socket_t* msg_router);

int msg_s_open_server (msg_socket_t*   msg_socket,
                       ioloop_t*       ioloop,
                       const char*     address,
                       uint16_t        port);

int msg_s_open_client (msg_socket_t*   msg_socket,
                       ioloop_t*       ioloop,
                       const char*     address,
                       uint16_t        port);

void msg_s_close_connection (msg_socket_t* msg_socket, msg_endpoint_t* ep);

void msg_s_set_newconnection_handler (msg_socket_t* msg_socket, msg_newconn_func_t func, void* arg);

void msg_s_set_incomming_handler (msg_socket_t* msg_socket, msg_incom_func_t func, void* arg);

msg_endpoint_t* msg_s_get_endpoint (msg_socket_t*   msg_socket,
                                    int             index,
                                    uint32_t        flags);

void msg_s_send (msg_endpoint_t*    recv_ep,
                 msg_t*             message);

#endif /* _MESSAGE_SOCKET_H_ */
/** @} */
