/**
 * @addtogroup VBUSD_OR_SERIAL
 * @brief "vbus or serial": Module to switch between a vbusd connection and a
 *        serial connection.
 *
 * This module allows an application to switch between a vbusd connection and
 * a serial connection during startup. This is useful if a application shall
 * run in both ways. For example a monitoring application could be connected to
 * vbusd during developement of a bus application and to a serial connection
 * later in the field.
 *
 * @{
 * @file    vos.h
 * @brief   "vbus or serial": Module to switch between a vbusd connection and a
 *          serial connection.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _VOS_H_
#define _VOS_H_

// --- Include section ---------------------------------------------------------

#include "system.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum vos_if {
    eVOS_IF_TYPE_UNKNOWN,
    eVOS_IF_TYPE_SERIAL,
    eVOS_IF_TYPE_VBUSD,
    eVOS_IF_TYPE_LAST
} vos_if_t;

typedef struct vos {
    vos_if_t    interface_type;
    sys_fd_t    fd;
} vos_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int vos_open_serial (vos_t* vos, const char* device, int baudrate);

int vos_open_vbusd (vos_t* vos, const char* address, uint16_t port);

void vos_close (vos_t* vos);

int vos_set_params (vos_t* vos, int baudrate);

size_t vos_send (vos_t* vos, void* buf, size_t bufsize);

size_t vos_recv (vos_t* vos, void* buf, size_t bufsize);

void vos_flush (vos_t* vos);

size_t vos_get_pending (vos_t* vos);

size_t vos_get_pending_send_bytes (vos_t* vos);

#endif /* _VOS_H_ */
/** @} */
