/**
 * @addtogroup MESSAGE_SERIAL
 *
 * @{
 * @file    message_serial.c
 * @brief   Bus messages are transmitted or received via a serial connection  
 *          between the application and a gateway.
 * 
 * Binary format of a message sent over a serial line:
 * [16-bit address receiver][length][message]
 * [highbyte]      [lowbyte]
 * 
 * ASCII format of a message sent over a serial line:
 * Bytes are transmitted in the following way 01EA3F7C
 * A message ends with a cariage return character.
 * [16-bit address receiver][length][message][CR]
 * [highbyte]      [lowbyte]
 * 
 * 
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "ioloop.h"
#include "message_serial.h"
#include "sysserial.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static int32_t msg_read (void* arg)
{
    int             rc;
    msg_serial_t*   msg_serial = (msg_serial_t*)arg;
    msg_t           message;

    do {
        // reveive message
        //rc = sys_serial_recv(msg_serial->fd, &message, sizeof(message));

        // handle message
        if (msg_serial->incomming_handler != NULL) {
            msg_serial->incomming_handler(&message, msg_serial, msg_serial->incomming_arg);
        }
    } while (0);
    return 0;
}

static size_t format_serial_message (char* serial_buffer, uint8_t max_buffersize, msg_t* message)
{
    uint8_t ii;

    if (message->length > MAX_MSG_SIZE) return 0;
    if (max_buffersize < (message->length + 3)) return 0;
    
    // receiver    
    serial_buffer[0] = (message->receiver & 0xFF00) >> 8;
    serial_buffer[1] = message->receiver & 0x00FF;
    // length
    serial_buffer[2] = message->length;
    // message
    for (ii=0; ii<message->length; ii++) {
        serial_buffer[2+ii] = message->data[ii];
    }
    return message->length + 3;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg_ser_init (msg_serial_t* msg_serial)
{
    assert(msg_serial != NULL);

    memset(msg_serial, 0, sizeof(msg_serial_t));
}

int msg_ser_open (msg_serial_t*   msg_serial,
                  ioloop_t*       ioloop,
                  const char*     device,
                  int             baudrate)
{
    int             rc = eERR_NONE;
    sys_fd_t        fd;

    do {
        assert(msg_serial != NULL);
        assert(ioloop != NULL);
        assert(device != NULL);

        msg_serial->ioloop = ioloop;

        strncpy_s(msg_serial->device,
                  sizeof(msg_serial->device),
                  device,
                  strlen(device));
        msg_serial->baudrate = baudrate;

        msg_serial->fd = sys_serial_open(device);
        if (msg_serial->fd == INVALID_FD) {
            perror("error opening serial connection");
            rc = eERR_SYSTEM;
            break;
        }
        
        rc = sys_serial_set_params(msg_serial->fd,
                                   sys_serial_baudrate(baudrate),
                                   eSYS_SER_DB_8,
                                   eSYS_SER_P_NONE,
                                   eSYS_SER_SB_1);
        if (rc != eERR_NONE) {
            perror("error setting baudrate and serial parameters");
            break;
        }
        sys_serial_flush(msg_serial->fd);
        
        ioloop_register_fd(ioloop, msg_serial->fd, eIOLOOP_EV_READ, msg_read, (void*)msg_serial);
    } while (0);
    return rc;
}

void msg_ser_close (msg_serial_t* msg_serial)
{
    assert(msg_serial != NULL);

    ioloop_unregister_fd(msg_serial->ioloop, msg_serial->fd);
    sys_serial_close (msg_serial->fd);
}

void msg_ser_set_incomming_handler (msg_serial_t* msg_serial, msg_incom_func_t func, void* arg)
{
    assert(msg_serial != NULL);

    msg_serial->incomming_handler = func;
    msg_serial->incomming_arg = arg;
}

int msg_ser_send (msg_serial_t* msg_serial, msg_t* message)
{
    int     rc = eERR_NONE;
    
    do {
        assert(msg_serial != NULL);
        assert(message != NULL);

        // still bytes left to write (from previous call)?
        if (msg_serial->ser_data_length > msg_serial->ser_data_written) {
            rc = msg_ser_continue_sending(msg_serial);
            if (rc == eRUNNING) {
                rc = eMSG_ERR_BUSY;
                break; // still not ready to send new message
            }
        }
        
        msg_serial->ser_data_length = format_serial_message(msg_serial->ser_data, sizeof(msg_serial->ser_data), message);
        if (msg_serial->ser_data_length == 0) {
            fprintf(stderr, "msg_ser: error encoding serial message!");
            rc = eMSG_ERR_SIZE;
        } else {            
            msg_serial->ser_data_written = sys_serial_send(msg_serial->fd, 
                                                           (void*)msg_serial->ser_data, 
                                                           msg_serial->ser_data_length);
            if (msg_serial->ser_data_written < 0) {
                perror("serial send failed!");
                rc = eERR_SYSTEM;
            } else if (msg_serial->ser_data_written < msg_serial->ser_data_length) {
                fprintf(stderr, "msg_ser: not all bytes written\n");
                rc = eRUNNING;
            }            
        }
    } while (FALSE);
    int tx_queue_size = sys_serial_get_pending_sendq(msg_serial->fd);
    printf("tx queue %d\n", tx_queue_size);
        
    return rc;
}

int msg_ser_continue_sending (msg_serial_t* msg_serial)
{
    int rc = eERR_NONE;
    size_t to_send = 0, written = 0;
    
    assert(msg_serial != NULL);
    
    if (msg_serial->ser_data_written < msg_serial->ser_data_length) {
        to_send = msg_serial->ser_data_length - msg_serial->ser_data_written;
        written = sys_serial_send(msg_serial->fd, 
                                  &msg_serial->ser_data[msg_serial->ser_data_written], 
                                  to_send);
        if (written < 0) {
            perror("serial send failed!");
            rc = eERR_SYSTEM;
        } else {
            msg_serial->ser_data_written += written;
            if (msg_serial->ser_data_written < msg_serial->ser_data_length) {
                rc = eRUNNING;
            }
        }
    }
    return rc;
}

bool msg_ser_is_tx_empty (msg_serial_t* msg_serial)
{
    int pending_tx = sys_serial_get_pending_sendq(msg_serial->fd);
    
    if (pending_tx > 100) {
        return false;
    } else {
        return true;
    }
}

/** @} */
