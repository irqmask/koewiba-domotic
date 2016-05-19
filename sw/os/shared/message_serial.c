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
//#include <stdio.h>
#include <string.h>

#include "ioloop.h"
#include "log.h"
#include "message_serial.h"
#include "sysserial.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------


typedef enum receive_state {
    eSER_RECV_STATE_IDLE,
    eSER_RECV_STATE_RECEIVER,
    eSER_RECV_STATE_LENGTH,
    eSER_RECV_STATE_DATA,
    eSER_RECV_STATE_NEWLINE
} receive_state_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void convert_byte_to_ascii (char* serial_buffer, uint8_t* offset,
                                   uint8_t byte)
{
    uint8_t nibble;

    nibble = byte >> 4;
    if (nibble < 10) {
        serial_buffer[(*offset)++] = nibble + '0';
    } else {
        serial_buffer[(*offset)++] = nibble + 'A' - 10;
    }
    nibble = byte & 0x0F;
    if (nibble < 10) {
        serial_buffer[(*offset)++] = nibble + '0';
    } else {
        serial_buffer[(*offset)++] = nibble + 'A' - 10;
    }
}

static int convert_char_to_nibble (char single_char, uint8_t* nibble)
{
    if (single_char >= '0' && single_char <= '9') {
        *nibble = (uint8_t)(single_char - '0');
    } else if (single_char >= 'a' && single_char <= 'f') {
        *nibble = (uint8_t)(single_char - ('a' - 10));
    } else if (single_char >= 'A' && single_char <= 'F') {
        *nibble = (uint8_t)(single_char - ('A' - 10));
    } else {
        return (uint8_t)-1;
    }
    return 0;
}

static void reset_incomming_on_error(msg_serial_t* msg_serial)
{
    uint8_t ii;

    log_error("SERIAL Incomming message corrupt: ");
    for (ii=0; ii<msg_serial->incomming_num_received; ii++) {
        switch (msg_serial->incomming_buffer[ii]) {
        case '\n': fprintf(stderr, "<NL>"); break;
        case '\r': fprintf(stderr, "<CR>"); break;
        default:   fprintf(stderr, "%c", msg_serial->incomming_buffer[ii]); break;
        }
    }
    fprintf(stderr, "total length = %d, message length = %d\n", msg_serial->incomming_num_received, msg_serial->incomming_message.length);
    msg_serial->incomming_num_received = 0;
    msg_serial->incomming_state = eSER_RECV_STATE_IDLE;
}

static BOOL process_receiving(msg_serial_t* msg_serial, char new_char)
{
    uint8_t digit, nibble, index;

    // first byte of new message received
    if (msg_serial->incomming_state == eSER_RECV_STATE_IDLE) {
        msg_serial->incomming_state = eSER_RECV_STATE_RECEIVER;
        msg_serial->incomming_message.sender = 0;
        msg_serial->incomming_message.receiver = 0;
        msg_serial->incomming_message.length = 0;
    }

    msg_serial->incomming_buffer[msg_serial->incomming_num_received++] = new_char;
    // below this line incomming_num_received is counting from 1!

    // decode received
    switch (msg_serial->incomming_state) {
    case eSER_RECV_STATE_RECEIVER:
        digit = 4 - msg_serial->incomming_num_received;
        if (convert_char_to_nibble(new_char, &nibble) != 0) {
            log_error("Error in state eSER_RECV_STATE_RECEIVER!");
            reset_incomming_on_error(msg_serial);
            break;
        } else {
            msg_serial->incomming_message.sender |= (nibble << (4*digit));
        }
        if (msg_serial->incomming_num_received >= 4) {
            msg_serial->incomming_state = eSER_RECV_STATE_LENGTH;
        }
        break;

    case eSER_RECV_STATE_LENGTH:
        digit = 6 - msg_serial->incomming_num_received;
        if (convert_char_to_nibble(new_char, &nibble) != 0) {
            log_error("Error in state eSER_RECV_STATE_LENGTH char %x!", new_char);
            reset_incomming_on_error(msg_serial);
            break;
        } else {
            msg_serial->incomming_message.length |= (nibble << (4*digit));
        }
        if (msg_serial->incomming_num_received >= 6) {
            msg_serial->incomming_state = eSER_RECV_STATE_DATA;
        }
        break;

    case eSER_RECV_STATE_DATA:
        digit = msg_serial->incomming_num_received % 2; // digit sequence: 1010...
        index = (msg_serial->incomming_num_received - 7) / 2;
        if (convert_char_to_nibble(new_char, &nibble) != 0) {
            log_error("Error in state eSER_RECV_STATE_DATA!");
            reset_incomming_on_error(msg_serial);
            break;
        } else {
            if (digit == 1) msg_serial->incomming_message.data[index] = 0;
            msg_serial->incomming_message.data[index] |= (nibble << (4*digit));
        }
        if (index + 1 == msg_serial->incomming_message.length && digit == 0) {
            msg_serial->incomming_state = eSER_RECV_STATE_NEWLINE;
        } else if (index + 1 > msg_serial->incomming_message.length){
            log_error("Error in state eSER_RECV_STATE_DATA!");
            reset_incomming_on_error(msg_serial);
        }
        break;

    case eSER_RECV_STATE_NEWLINE:
        if (msg_serial->incomming_num_received != (msg_serial->incomming_message.length * 2 + 7) ||
            new_char != '\n') {
            log_error("Error in state eSER_RECV_STATE_NEWLINE!");
            reset_incomming_on_error(msg_serial);
        } else {
            // message received
            msg_serial->incomming_num_received = 0;
            msg_serial->incomming_state = eSER_RECV_STATE_IDLE;
            return TRUE;
        }
        break;

    default:
        log_error("Wrong receive state: %d", msg_serial->incomming_state);
        reset_incomming_on_error(msg_serial);
        break;
    }
    return FALSE;
}

