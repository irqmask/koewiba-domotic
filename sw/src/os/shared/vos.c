/**
 * @addtogroup VBUSD_OR_SERIAL
 *
 * @{
 * @file    vos.c
 * @brief   "vbus or serial": Module to switch between a vbusd connection and a
 *          serial connection.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <assert.h>

#include "sysserial.h"
#include "syssocket.h"
#include "vos.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int vos_open_serial (vos_t* vos, const char* device, int baudrate)
{
    int rc = eERR_NONE;

    assert(vos != NULL);

    do {
        vos->fd = INVALID_FD;
        vos->interface_type = eVOS_IF_TYPE_SERIAL;

        vos->fd = sys_serial_open(device);
        if (vos->fd <= INVALID_FD) {
            rc = eSYS_ERR_INVALID_FD;
            break;
        }

        if ((rc = sys_serial_set_params(vos->fd,
                                        sys_serial_baudrate(baudrate),
                                        eSYS_SER_DB_8,
                                        eSYS_SER_P_NONE,
                                        eSYS_SER_SB_1)) != eSYS_ERR_NONE) {
            break;
        }

        sys_serial_set_blocking(vos->fd, false);
    } while (0);

    return rc;
}

int vos_open_vbusd (vos_t* vos, const char* address, uint16_t port)
{
    int rc = eERR_NONE;

    assert(vos != NULL);

    do {
        vos->interface_type = eVOS_IF_TYPE_VBUSD;

        if (port == 0) {
            vos->fd = sys_socket_open_client_unix(address);
        } else {
            vos->fd = sys_socket_open_client_tcp(address, port);
        }
        if (vos->fd <= INVALID_FD) {
            rc = eSYS_ERR_INVALID_FD;
            break;
        }

        sys_socket_set_blocking(vos->fd, false);
    } while (0);

    return rc;
}

void vos_close (vos_t* vos)
{
    assert(vos != NULL);

    switch (vos->interface_type) {
    case eVOS_IF_TYPE_SERIAL:
        sys_serial_close(vos->fd);
        break;
    case eVOS_IF_TYPE_VBUSD:
        sys_socket_close(vos->fd);
        break;
    default:
        break;
    }
}

size_t vos_send (vos_t* vos, void* buf, size_t bufsize)
{
    assert(vos != NULL);

    switch (vos->interface_type) {
    case eVOS_IF_TYPE_SERIAL:
        return sys_serial_send(vos->fd, buf, bufsize);
        break;
    case eVOS_IF_TYPE_VBUSD:
        return sys_socket_send(vos->fd, buf, bufsize);
        break;
    default:
        return 0;
        break;
    }
}

size_t vos_recv (vos_t* vos, void* buf, size_t bufsize)
{
    assert(vos != NULL);

    switch (vos->interface_type) {
    case eVOS_IF_TYPE_SERIAL:
        return sys_serial_recv(vos->fd, buf, bufsize);
        break;
    case eVOS_IF_TYPE_VBUSD:
        return sys_socket_recv(vos->fd, buf, bufsize);
        break;
    default:
        return 0;
        break;
    }
}

void vos_flush (vos_t* vos)
{
    assert(vos != NULL);

    switch (vos->interface_type) {
    case eVOS_IF_TYPE_SERIAL:
        sys_serial_flush(vos->fd);
        break;
    case eVOS_IF_TYPE_VBUSD:
        sys_socket_flush(vos->fd);
        break;
    default:
        break;
    }
}

size_t vos_get_pending (vos_t* vos)
{
    assert(vos != NULL);

    switch (vos->interface_type) {
    case eVOS_IF_TYPE_SERIAL:
        return sys_serial_get_pending(vos->fd);
        break;
    case eVOS_IF_TYPE_VBUSD:
        return sys_socket_get_pending(vos->fd);
        break;
    default:
        return 0;
        break;
    }
}

size_t vos_get_pending_send_bytes (vos_t* vos)
{
    assert(vos != NULL);

    switch (vos->interface_type) {
    case eVOS_IF_TYPE_SERIAL:
    case eVOS_IF_TYPE_VBUSD:
        break;
    default:
        return 0;
    }
}
}

/** @} */
