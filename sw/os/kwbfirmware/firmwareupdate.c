/**
 * @addtogroup FIRMWAREUPDATE
 *
 * @{
 * @file    firmwareupdate.c
 * @brief   Process firmware update of a module.
 *
 * message format for firmware update:
 * Byte 0       |Byte 1       |Byte 2       |Byte 3       |Byte 4       |Byte 5       |Byte 6       |Byte 7    ...
 * -------------+-------------+-------------+-------------+-------------+-------------+-------------+------------
 * Block-Start  |Datatype     |[highbyte ...        Start-Adresse         ...lowbyte ]|
 * Block-Data   |[high...   Offset   ...low]|[                         ... Data ...
 * Block-End    |[high...   CRC16    ...low]|
 * Block-Info   |[high..CRC16 expected..low]|[high..CRC16 calcul ...low]|[high...
 *
 * Datatype: 0 undefined
 *           1 internal EEProm
 *           2 ext. firmware EEProm
 *           3 string
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
  #include <safe_lib.h>
#endif

#include "cmddef_common.h"
#include "moddef_common.h"

#include "kwb_defines.h"

#include "crc16.h"
#include "firmwareupdate.h"
#include "ihex.h"
#include "log.h"
#include "message.h"
#include "message_serial.h"
#include "prjtypes.h"
#include "sysconsole.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Write a memory block as hexadecimal dump to a log file.
 *
 * @param[in]   data    Pointer to data.
 * @param[in]   length  Length in byte of memory to be logged.
 */
static void log_hexdump16 (log_mask_t logmask, char* keyword, uint8_t* data, uint16_t length)
{
    char        buffer[LOG_MAX_MESSAGE_LENGTH];
    char        hexbuffer[4];
    uint16_t    offset = 0;

    assert (data != NULL);

    buffer[0] = '\0';

    while (offset < length) {
        if ((offset % 16) == 0) {
            // log old message
            if (offset != 0) {
                log_msg(logmask, "%s %04X %s", keyword, offset-16, buffer);
            }
            // prepare new message
            buffer[0]='\0';
        }

        snprintf(hexbuffer, sizeof(hexbuffer), "%02X ", data[offset]);
        offset++;
        strcat_s(buffer, sizeof(buffer), hexbuffer);
    }
    // check if there is not-logged data
    if (offset % 16 != 0) {
        log_msg(logmask, "%s %04X %s", keyword, offset > 16 ? offset-16 : 0, buffer);
    }
}

static uint16_t calculate_crc16 (firmwareupdate_t* fwu)
{
    uint32_t ii;
    uint16_t crc;

    crc = CRC_START_VALUE;
    for (ii=0; ii<fwu->fw_size; ii++) {
        crc = crc_16_next_byte(crc, fwu->fw_memory[ii]);
    }
    return crc;
}

static void parse_set_reg_8bit_msg (msg_t* message, firmwareupdate_t* fwu)
{
    if (message->data[1] == MOD_eReg_BldFlag) {
        fwu->bldflags = message->data[2];
        fwu->bldflags_received = 1;
    }
}

/**
 * Handles block_info message.
 * Sets current node crc and current node write offset.
 */
static void parse_block_info_msg (msg_t* message, firmwareupdate_t* fwu)
{
    uint32_t node_last_address = 0;
    uint16_t node_crc_expected = 0, node_crc_calculated = 0;

    if (message->length >= 9 && fwu != NULL) {
        node_crc_expected |= (message->data[1] << 8);
        node_crc_expected |= (message->data[2]);
        node_crc_calculated |= (message->data[3] << 8);
        node_crc_calculated |= (message->data[4]);
        node_last_address |= (message->data[5] << 24);
        node_last_address |= (message->data[6] << 16);
        node_last_address |= (message->data[7] << 8);
        node_last_address |= (message->data[8]);

        fwu->last_node_address = node_last_address;
        fwu->node_crc_expected = node_crc_expected;
        fwu->node_crc_calculated = node_crc_calculated;
        fwu->block_info_received = 1;
    }
}

static void parse_block_ack_msg (msg_t* message, firmwareupdate_t* fwu)
{
    if (message->data[1] == eCMD_BLOCK_DATA) {
        fwu->last_node_offset = (message->data[2] << 8) | message->data[3];
    }
}

