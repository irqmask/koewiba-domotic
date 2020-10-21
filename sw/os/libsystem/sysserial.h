/**
 * @addtogroup SYSSERIAL
 * @brief Blocking send and receive functions for serial communication.
 *
 * This module offers system independent functions to send and receive data
 * over serial devices e.g. RS232 or RS485.
 *
 * @{
 * @file    sysserial.h
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
#ifndef _SYSSERIAL_H_
#define _SYSSERIAL_H_

// --- Include section ---------------------------------------------------------

#include <stdio.h>

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! predefined baud rate constants. Some may not be supported by different platforms.
typedef enum sys_ser_baudrate {
    eSYS_SER_BR_50 = 0,
    eSYS_SER_BR_75,
    eSYS_SER_BR_110,
    eSYS_SER_BR_134,
    eSYS_SER_BR_150,
    eSYS_SER_BR_200,
    eSYS_SER_BR_300,
    eSYS_SER_BR_600,
    eSYS_SER_BR_1200,
    eSYS_SER_BR_1800,
    eSYS_SER_BR_2400,
    eSYS_SER_BR_4800,
    eSYS_SER_BR_9600,
    eSYS_SER_BR_14400,
    eSYS_SER_BR_19200,
    eSYS_SER_BR_28800,
    eSYS_SER_BR_38400,
    eSYS_SER_BR_56000,
    eSYS_SER_BR_57600,
    eSYS_SER_BR_115200,
    eSYS_SER_BR_128000,
    eSYS_SER_BR_230400,
    eSYS_SER_BR_256000,
    eSYS_SER_BR_460800,
    eSYS_SER_BR_500000,
    eSYS_SER_BR_576000,
    eSYS_SER_BR_921600,
    eSYS_SER_BR_1000000,
    eSYS_SER_BR_1152000,
    eSYS_SER_BR_1500000,
    eSYS_SER_BR_2000000,
    eSYS_SER_BR_2500000,
    eSYS_SER_BR_3000000,
    eSYS_SER_BR_3500000,
    eSYS_SER_BR_4000000,
    eSYS_SER_BR_LAST
} sys_ser_baudrate_t;

//! Constants for configuring number of data bits per transmitted character.
typedef enum sys_ser_databits {
    eSYS_SER_DB_5 = 0,
    eSYS_SER_DB_6,
    eSYS_SER_DB_7,
    eSYS_SER_DB_8,
    eSYS_SER_DB_LAST
} sys_ser_databits_t;

//! Constants for parity settings.
typedef enum sys_ser_parity {
    eSYS_SER_P_NONE = 0,
    eSYS_SER_P_ODD,
    eSYS_SER_P_EVEN,
    eSYS_SER_P_MARK,
    eSYS_SER_P_LAST
} sys_ser_parity_t;

//! Constants for configuring number of stop bits per transmitted character.
typedef enum sys_ser_stopbits {
    eSYS_SER_SB_1 = 0,
    eSYS_SER_SB_1p5,
    eSYS_SER_SB_2,
    eSYS_SER_SB_LAST
} sys_ser_stopbits_t;

//! Constants for configuring the data flow control.
typedef enum sys_ser_flowctrl {
    eSYS_SER_FC_NONE = 0,
    eSYS_SER_FC_HW,
    eSYS_SER_FC_XONXOFF,
    eSYS_SER_FC_LAST
} sys_ser_flowctrl_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Open a connection to a derial device e.g. /dev/ttyS0
 *
 * @param[in]   device      Path and name of device.
 *
 * @return File descriptor if successful, otherwise INVALID_FD.
 * @note call sys_err_get_as_string() to get error description.
 */
sys_fd_t sys_serial_open(const char *device);

/**
 * Close a connection to a serial device.
 *
 * @param[in]   fd          File descriptor of device.
 */
void sys_serial_close(sys_fd_t fd);

/**
 * Set parameters of a serial connection
 *
 * @param[in]   fd          File descriptor of device.
 * @param[in]   baudrate    Baudrate to set. Can be one of sys_ser_baudrate_t.
 * @param[in]   databits    Databits to use. Can be one of sys_ser_databits_t.
 * @param[in]   parity      Usage of parity bit. Can be one of sys_ser_parity_t.
 * @param[in]   stopbits    Usage of stop bits. Can be one of sys_ser_stopbits_t.
 * @param[in]   flowcontrol Usage of hardware or software flow control. Can be
 *                          one of sys_ser_flowctrl_t.
 *
 * @return 0 if successful, otherwise errorcode.
 * @note call sys_err_get_as_string() to get error description.
 */
int sys_serial_set_params(sys_fd_t            fd,
                          sys_ser_baudrate_t  baudrate,
                          sys_ser_databits_t  databits,
                          sys_ser_parity_t    parity,
                          sys_ser_stopbits_t  stopbits,
                          sys_ser_flowctrl_t  flowcontrol);

/**
 * Send data over the serial connection.
 *
 * @param[in]   fd          File descriptor of device.
 * @param[in]   buf         Buffer containing data to send.
 * @param[in]   bufsize     Number of bytes to be received.
 *
 * @return Number of sent bytes if successful, otherwise -1.
 * @note call sys_err_get_as_string() to get error description.
 */
ssize_t sys_serial_send(sys_fd_t fd, const void *buf, size_t bufsize);

/**
 * Receive data over the serial connection.
 *
 * @param[in]   fd          File descriptor of device.
 * @param[in]   buf         Buffer to receive bytes to.
 * @param[in]   bufsize     Desired number of bytes to receive.
 *
 * @return Number of received bytes if successful, 0 if connection is closed
 *         remotely or -1 on error.
 * @note call sys_err_get_as_string() to get error description.
 */
ssize_t sys_serial_recv(sys_fd_t fd, void *buf, size_t bufsize);

/**
 * Flush serial device's RX and TX buffers.
 *
 * @param[in]   fd          File descriptor of device.
 */
void sys_serial_flush(sys_fd_t fd);

/**
 * Get pending bytes in serial device's send queue.
 *
 * @param[in]   fd          File descriptor of device.
 *
 * @returns bytes to be sent in send-queue.
 */
size_t sys_serial_get_pending_sendq(sys_fd_t fd);

/**
 * Get pending bytes waiting in receive queue.
 *
 * @param[in]   fd          File descriptor of device.
 *
 * @returns pending bytes in receive queue
 */
size_t sys_serial_get_pending_recvq(sys_fd_t fd);

/**
 * Set if calls to serial device e.g. send and receive shall be blocking or
 * non-blocking.
 *
 * @param[in]   fd          File descriptor of device.
 * @param[in]   blocking    true = calls shall be blocking.
 */
void sys_serial_set_blocking(sys_fd_t fd, bool blocking);

/**
 * Convert baudrate as integer into baudrate enumeration.
 *
 * @param[in]   baudrate    Baudrate as integer (e.g. 38400)
 *
 * @return Converted baudrate (e.g. eSYS_SER_BR_38400), if not convertable -1.
 */
sys_ser_baudrate_t sys_serial_baudrate(uint32_t baudrate);

#ifdef __cplusplus
}
#endif

#endif /* _SYSSERIAL_H_ */
/** @} */
