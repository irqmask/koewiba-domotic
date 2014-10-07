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

#include "system.h"

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
}

sys_fd_t sys_socket_open_client_unix (const char* socketname)
{
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
    close(fd);
}

sys_fd_t sys_socket_accept (sys_fd_t server_fd)
{
    struct sockaddr_un  sockinfo;
    socklen_t           sockinfolen = sizeof(sockinfo);

    return accept (server_fd, (struct sockaddr *)&sockinfo, &sockinfolen);
}

int sys_socket_recv (sys_fd_t fd, void* buffer, size_t buffersize)
{
    return recv(fd, buffer, buffersize, 0);
}

int sys_socket_send (sys_fd_t fd, void* buffer, size_t buffersize)
{
    return send(fd, buffer, buffersize, 0);
}

void sys_socket_flush (sys_fd_t fd)
{
    tcflush(fd, TCIOFLUSH);
}

size_t sys_socket_get_pending (sys_fd_t fd)
{
    size_t pending_bytes = 0;

    ioctl(fd, FIONREAD, &pending_bytes);
    return pending_bytes;
}

void sys_socket_set_blocking (sys_fd_t fd, bool blocking)
{
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
}

/** @} */