static void parse_block_nak_msg (msg_t* message, firmwareupdate_t* fwu)
{
    if (message->data[1] == eCMD_BLOCK_START ||
        message->data[1] == eCMD_BLOCK_DATA ||
        message->data[1] == eCMD_BLOCK_END) {
        fwu->curr_state = eFWU_ERROR_ABORT;
    }
}

/**
 * Message handler for incomming messages.
 * Specific message handlers will be called from here.
 *
 * param[in] message    Message to be parsed.
 * param[in] reference  Pointer to connection from where the message has been received.
 * param[in] arg        Special argument: Pointer to firmware update structure.
 */
static void handle_incomming_messages (msg_t* message, void* reference, void* arg)
{
    log_hexdump16(LOG_VERBOSE2, "BUS I ", message->data, message->length);

    if (message->length > 0) {
        // incomming command handler
	switch (message->data[0]) {
        case eCMD_STATE_8BIT:
            parse_set_reg_8bit_msg(message, arg);
            break;
        case eCMD_BLOCK_INFO:
            parse_block_info_msg(message, arg);
            break;
        case eCMD_ACK:
            parse_block_ack_msg(message, arg);
            break;
        case eCMD_NAK:
            parse_block_nak_msg(message, arg);
            break;
        default:
            break;
        }
    }
}

/**
 * Send a block start message to the target node. The block start message
 * contains the block-type and the first address in the target memory.
 *
 * @param[in]   fwu     Pointer to firmware update process data.
 *
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
static int send_block_start_message (firmwareupdate_t* fwu)
{
    int         rc = eRUNNING;
    msg_t       msg;

    memset(&msg, 0, sizeof(msg));
    msg.receiver = fwu->module_address;
    msg.data[0] = eCMD_BLOCK_START;
    msg.data[1] = 2; // blocktype external EEProm (firmware)
    msg.data[2] = (fwu->fw_firstaddress & 0xFF000000) >> 24; // first address (32bit) highest byte
    msg.data[3] = (fwu->fw_firstaddress & 0x00FF0000) >> 16; // first address (32bit)
    msg.data[4] = (fwu->fw_firstaddress & 0x0000FF00) >> 8;  // first address (32bit)
    msg.data[5] = (fwu->fw_firstaddress & 0x000000FF);       // first address (32bit) lowest byte
    msg.data[6] = (fwu->fw_size & 0xFF000000) >> 24; // size of data (32bit) highest byte
    msg.data[7] = (fwu->fw_size & 0x00FF0000) >> 16; // size of data (32bit)
    msg.data[8] = (fwu->fw_size & 0x0000FF00) >> 8;  // size of data (32bit)
    msg.data[9] = (fwu->fw_size & 0x000000FF);       // size of data (32bit) lowest byte
    msg.length = 10;

    rc = msg_ser_send(&fwu->msg_serial, &msg);
    return rc;
}

/**
 * Send a block data message to the target node. The block data message
 * contains an offset in the target memory and up to 32 byte of data.
 *
 * @param[in]   fwu     Pointer to firmware update process data.
 *
 * @returns     eRUNNING if message has been sent successfully and there is
 *              remaining data to be transmitted.
 * @returns     eERR_NONE if message has been sent successfully and all of the
 *              firmware data has been transmitted.
 * @returns     Otherwise, if an error occurs in a subroutine, the error-code is
 *              returned.
 */
static int send_block_data_message (firmwareupdate_t* fwu)
{
    int         rc = eRUNNING;
    uint32_t    remaining, ii, block;
    uint32_t    chunksize = 32;
    msg_t       msg;

    do {
        // bytes to send?
        if (fwu->fw_size > fwu->curr_offset) {
            remaining = fwu->fw_size - fwu->curr_offset;
        } else {
            rc = eERR_NONE;
            break;
        }

        // prepare message
        memset(&msg, 0, sizeof(msg));
        msg.receiver = fwu->module_address;
        msg.data[0] = eCMD_BLOCK_DATA;
        msg.data[1] = (fwu->curr_offset & 0xFF00) >> 8;
        msg.data[2] = (fwu->curr_offset & 0x00FF);

        // calculate data size of the message
        if (remaining > chunksize) {
            block = chunksize;
        } else {
            block = remaining;
        }

        // copy firmware memory chunk into message
        for (ii=0; ii<block; ii++) {
            msg.data[3 + ii] = fwu->fw_memory[fwu->curr_offset + ii];
        }
        msg.length = 3 + block;

        // send message and check if all data has been sent
        fwu->last_offset = fwu->curr_offset;
        rc = msg_ser_send(&fwu->msg_serial, &msg);
        if (rc == eERR_NONE || rc == eRUNNING) {
            fwu->curr_offset += block;
            remaining -= block;
        } else {
            // error occurred or send buffer full
            break;
        }

        // still bytes to send?
        if (remaining == 0) {
            rc = eERR_NONE;
        } else {
            rc = eRUNNING;
        }
    } while (false);
    return rc;
}

