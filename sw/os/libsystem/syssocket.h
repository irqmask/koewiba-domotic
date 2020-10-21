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

/**
 * Open an unix socket server.
 *
 * @param[in]   socketname      Path and name of unix socket file.
 *
 * @return File descriptor of socket server if successful, otherwise INVALID_FD.
 * @note call sys_err_get_as_string() to get error description.
 */
sys_fd_t sys_socket_open_server_unix(const char *socketname);

/**
 * Open a connection as client to a unix socket server.
 *
 * @param[in]   socketname      Path and name of unix socket file.
 *
 * @return File descriptor of socket connection if successful, otherwise INVALID_FD.
 * @note call sys_err_get_as_string() to get error description.
 */
sys_fd_t sys_socket_open_client_unix(const char *socketname);

/**
 * Open a tcp/ip socket server.
 *
 * @param[in]   port            Port number of socket server.
 *
 * @return File descriptor of socket server if successful, otherwise INVALID_FD.
 * @note call sys_err_get_as_string() to get error description.
 */
sys_fd_t sys_socket_open_server_tcp(uint16_t port);

/**
 * Open a connection as client to a tcp/ip socket server.
 *
 * @param[in]   socketaddress   IPv4 address of socket server.
 * @param[in]   port            Port number of socket server.
 *
 * @return File descriptor of socket connection if successful, otherwise INVALID_FD.
 * @note call sys_err_get_as_string() to get error description.
 */
sys_fd_t sys_socket_open_client_tcp(const char *socketaddress, uint16_t port);

/**
 * Close connection or server.
 *
 * @param[in]   fd              File description of connection or server.
 */
void sys_socket_close(sys_fd_t fd);

/**
 * Accept an incoming connection to the socket server.
 *
 * @param[in]   server_fd       File descriptor of socket server.
 * @param[out]  address         Remote address of accepted incoming connection.
 * @param[in]   address_len     Maximum buffer size of address
 * @param[out]  port            Remote port of accepted incoming connection.
 *
 * @return File descriptor of established connection, otherwise INVALID_FD.
 */
sys_fd_t sys_socket_accept(sys_fd_t server_fd, char *address, size_t address_len, uint16_t *port);

/**
 * Send data over the socket connection.
 *
 * @param[in]   fd          File descriptor of socket connection.
 * @param[in]   buffer      Buffer containing data to send.
 * @param[in]   buffersize  Number of bytes to be received.
 *
 * @return Number of sent bytes if successful, otherwise -1.
 * @note call sys_err_get_as_string() to get error description.
 */
ssize_t sys_socket_send(sys_fd_t fd, const void *buffer, size_t buffersize);

/**
 * Receive data over the socket connection.
 *
 * @param[in]   fd          File descriptor of socket connection.
 * @param[in]   buffer      Buffer to receive bytes to.
 * @param[in]   buffersize  Desired number of bytes to receive.
 *
 * @return Number of received bytes if successful, 0 if connection is closed
 *         remotely or -1 on error.
 * @note call sys_err_get_as_string() to get error description.
 */
ssize_t sys_socket_recv(sys_fd_t fd, void *buffer, size_t buffersize);

/**
 * Flush socket's RX and TX buffers.
 *
 * @param[in]   fd          File descriptor of socket connection.
 */
void sys_socket_flush(sys_fd_t fd);

/**
 * Get pending bytes in socket connection's send queue.
 *
 * @param[in]   fd          File descriptor of socket connection.
 *
 * @returns bytes to be sent in send-queue.
 */
size_t sys_socket_get_pending_sendq(sys_fd_t fd);

/**
 * Get pending bytes waiting in receive queue.
 *
 * @param[in]   fd          File descriptor of socket connection.
 *
 * @returns pending bytes in receive queue
 */
size_t sys_socket_get_pending_recvq(sys_fd_t fd);

/**
 * Set if calls to socket connection e.g. send and receive shall be blocking or
 * non-blocking.
 *
 * @param[in]   fd          File descriptor of socket connection.
 * @param[in]   blocking    true = calls shall be blocking.
 */
void sys_socket_set_blocking(sys_fd_t fd, bool blocking);

#ifdef __cplusplus
}
#endif

#endif /* _SYSSOCKET_H_ */
/** @} */
