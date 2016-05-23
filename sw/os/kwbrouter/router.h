/**
 * @addtogroup ROUTER
 * @brief Route KWB messages between IPC sockets, TCP/IP sockets and the bus
 *        itself.
 *
 * TODO: Detailed description of module.
 *
 * @{
 * @file    router.h
 * @brief   Route KWB messages between IPC sockets, TCP/IP sockets and the bus
 *          itself.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _ROUTER_H_
#define _ROUTER_H_

// --- Include section ---------------------------------------------------------

#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum route_type {
    eROUTE_TYPE_UNKNOWN,
    eROUTE_TYPE_SERIAL,
    eROUTE_TYPE_SOCKET,
    eROUTE_TYPE_LAST
} route_type_t;

typedef struct route_entry route_entry_t;

typedef struct router {
    route_entry_t* first_route;
} router_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void route_init (router_t* router);

int route_add (router_t*    router,
               uint16_t     first_module_id,
               uint16_t     last_module_id,
               const char*  target_address,
               uint16_t     target_port,
               route_type_t type,
               void*        reference);

void route_delete (router_t*    router);

void route_message (router_t*    router,
                    msg_t*       message,
                    void*        reference);

#endif /* _ROUTER_H_ */
/** @} */
