/**
 * @addtogroup KWBROUTER
 * @addtogroup ROUTER
 *
 * @{
 * @file    router.c
 * @brief   Route KWB messages between IPC sockets, TCP/IP sockets and the bus
 *          itself.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

#include "log.h"
#include "message.h"
#include "message_bus.h"
#include "message_socket.h"
#include "router.h"
#include "system.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef struct route_entry {
    uint16_t        first_module_id;
    uint16_t        last_module_id;
    char            target_address[256];
    uint16_t        target_port;
    route_type_t    type;
    void*           reference;
    route_entry_t*  next_entry;
} route_entry_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static route_entry_t* route_new (void)
{
    route_entry_t* route;

    route = (route_entry_t*)calloc(1, sizeof(route_entry_t));
    return route;
}

static void route_insert (router_t*         router,
                          route_entry_t*    route)
{
    route_entry_t *rt, *old_rt;

    rt = router->first_route;
    old_rt = rt;

    while (rt != NULL) {
        if (route->first_module_id < rt->first_module_id) break;
        if (route->first_module_id == rt->first_module_id &&
            route->last_module_id <= rt->last_module_id)  break;
        old_rt = rt;
        rt = rt->next_entry;
    }

    // is entry to be inserted at first?
    if (router->first_route == NULL) {
        router->first_route = route;
    } else if (old_rt == router->first_route &&
               ((route->first_module_id < rt->first_module_id) ||
                (route->first_module_id == rt->first_module_id &&
                 route->last_module_id < rt->last_module_id))) {
        // insert route before current first_route
        router->first_route = route;
        route->next_entry = old_rt;
    } else {
        // insert before rt
        route->next_entry = old_rt->next_entry;
        old_rt->next_entry = route;
    }
}

static void route_inspect (router_t* router)
{
    int             count = 0;
    route_entry_t*  rt;

    rt = router->first_route;

    log_msg(LOG_STATUS, " #  first  last  port address\n");
    while (rt != NULL) {
        log_msg(LOG_STATUS, "%3d %5d %5d %5d %s\n",
                            count, rt->first_module_id, rt->last_module_id, rt->target_port,
                            rt->target_address);
        count++;
        rt = rt->next_entry;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void route_init (router_t* router)
{
    assert(router != NULL);

    memset(router, 0, sizeof(router_t));
}

int route_add (router_t*    router,
               uint16_t     first_module_id,
               uint16_t     last_module_id,
               const char*  target_address,
               uint16_t     target_port,
               route_type_t type,
               void*        reference)
{
    int             rc = eERR_NOT_IMPLEMENTED;
    uint16_t        helper;
    route_entry_t*  route;

    assert(router != NULL);

    do {
        // do we need to swap module ids?
        if (first_module_id > last_module_id) {
            helper = first_module_id;
            first_module_id = last_module_id;
            last_module_id = helper;
        }

        if ((route = route_new()) == NULL) {
            rc = eERR_SYSTEM;
            break;
        }
        route->first_module_id = first_module_id;
        route->last_module_id = last_module_id;

        strcpy_s(route->target_address,
                 sizeof(route->target_address),
                 target_address);
        route->target_port = target_port;
        route->type = type;
        route->reference = reference;
        route_insert(router, route);
    } while (0);

    route_inspect(router);
    return rc;
}

void route_delete (router_t*    router)
{
    assert(router != NULL);

    ///@todo define deltetion criteria
    do {

    } while (0);
}

void route_message (router_t*    router,
                    msg_t*       message,
                    void*        reference)
{
    route_entry_t* route;

    assert(router != NULL);

    route = router->first_route;
    while (route != NULL) {
        // check if message receiver belongs to current route and if the
        // endpoint of this route is not the endpoint where this message
        // came from.
        if (message->receiver >= route->first_module_id &&
            message->receiver <= route->last_module_id &&
            reference != route->reference) {
            // route message
            switch (route->type) {
            case eROUTE_TYPE_SERIAL:
                msg_b_send((msg_bus_t*)route->reference, message);
                break;
            case eROUTE_TYPE_SOCKET:
                msg_s_send((msg_endpoint_t*)route->reference, message);
                break;
            }
        }
        route = route->next_entry;
    }
}

/** @}
 *  @} */
