/**
 * @addtogroup SYSSOCKET
 *
 * @{
 * @file    syssocket.c
 * @brief   Platform dependent code to handle sockets.
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

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <arpa/inet.h>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <safe_lib.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <termios.h>
    #include <unistd.h>
#elif defined (PRJCONF_WINDOWS)
    #include <windows.h>
    #include <winsock.h>
#endif

#include <string.h>

#include "syssocket.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Socket information of different socket classes.
 */
typedef union {
    //! Protocol common socket information
    struct sockaddr  common;
    //! Socket information of UNIX sockets
    struct sockaddr_un af_unix;
    //! Socket information of TCP/IP sockets
    struct sockaddr_in af_inet;
} sockinfo_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void sys_socket_get_address(sys_fd_t fd, sockinfo_t *sockinfo, char *address, size_t addr_len, uint16_t *port)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    struct sockaddr_storage ss;
    socklen_t sslen = sizeof(struct sockaddr_storage);

    switch (sockinfo->common.sa_family) {
    case AF_UNIX:
        if (getsockname(fd, (struct sockaddr *)&ss, &sslen) == 0) {
            struct sockaddr_un *un = (struct sockaddr_un *)&ss;
            if (address != NULL) {
                strcpy_s(address, addr_len, un->sun_path);
            }
        }
        if (port != NULL) {
            *port = 0;
        }
        break;

    case AF_INET:
        inet_ntop(AF_INET, &sockinfo->af_inet.sin_addr, address, addr_len);
        if (port != NULL) {
            *port = ntohs(sockinfo->af_inet.sin_port);
        }
        break;

    default:
        break;
    }

#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
#endif
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

sys_fd_t sys_socket_open_server_unix(const char *socketname)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    int                 rc;
    sys_fd_t            fd;
    struct sockaddr_un  sockinfo;
    size_t              sockinfolen;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("open unix server socket");
        return fd;
    }

    sockinfo.sun_family = AF_UNIX;
    strcpy_s(sockinfo.sun_path, sizeof(sockinfo.sun_path), socketname);
    sockinfolen = sizeof(sockinfo.sun_family) + strlen(sockinfo.sun_path) + 1;

    if (unlink(sockinfo.sun_path) < 0) {
        perror("open unix server unlink");
    }

    rc = bind(fd, (struct sockaddr *)&sockinfo, sockinfolen);
    if (rc < 0) {
        perror("open unix server bind");
        close(fd);
        return rc;
    }

    rc = listen(fd, 32);
    if (rc < 0) {
        perror("open unix server listen");
        close(fd);
        return rc;
    }

    return fd;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return INVALID_FD;
#endif
}

//----------------------------------------------------------------------------
sys_fd_t sys_socket_open_client_unix(const char *socketname)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    int                 rc;
    sys_fd_t            fd;
    struct sockaddr_un  sockinfo;
    size_t              sockinfolen;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("open unix client socket");
        return fd;
    }

    sockinfo.sun_family = AF_UNIX;
    strcpy_s(sockinfo.sun_path, sizeof(sockinfo.sun_path), socketname);
    sockinfolen = sizeof(sockinfo.sun_family) + strlen(sockinfo.sun_path) + 1;

    rc = connect(fd, (struct sockaddr *) &sockinfo, sockinfolen);
    if (rc != 0) {
        perror("connect to unix client");
        close(fd);
        return rc;
    }

    return fd;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return INVALID_FD;
#endif
}

