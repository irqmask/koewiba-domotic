/**
 * @addtogroup SYSSERIAL
 *
 * @{
 * @file    sysserial.c
 * @brief   Blocking send and receive functions for serial communication.
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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    #include <fcntl.h>
    #include <safe_lib.h>
    #include <sys/ioctl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <termios.h>
    #include <unistd.h>
#elif defined (PRJCONF_WINDOWS)
    #include <windows.h>
#endif

#include "sysserial.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
static const int c_baudrate[eSYS_SER_BR_LAST] = {
    B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600,
    -1, B19200, -1, B38400, -1, B57600, B115200, -1, B230400, -1, B460800, B500000,
    B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000,
    B3500000, B4000000
};
#elif defined (PRJCONF_WINDOWS)
static const int c_baudrate[eSYS_SER_BR_LAST] = {
    -1, -1, CBR_110, -1, -1, -1, CBR_300, CBR_600, CBR_1200, -1, CBR_2400, CBR_4800, CBR_9600,
        CBR_14400, CBR_19200, -1, CBR_38400, -1, CBR_57600, CBR_115200, CBR_128000, -1, CBR_256000, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
#endif

static const int c_baudConsts2baudrate[eSYS_SER_BR_LAST] = {
    50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 14400,
    19200, 28800, 38400, 56000, 57600, 115200, 128000, 230400, 256000, 460800,
    500000, 576000, 921600, 1000000, 1152000, 1500000, 2000000, 2500000, 3000000,
    3500000, 4000000,
};

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
static const int c_databits[eSYS_SER_DB_LAST] = {
    CS5,            // eSYS_SER_DB_5
    CS6,            // eSYS_SER_DB_6
    CS7,            // eSYS_SER_DB_7
    CS8             // eSYS_SER_DB_8
};
#elif defined (PRJCONF_WINDOWS)
static const int c_databits[eSYS_SER_DB_LAST] = {
    5,              // eSYS_SER_DB_5
    6,              // eSYS_SER_DB_6
    7,              // eSYS_SER_DB_7
    8               // eSYS_SER_DB_8
};
#endif

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
static const int c_parity[eSYS_SER_P_LAST] = {
    0,              // eSYS_SER_P_NONE
    PARENB | PARODD,// eSYS_SER_P_ODD
    PARENB,         // eSYS_SER_P_EVEN
    -1,             // eSYS_SER_P_MARK
};
#elif defined (PRJCONF_WINDOWS)
static const int c_parity[eSYS_SER_P_LAST] = {
    NOPARITY,       // eSYS_SER_P_NONE
    ODDPARITY,      // eSYS_SER_P_ODD
    EVENPARITY,     // eSYS_SER_P_EVEN
    MARKPARITY      // eSYS_SER_P_MARK
};
#endif

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
static const int c_stopbits[eSYS_SER_SB_LAST] = {
    0,              // eSYS_SER_SB_1
    -1,             // eSYS_SER_SB_1p5
    CSTOPB          // eSYS_SER_SB_2
};
#elif defined (PRJCONF_WINDOWS)
static const int c_stopbits[eSYS_SER_SB_LAST] = {
    ONESTOPBIT,     // eSYS_SER_SB_1
    ONE5STOPBITS,   // eSYS_SER_SB_1p5
    TWOSTOPBITS     // eSYS_SER_SB_2
};
#endif

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

sys_fd_t sys_serial_open(const char *device)
{
    sys_fd_t fd = INVALID_FD;
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)

    struct termios settings;

    assert(device != NULL);

    do {
        // open for reading and writing, without interruptions from tty devices,
        // without delay (check if other side is properly connected)
        fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd <= INVALID_FD) {
            fd = INVALID_FD;
            perror("Unable to open serial device");
            break;
        }
    } while (0);
#elif defined (PRJCONF_WINDOWS)
    // exclusive access, default security attributes, non-overlapped IO
    fd = CreateFile(device,
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    0,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    0);
#endif
    return fd;
}

//----------------------------------------------------------------------------
void sys_serial_close(sys_fd_t fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    close(fd);
#elif defined (PRJCONF_WINDOWS)
    CloseHandle(fd);
#endif
}

//----------------------------------------------------------------------------
int sys_serial_set_params(sys_fd_t            fd,
                          sys_ser_baudrate_t  baudrate,
                          sys_ser_databits_t  databits,
                          sys_ser_parity_t    parity,
                          sys_ser_stopbits_t  stopbits,
                          sys_ser_flowctrl_t  flowcontrol)
{
    int             rc = eSYS_ERR_NONE;
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    int             sys_baudrate;
    int             sys_databits;
    int             sys_parity;
    int             sys_stopbits;
    int             sys_flowcontrol;
    struct termios  settings;

    do {
        if (baudrate >= eSYS_SER_BR_LAST ||
            databits >= eSYS_SER_DB_LAST ||
            parity >= eSYS_SER_P_LAST ||
            stopbits >= eSYS_SER_SB_LAST ||
            flowcontrol >= eSYS_SER_FC_LAST) {
            rc = eERR_BAD_PARAMETER;
            break;
        }

        if (tcgetattr(fd, &settings) < 0) {
            perror("Unable to get device settings");
            rc = eSYS_ERR_SER_CONFIGURE;
            break;
        }

        settings.c_cc[VMIN]  = 1;     // block until n bytes are received
        settings.c_cc[VTIME] = 0;     // no timeout
        if (tcsetattr(fd, TCSANOW, &settings) < 0) {
            perror("Unable to set device settings");
            rc = eSYS_ERR_SER_CONFIGURE;
            break;
        }

        sys_baudrate = c_baudrate[baudrate];
        sys_databits = c_databits[databits];
        sys_parity = c_parity[parity];
        sys_stopbits = c_stopbits[stopbits];

        // convert flow control parameter to system parameter
        switch (flowcontrol) {
        case eSYS_SER_FC_HW:
            sys_flowcontrol = CRTSCTS;
            break;
        default:
            sys_flowcontrol = 0;
            break;
        }

        if (sys_baudrate < 0 || sys_databits < 0 || sys_parity < 0 || sys_stopbits < 0) {
            rc = eSYS_ERR_SER_UNSUPPORTED;
            break;
        }

        memset(&settings, 0, sizeof(settings));
        tcgetattr(fd, &settings);
        settings.c_cflag = sys_databits | sys_parity | sys_stopbits | sys_flowcontrol | CLOCAL | CREAD;
        settings.c_iflag = IGNPAR;
        settings.c_oflag = 0;
        settings.c_lflag = 0;
        settings.c_cc[VMIN]  = 1;     // block until n bytes are received
        settings.c_cc[VTIME] = 0;     // no timeout

        if ((cfsetispeed(&settings, sys_baudrate) < 0) ||
            (cfsetospeed(&settings, sys_baudrate) < 0) ||
            (tcsetattr(fd, TCSANOW, &settings)  < 0)) {
            rc = eSYS_ERR_SER_CONFIGURE;
        }
    } while (0);
#elif defined (PRJCONF_WINDOWS)
    DCB dcb;

    do {
        if (baudrate >= eSYS_SER_BR_LAST ||
            databits >= eSYS_SER_DB_LAST ||
            parity >= eSYS_SER_P_LAST ||
            stopbits >= eSYS_SER_SB_LAST ||
            flowcontrol >= eSYS_SER_FC_LAST) {
            eERR_BAD_PARAMETER;
            break;
        }
        // Initialize the DCB structure.
        SecureZeroMemory(&dcb, sizeof(DCB));
        dcb.DCBlength = sizeof(DCB);

        // Build on the current configuration by first retrieving all current
        // settings.
        if (!GetCommState(fd, &dcb)) {
            rc = eSYS_ERR_SER_CONFIGURE;
            break;
        }

        dcb.BaudRate = c_baudrate[baudrate];
        dcb.ByteSize = c_databits[databits];
        dcb.Parity = c_parity[parity];
        dcb.StopBits = c_stopbits[stopbits];

        // flow-control is currently only available for transmitting data.
        dcb.fOutxDsrFlow = FALSE;
        dcb.fInX = FALSE;

        switch (flowcontrol) {
        case eSYS_SER_FC_HW:
            dcb.fOutxCtsFlow = TRUE;
            dcb.fOutX = FALSE;
            break;
        case eSYS_SER_FC_XONXOFF:
            dcb.fOutxCtsFlow = FALSE;
            dcb.fOutX = TRUE;
            break;
        case eSYS_SER_FC_NONE:
            dcb.fOutxCtsFlow = FALSE;
            dcb.fOutX = FALSE;
            break;
        default:
            break;
        }

        if (!SetCommState(fd, &dcb)) {
            rc = eSYS_ERR_SER_CONFIGURE;
            break;
        }
    } while (0);
#endif
    return rc;
}

//----------------------------------------------------------------------------
ssize_t sys_serial_send(sys_fd_t fd, const void *buf, size_t bufsize)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    return write(fd, buf, bufsize);
#elif defined (PRJCONF_WINDOWS)
    //TODO implement windows version
    DWORD written = 0;
    WriteFile(fd, buf, bufsize, &written, FALSE);
    return (size_t)written;
#else
    return 0;
#endif
}

//----------------------------------------------------------------------------
ssize_t sys_serial_recv(sys_fd_t fd, void *buf, size_t bufsize)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    return read(fd, buf, bufsize);
#elif defined (PRJCONF_WINDOWS)
    DWORD bytesread = 0;
    ReadFile(fd, buf, bufsize, &bytesread, NULL);
    return bytesread;
#else
    return 0;
#endif
}

//----------------------------------------------------------------------------
void sys_serial_flush(sys_fd_t fd)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    tcflush(fd, TCIOFLUSH);
#elif defined (PRJCONF_WINDOWS)
    PurgeComm(fd, PURGE_RXCLEAR | PURGE_TXCLEAR);
#endif
}

//----------------------------------------------------------------------------
size_t sys_serial_get_pending_sendq(sys_fd_t fd)
{
    size_t pending_bytes = 0;

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    ioctl(fd, TIOCOUTQ, &pending_bytes);
#elif defined (PRJCONF_WINDOWS)
    COMSTAT stat = {0};
    // TODO replace ClearCommError to avoid clearing error here.
    ClearCommError(fd, NULL, &stat);
    pending_bytes = stat.cbOutQue;
#endif
    return pending_bytes;
}

//----------------------------------------------------------------------------
size_t sys_serial_get_pending_recvq(sys_fd_t fd)
{
    size_t pending_bytes = 0;

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    ioctl(fd, FIONREAD, &pending_bytes);
#elif defined (PRJCONF_WINDOWS)
    COMSTAT stat = {0};
    // TODO replace ClearCommError to avoid clearing error here.
    ClearCommError(fd, NULL, &stat);
    pending_bytes = stat.cbInQue;
#endif
    return pending_bytes;
}

//----------------------------------------------------------------------------
void sys_serial_set_blocking(sys_fd_t fd, bool blocking)
{
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
    if (blocking) {
        fcntl(fd, F_SETFL, 0);
    }
    else {
        fcntl(fd, F_SETFL, FNDELAY);
    }
#elif defined (PRJCONF_WINDOWS)
    // using non-overlapping I/O under Windows per default
#endif
}

//----------------------------------------------------------------------------
sys_ser_baudrate_t sys_serial_baudrate(uint32_t baudrate)
{
    sys_ser_baudrate_t br = -1;
    int ii;

    for (ii = 0; ii < eSYS_SER_BR_LAST; ii++) {
        if (baudrate == c_baudConsts2baudrate[ii]) {
            br = ii;
            break;
        }
    }
    return br;
}

/** @} */