/**
 * Send a block end message to the target node. The block end message
 * contains the checksum of the sent data. The target node is expected to
 * calculate a checksum by itself and compare it to the checksum sent with the
 * block end message.
 *
 * @param[in]   fwu     Pointer to firmware update process data.
 *
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
static int send_block_end_message (firmwareupdate_t* fwu)
{
    int      rc = eERR_NONE;
    uint16_t crc;
    msg_t    msg;

    crc = calculate_crc16(fwu);

    memset(&msg, 0, sizeof(msg));
    msg.receiver = fwu->module_address;
    msg.data[0] = eCMD_BLOCK_END;
    msg.data[1] = (crc & 0xFF00) >> 8;
    msg.data[2] = (crc & 0x00FF);
    msg.length = 3;

    rc = msg_ser_send(&fwu->msg_serial, &msg);
    return rc;
}

/**
 * Send a reset message to the target node.
 *
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
static int send_reset_message (firmwareupdate_t* fwu)
{
    int      rc = eERR_NONE;
    msg_t    msg;

    memset(&msg, 0, sizeof(msg));
    msg.receiver = fwu->module_address;
    msg.data[0] = eCMD_RESET;
    msg.length = 1;

    rc = msg_ser_send(&fwu->msg_serial, &msg);
    return rc;
}

/**
 * Send a request register message to the target node.
 *
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
static int send_request_bldflags_message (firmwareupdate_t* fwu)
{
    int      rc = eERR_NONE;
    msg_t    msg;

    memset(&msg, 0, sizeof(msg));
    msg.receiver = fwu->module_address;
    msg.data[0] = eCMD_REQUEST_REG;
    msg.data[1] = MOD_eReg_BldFlag;
    msg.length = 2;

    rc = msg_ser_send(&fwu->msg_serial, &msg);
    return rc;
}

static int receive_and_check_bldflags (firmwareupdate_t* fwu)
{
    int rc = eRUNNING;

    if (fwu->bldflags_received) {
        rc = eERR_NONE;
        log_msg(LOG_STATUS, "BLDFlags: %d", fwu->bldflags);
        if ((fwu->bldflags & (1<<eBldFlagNewSWProgrammed)) == 0) {
            log_error("BLDFlags: Controller has NOT been flashed!");
            rc = eERR_PROCESS_FAILED;
        } else {
            log_msg(LOG_STATUS, "BLDFlags: Controller has been flashed.");
        }
        if (fwu->bldflags & (1<<eBldFlagCRCMismatch)) {
            log_error("BLDFlags: Node CRC mismatch for firmware! Firmware not flashed into the controller!");
            if (rc <= eERR_NONE) rc = eERR_PROCESS_FAILED;
        }
        if (fwu->bldflags & (1<<eBldFlagControllerTypeMismatch)) {
            log_error("BLDFlags: Node controller type mismatch. Firmware not flashed into the controller!");
            if (rc <= eERR_NONE) rc = eERR_PROCESS_FAILED;
        }
        if (fwu->bldflags & (1<<eBldFlagBoardTypeMismatch)) {
            log_warning("BLDFlags: Node board type mismatch. Firmware flashed. Check if firmware is suitable for the node board!");
        }
        if (fwu->bldflags & (1<<eBldFlagAppIDChanged)) {
            log_msg(LOG_STATUS, "BLDFlags: Node application ID changed");
        }
        if (fwu->bldflags & (1<<eBldFlagAppIDChanged)) {
            log_msg(LOG_STATUS, "BLDFlags: Application version changed");
        }
        if (fwu->bldflags & (1<<eBldFlagAppProgram)) {
            log_error("BLDFlags: Node reset not performed!");
            if (rc <= eERR_NONE) rc = eERR_PROCESS_FAILED;
        }
    }
    return rc;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize the firmware update process runtime data, establish a connection
 * to the target node (bus, rs232 or network) and register the communication
 * device at the ioloop module.
 * @note Currently only the connection over a RS232 gateway is supported.
 *
 * @param[in,out]   fwu         Pointer to firmware update process data.
 * @param[in]       ioloop      Pointer to ioloop handler.
 * @param[in]       device      Serial connection device.
 * @param[in]       baudrate    Baudrate of serial connection.
 *
 * @returns         0 if firmware update runtime data and environment is
 *                  successfully initialized.
 */