//----------------------------------------------------------------------------
sys_fd_t sys_socket_open_server_tcp(uint16_t port)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    int                 rc;
    sys_fd_t            fd;
    struct sockaddr_in  sockinfo;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("open tcp client socket");
        return fd;
    }

    // allow port to be reused. Especially for tests where the server is opened / closed in a short interval.
    int so_reuseport = 1;
    rc = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &so_reuseport, sizeof(so_reuseport));
    if (rc < 0) {
        perror("setsockopt SO_REUSEPORT");
        close(fd);
        return rc;
    }

    memset(&sockinfo, 0, sizeof(sockinfo));
    sockinfo.sin_family = AF_INET;
    sockinfo.sin_addr.s_addr = INADDR_ANY;
    sockinfo.sin_port = htons(port);

    rc = bind(fd, (struct sockaddr *)&sockinfo, sizeof(sockinfo));
    if (rc < 0) {
        perror("bind tcp server");
        close(fd);
        return rc;
    }

    rc = listen(fd, 3);
    if (rc < 0) {
        perror("listen tcp server");
        close(fd);
        return rc;
    }

    return fd;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return INVALID_FD;
#endif
}

//----------------------------------------------------------------------------
sys_fd_t sys_socket_open_client_tcp(const char *socketaddress, uint16_t port)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    int                 rc;
    sys_fd_t            fd;
    struct sockaddr_in  sockinfo;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("open tcp client socket");
        return fd;
    }

    memset(&sockinfo, 0, sizeof(sockinfo));
    sockinfo.sin_family = AF_INET;
    if (inet_pton(AF_INET, socketaddress, &sockinfo.sin_addr.s_addr) < 0) {
        rc = errno;
        perror("unable to convert address to IPv4 address!");
        close(fd);
        return rc;
    }
    sockinfo.sin_port = htons(port);

    rc = connect(fd, (struct sockaddr *) &sockinfo, sizeof(sockinfo));
    if (rc < 0) {
        perror("unable to connect to tcp client");
        close(fd);
        return rc;
    }

    return fd;
#else
    //TODO implement
    return INVALID_FD;
#endif
}

//----------------------------------------------------------------------------
void sys_socket_close(sys_fd_t fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    close(fd);
#elif defined (PRJCONF_WINDOWS)
#endif
}

//----------------------------------------------------------------------------
sys_fd_t sys_socket_accept(sys_fd_t server_fd, char *address, size_t address_len, uint16_t *port)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    sys_fd_t    fd;
    sockinfo_t  sockinfo;
    socklen_t   sockinfolen = sizeof(sockinfo_t);

    fd = accept(server_fd, (struct sockaddr *)&sockinfo, &sockinfolen);
    if (fd >= 0)
        sys_socket_get_address(fd, &sockinfo, address, address_len, port);
    return fd;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return INVALID_FD;
#endif
}

//----------------------------------------------------------------------------
ssize_t sys_socket_recv(sys_fd_t fd, void *buffer, size_t buffersize)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    return recv(fd, buffer, buffersize, 0);
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return 0;
#endif
}

//----------------------------------------------------------------------------
ssize_t sys_socket_send(sys_fd_t fd, const void *buffer, size_t buffersize)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    return send(fd, buffer, buffersize, 0);
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return 0;
#endif
}

//----------------------------------------------------------------------------
void sys_socket_flush(sys_fd_t fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    tcflush(fd, TCIOFLUSH);
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
#endif
}

//----------------------------------------------------------------------------
size_t sys_socket_get_pending_sendq(sys_fd_t fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    size_t pending_bytes = 0;

    ioctl(fd, TIOCOUTQ, &pending_bytes);
    return pending_bytes;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return 0;
#endif
}

//----------------------------------------------------------------------------
size_t sys_socket_get_pending_recvq(sys_fd_t fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    size_t pending_bytes = 0;

    ioctl(fd, FIONREAD, &pending_bytes);
    return pending_bytes;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return 0;
#endif
}

//----------------------------------------------------------------------------
void sys_socket_set_blocking(sys_fd_t fd, bool blocking)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    int opts;

    do {
        opts = fcntl(fd, F_GETFL);
        if (opts < 0) {
            perror("fcntl(F_GETFL)");
            break;
        }
        if (blocking) {
            opts &= ~O_NONBLOCK;
        }
        else {
            opts = (opts | O_NONBLOCK);
        }
        if (fcntl(fd, F_SETFL, opts) < 0) {
            perror("fcntl(F_SETFL)");
            break;
        }
    } while (0);
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
#endif
}

/** @} */
