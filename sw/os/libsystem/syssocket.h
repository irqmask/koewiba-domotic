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
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _SYSSOCKET_H_
#define _SYSSOCKET_H_

// --- Include section ---------------------------------------------------------

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

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

sys_fd_t sys_socket_open_server_tcp (uint16_t port);

sys_fd_t sys_socket_open_client_tcp (const char* socketaddress, uint16_t port);

void sys_socket_close (sys_fd_t fd);

sys_fd_t sys_socket_accept (sys_fd_t server_fd, char* address, size_t address_len, uint16_t* port);

ssize_t sys_socket_send (sys_fd_t fd, void* buffer, size_t buffersize);

ssize_t sys_socket_recv (sys_fd_t fd, void* buffer, size_t buffersize);

void sys_socket_flush (sys_fd_t fd);

size_t sys_socket_get_pending_sendq (sys_fd_t fd);

size_t sys_socket_get_pending_recvq (sys_fd_t fd);

void sys_socket_set_blocking (sys_fd_t fd, bool blocking);

#ifdef __cplusplus
}
#endif

#endif /* _SYSSOCKET_H_ */
/** @} */