int firmware_update_init (firmwareupdate_t* fwu,
                          ioloop_t*         ioloop,
                          const char*       device,
                          int               baudrate)
{
    int rc = 0;

    do {
        assert(fwu != NULL);
        memset(fwu, 0, sizeof(firmwareupdate_t));

        msg_ser_init(&fwu->msg_serial);

        rc = msg_ser_open(&fwu->msg_serial, ioloop, device, baudrate);
        if (rc != eERR_NONE) break;

        msg_ser_set_incomming_handler(&fwu->msg_serial, handle_incomming_messages, fwu);
        // default: reset target node after update
        fwu->reset_target_node = 1;
        fwu->progress_thd = 5;
    } while (false);
    return rc;
}

/**
 * Start a firmware update process.
 *
 * @param[in,out]   fwu             Pointer to firmware update process data.
 * @param[in]       filename        Path and filename of IHEX file to be sent to
 *                                  the target node.
 * @param[in]       module_address  Module address of the target node.
 *
 * @returns         eERR_NONE if the firmware update process has successfully
 *                  been started.
 * @returns         eERR_MALLOC if the memory is not sufficient to load the
 *                  IHEX file.
 */
int firmware_update_start (firmwareupdate_t*    fwu,
                           const char*          filename,
                           uint16_t             module_address)
{
    int rc = eERR_NONE;
    uint32_t target_last_addr;

    assert(fwu != NULL);

    fwu->curr_state = eFWU_IDLE;
    strcpy_s(fwu->filename, sizeof(fwu->filename), filename);
    fwu->module_address = module_address;

    do {
        // calculate IHEX target memory size
        target_last_addr = 0;

        rc = ihex_read_file(fwu->filename,
                            &fwu->fw_startaddress,
                            &fwu->fw_firstaddress,
                            &target_last_addr, NULL, NULL);
        if (rc != eERR_NONE) {
            log_error("Failed to open %s, error %d", fwu->filename, rc);
            break;
        }
        fwu->fw_size = target_last_addr - fwu->fw_firstaddress + 1;
        log_msg(KWB_LOG_INFO, "target file: %s, module address %d", fwu->filename, fwu->module_address);
        log_msg(KWB_LOG_INFO, "target start address: 0x%04X", fwu->fw_startaddress);
        log_msg(KWB_LOG_INFO, "target first address: 0x%04X", fwu->fw_firstaddress);
        log_msg(KWB_LOG_INFO, "target last address:  0x%04X", target_last_addr);
        log_msg(KWB_LOG_INFO, "size:                 %d", fwu->fw_size);

        // allocate and initialie target memory. Initialize to 0xFF
        // because flash memory is 0xFF when it is erased.
        fwu->fw_memory = (uint8_t*)malloc(fwu->fw_size);
        memset(fwu->fw_memory, 0xFF, fwu->fw_size);
        if (fwu->fw_memory == NULL) {
            log_error("Out of memory. Failed to allocate %d bytes", fwu->fw_size);
            rc = eERR_MALLOC;
            break;
        }

        // read ihex content to memory
        rc = ihex_read_file_mem(filename, NULL, NULL, NULL,
                                fwu->fw_memory, fwu->fw_size);
        if (rc != eERR_NONE) {
            log_error("Failed to open %s, error %d", fwu->filename, rc);
            free(&fwu->fw_memory);
            fwu->fw_memory = NULL;
            break;
        }
        fwu->curr_state = eFWU_START;
    } while (false);
    return rc;
}

/**
 * Run the firmware update process. This function has to be called as long as
 * eRUNNING is returned.
 *
 * @param[in,out]   fwu             Pointer to firmware update process data.
 *
 * @returns         eRUNNING if the firmware is still beening transmitted.
 * @returns         eERR_NONE if the firmware has been transmitted completely.
 * @returns         Otherwise, if an error occurs in a subroutine, the errorcode
 *                  is returned.
 */
