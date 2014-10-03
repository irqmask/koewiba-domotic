/**
 * @addtogroup SYSSOCKET
 * @brief Socket interface.
 *
 * Socket interface for unix and tcp sockets.
 *
 * @{
 * @file    syssocket.h
 * @brief   Socket interface for unix and tcp sockets.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _SYSSOCKET_H_
#define _SYSSOCKET_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include "system.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

sys_fd_t sys_socket_open_server_unix (const char* socketname);

sys_fd_t sys_socket_open_client_unix (const char* socketname);

sys_fd_t sys_socket_open_server_tcp (const char* socketaddress, uint16_t port);

sys_fd_t sys_socket_open_client_tcp (const char* socketaddress, uint16_t port);

void sys_socket_close (sys_fd_t fd);

sys_fd_t sys_socket_accept (sys_fd_t server_fd);

size_t sys_socket_send (sys_fd_t fd, void* buffer, size_t buffersize);

size_t sys_socket_recv (sys_fd_t fd, void* buffer, size_t buffersize);

void sys_socket_flush (sys_fd_t fd);

size_t sys_socket_get_pending (sys_fd_t fd);

int sys_socket_set_blocking (sys_fd_t fd, bool blocking);

#endif /* _SYSSOCKET_H_ */
/** @} */
