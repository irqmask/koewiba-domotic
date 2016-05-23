/**
 * @addtogroup SYSSOCKET
 *
 * @{
 * @file    syssocket.c
 * @brief   Platform dependent code to handle sockets.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <fcntl.h>
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

#include "syssocket.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

sys_fd_t sys_socket_open_server_unix (const char* socketname)
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

    rc = listen (fd, 32);
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

sys_fd_t sys_socket_open_client_unix (const char* socketname)
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
        perror("open unix client connect");
        close(fd);
        return rc;
    }

    return fd;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return INVALID_FD;
#endif
}

sys_fd_t sys_socket_open_server_tcp (const char* socketaddress, uint16_t port)
{
    //TODO implement
    return INVALID_FD;
}

sys_fd_t sys_socket_open_client_tcp (const char* socketaddress, uint16_t port)
{
    //TODO implement
    return INVALID_FD;
}

void sys_socket_close (sys_fd_t fd)
{
    #if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    close(fd);
#elif defined (PRJCONF_WINDOWS)
#endif
}

sys_fd_t sys_socket_accept (sys_fd_t server_fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    struct sockaddr_un  sockinfo;
    socklen_t           sockinfolen = sizeof(sockinfo);
    sys_fd_t            fd;

    fd = accept (server_fd, (struct sockaddr *)&sockinfo, &sockinfolen);
    return fd;
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    return INVALID_FD;
#endif
}

size_t sys_socket_recv (sys_fd_t fd, void* buffer, size_t buffersize)
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

size_t sys_socket_send (sys_fd_t fd, void* buffer, size_t buffersize)
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

void sys_socket_flush (sys_fd_t fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    tcflush(fd, TCIOFLUSH);
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
#endif
}

size_t sys_socket_get_pending_sendq (sys_fd_t fd)
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

size_t sys_socket_get_pending_recvq (sys_fd_t fd)
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

void sys_socket_set_blocking (sys_fd_t fd, bool blocking)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    int opts;

    do {
        opts = fcntl(fd ,F_GETFL);
        if (opts < 0) {
            perror("fcntl(F_GETFL)");
            break;
        }
        if (blocking) opts &= ~O_NONBLOCK;
        else opts = (opts | O_NONBLOCK);
        if (fcntl(fd, F_SETFL, opts) < 0) {
            perror("fcntl(F_SETFL)");
            break;
        }
    } while (0);
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
#endif
}

void sys_socket_get_name (sys_fd_t fd, char* address, size_t addr_len, uint16_t* port)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    union {
        struct sockaddr  common;
        struct sockaddr_un af_unix;
    } sockinfo;

    socklen_t len = sizeof(sockinfo);

    if (getsockname(fd, (struct sockaddr *)&sockinfo, &len) == -1)
        perror("getsockname");
    else {
        switch (sockinfo.common.sa_family) {
        case AF_UNIX:
            if (address != NULL) strcpy_s(address, addr_len, sockinfo.af_unix.sun_path);
            if (port != NULL) *port = 0;
            break;
        case AF_INET:
            break;
        default:
            break;
        }
    }
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
#endif
}

/** @} */