int firmware_update_run (firmwareupdate_t* fwu)
{
    int     rc = eRUNNING;
    uint8_t progress = 0;

    switch (fwu->curr_state) {
    case eFWU_IDLE:
        rc = eERR_NONE;
        break;
    case eFWU_START:
        // start the block transfer
        send_block_start_message(fwu);
        fwu->curr_state = eFWU_DATA;
        break;
    case eFWU_DATA:
        // send next chunk of data to the node
        fwu->block_info_received = 0;
        send_block_data_message(fwu);
        fwu->curr_state = eFWU_WAIT_ACK;
        break;
    case eFWU_WAIT_ACK:
        // wait until acknowledge message has been received
        if (fwu->last_offset == fwu->last_node_offset) {
            if (fwu->fw_size > fwu->curr_offset) {
                fwu->curr_state = eFWU_DATA;
            } else {
                fwu->curr_state = eFWU_END;
            }
            // update progress
            progress = (uint8_t)(100 * fwu->curr_offset / fwu->fw_size);
            if ((progress - fwu->last_progress) >= fwu->progress_thd || progress == 100) {
                fwu->last_progress = progress;
                if (fwu->progress_func != NULL) {
                    fwu->progress_func(progress, fwu->progress_arg);
                }
            }
        }
        break;
    case eFWU_END:
        fwu->block_info_received = 0;
        send_block_end_message(fwu);
        fwu->curr_state = eFWU_CRC_INFO;
        break;
    case eFWU_CRC_INFO:
        if (fwu->block_info_received) {
            if (fwu->node_crc_expected == fwu->node_crc_calculated) {
                if (fwu->reset_target_node) {
                    fwu->curr_state = eFWU_RESET_NODE;
                } else {
                    fwu->curr_state = eFWU_IDLE;
                }
            } else {
                fwu->curr_state = eFWU_ERROR_ABORT;
            }
        }
        break;
    case eFWU_RESET_NODE:
        send_reset_message(fwu);
        fwu->curr_state = eFWU_WAIT_AFTER_RESET;
        fwu->wait_start = sys_time_get_usecs();
        break;
    case eFWU_WAIT_AFTER_RESET:
        // wait 10 seconds
        if ((sys_time_get_usecs() - fwu->wait_start) >= 10000*1000) {
            fwu->curr_state = eFWU_BLDFLAGS;
        }
        break;
    case eFWU_BLDFLAGS:
        send_request_bldflags_message(fwu);
        fwu->curr_state = eFWU_WAIT_BLDFLAGS;
        fwu->wait_start = sys_time_get_usecs();
        break;
    case eFWU_WAIT_BLDFLAGS:
        if ((sys_time_get_usecs() - fwu->wait_start) >= 10000*1000) {
            fwu->curr_state = eFWU_ERROR_ABORT;
        }
        if ((rc = receive_and_check_bldflags(fwu)) >= 0) {
            fwu->curr_state = eFWU_IDLE;
        }
        break;
    case eFWU_ERROR_ABORT:
        rc = eERR_PROCESS_FAILED;
        fwu->curr_state = eFWU_IDLE;
        break;
    default:
        break;
    }

    return rc;
}

/**
 * Close and end the firmware update process. Also the connection to the target
 * node is closed.
 *
 * @param[in,out]   fwu             Pointer to firmware update process data.
 */
void firmware_update_close (firmwareupdate_t* fwu)
{
    assert(fwu != NULL);
    if (fwu->fw_memory != NULL) {
        free(fwu->fw_memory);
        fwu->fw_memory = NULL;
    }
    fwu->fw_size = 0;
    fwu->curr_offset = 0;

    msg_ser_close(&fwu->msg_serial);
}

/**
 * Register a function which is used to upgrade the progress of the firmware
 * upload.
 *
 * @param[in,out]   fwu             Pointer to firmware update process data.
 * @param[in]       func            Progress update function.
 * @param[in]       arg             Additional information for the func()
 */
void firmware_register_progress_func (firmwareupdate_t*     fwu,
                                      fwu_progress_func_t   func,
                                      void*                 arg)
{
    assert(fwu != NULL);
    fwu->progress_func = func;
    fwu->progress_arg = arg;
}

/**
 * Set threshold for the progress update.
 *
 * The progress update function is called, if the progress increased
 * at least thd since the last call of the progress update function.
 *
 * @param[in,out]   fwu             Pointer to firmware update process data.
 * @param[in]       thd             Threshold.
 */
void firmware_set_progress_thd (firmwareupdate_t*     fwu,
                                uint8_t               thd)
{
    assert(fwu != NULL);
    fwu->progress_thd = thd;
}

/** @} */