static int32_t msg_read (void* arg)
{
    msg_serial_t*   msg_serial = (msg_serial_t*)arg;
    char            new_char;
    BOOL            message_complete = FALSE;

    do {
        // reveive message
        do {
            // read one character, break if receive queue empty
            if (1 != sys_serial_recv(msg_serial->fd, &new_char, 1)) break;
            // process character and check for a complete message
            message_complete = process_receiving(msg_serial, new_char);
        } while (!message_complete);

        // handle complete message
        if (message_complete && msg_serial->incomming_handler != NULL) {
            msg_serial->incomming_handler(&msg_serial->incomming_message,
                                          msg_serial,
                                          msg_serial->incomming_arg);
        }
    } while (0);
    return 0;
}

static size_t format_serial_message (char* serial_buffer, uint8_t max_buffersize, msg_t* message)
{
    uint8_t ii, offset = 0;

    if (message->length > MAX_MSG_SIZE)  return 0;
    if (max_buffersize < (message->length*2 + 7)) return 0;
    memset(serial_buffer, 0, max_buffersize);
    // receiver
    convert_byte_to_ascii(serial_buffer, &offset, (message->receiver & 0xFF00) >> 8);
    convert_byte_to_ascii(serial_buffer, &offset, message->receiver & 0x00FF);
    // length
    convert_byte_to_ascii(serial_buffer, &offset, message->length);
    // message
    for (ii=0; ii<message->length; ii++) {
        convert_byte_to_ascii(serial_buffer, &offset, message->data[ii]);
    }
    serial_buffer[offset++] = '\n';
    serial_buffer[offset] = '\0';
    return offset;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize serial connection handle.
 *
 * @param[in] msg_serial    Serial connection handle.
 */
void msg_ser_init (msg_serial_t* msg_serial)
{
    assert(msg_serial != NULL);

    memset(msg_serial, 0, sizeof(msg_serial_t));
}

/**
 * Open serial connection.
 *
 * @param[in] msg_serial    Serial connection handle.
 * @param[in] ioloop        IOloop module handle.
 * @param[in] device        Path and filename of serial device.
 * @param[in] baudrate      Serial baudrate.
 */
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
            log_sys_error("msg_serial: error opening serial connection");
            rc = eERR_SYSTEM;
            break;
        }

        rc = sys_serial_set_params(msg_serial->fd,
                                   sys_serial_baudrate(baudrate),
                                   eSYS_SER_DB_8,
                                   eSYS_SER_P_NONE,
                                   eSYS_SER_SB_1,
                                   eSYS_SER_FC_HW);
        if (rc != eERR_NONE) {
            log_sys_error("msg_serial: error setting baudrate and serial parameters");
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
            log_error("msg_serial: error encoding serial message!");
            rc = eMSG_ERR_SIZE;
        } else {
            log_msg(LOG_VERBOSE2, "SERIAL %s", msg_serial->ser_data);
            msg_serial->ser_data_written = sys_serial_send(msg_serial->fd,
                                                           (void*)msg_serial->ser_data,
                                                           msg_serial->ser_data_length);
            if (msg_serial->ser_data_written < 0) {
                log_sys_error("msg_serial: serial send failed!");
                rc = eERR_SYSTEM;
            } else if (msg_serial->ser_data_written < msg_serial->ser_data_length) {
                log_warning("SERIAL Not all bytes written");
                rc = eRUNNING;
            }
        }
    } while (FALSE);
    int tx_queue_size = sys_serial_get_pending_sendq(msg_serial->fd);
    //printf("msg_serial: tx queue %d\n", tx_queue_size);

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
            log_sys_error("msg_serial: serial send failed!");
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

BOOL msg_ser_is_tx_empty (msg_serial_t* msg_serial)
{
    int pending_tx = sys_serial_get_pending_sendq(msg_serial->fd);

    if (pending_tx > 100) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/** @} */
