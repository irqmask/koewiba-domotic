/**
 * @addtogroup FIRMWAREUPDATE
 *
 * @{
 * @file    firmwareupdate.cpp
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

#include <memory>

#include <assert.h>
#include <malloc.h>
#include <string.h>

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
extern "C" {
#include <safe_lib.h>
}
#endif

#include "cmddef_common.h"
#include "moddef_common.h"

#include "kwb_defines.h"

#include "connection.h"
#include "crc16.h"
#include "exceptions.h"
#include "ihex.h"
#include "log.h"
#include "message.h"
#include "prjtypes.h"
#include "sysconsole.h"
#include "systime.h"

#include "firmwareupdate.h"

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
static void log_hexdump16(log_mask_t logmask, const char *keyword, const uint8_t *data, uint16_t length)
{
    char        buffer[LOG_MAX_MESSAGE_LENGTH];
    char        hexbuffer[4];
    uint16_t    offset = 0;

    assert(data != NULL);

    buffer[0] = '\0';

    while (offset < length) {
        if ((offset % 16) == 0) {
            // log old message
            if (offset != 0) {
                log_msg(logmask, "%s %04X %s", keyword, offset - 16, buffer);
            }
            // prepare new message
            buffer[0] = '\0';
        }

        snprintf(hexbuffer, sizeof(hexbuffer), "%02X ", data[offset]);
        offset++;
        strcat_s(buffer, sizeof(buffer), hexbuffer);
    }
    // check if there is not-logged data
    if (offset % 16 != 0) {
        log_msg(logmask, "%s %04X %s", keyword, offset > 16 ? offset - 16 : 0, buffer);
    }
}

/**
 * Calculate the checksum of firmware in memory using the CRC16 algorithm.
 *
 * @return calculated CRC16 checksum.
 */
uint16_t FirmwareUpdate::calculateCRC16()
{
    uint32_t ii;
    uint16_t crc;

    crc = CRC_START_VALUE;
    for (ii = 0; ii < this->fw_size; ii++) {
        crc = crc_16_next_byte(crc, this->fw_memory[ii]);
    }
    return crc;
}

/**
 * Expect a SetReg message and parse it to retrieve the bootloader flags.
 *
 * @param[in] message Message to parse
 */
void FirmwareUpdate::parseSetReg8bitMsg(const msg_t &message)
{
    if (message.data[1] == MOD_eReg_BldFlag) {
        this->bldflags = message.data[2];
        this->bldflags_received = 1;
    }
}

/**
 * Handles block_info message.
 * Sets current node crc and current node write offset.
 *
 * @param[in] message Message to parse
 */
void FirmwareUpdate::parseBlockInfoMsg(const msg_t &message)
{
    uint32_t node_last_address = 0;
    uint16_t node_crc_expected = 0, node_crc_calculated = 0;

    if (message.length >= 9) {
        node_crc_expected |= (message.data[1] << 8);
        node_crc_expected |= (message.data[2]);
        node_crc_calculated |= (message.data[3] << 8);
        node_crc_calculated |= (message.data[4]);
        node_last_address |= (message.data[5] << 24);
        node_last_address |= (message.data[6] << 16);
        node_last_address |= (message.data[7] << 8);
        node_last_address |= (message.data[8]);

        this->last_node_address = node_last_address;
        this->node_crc_expected = node_crc_expected;
        this->node_crc_calculated = node_crc_calculated;
        this->block_info_received = 1;
    }
}

/**
 * Expect a block acknowledge message and advance offset of already sent firmware.
 *
 * @param[in] message Message to parse
 */
void FirmwareUpdate::parseBlockAckMsg(const msg_t &message)
{
    if (message.data[1] == eCMD_BLOCK_DATA) {
        this->last_node_offset = (message.data[2] << 8) | message.data[3];
    }
}

/**
 * Expect a block not-acknowledge message and abort firmware update.
 *
 * @param[in] message Message to parse
 */
void FirmwareUpdate::parseBlockNakMsg(const msg_t &message)
{
    if (message.data[1] == eCMD_BLOCK_START ||
        message.data[1] == eCMD_BLOCK_DATA ||
        message.data[1] == eCMD_BLOCK_END) {
        this->curr_state = eFWU_ERROR_ABORT;
    }
}

/**
 * Message handler for incomming messages.
 * Specific message handlers will be called from here.
 *
 * @param[in] message    Message to be parsed.
 * @param[in] reference  Pointer to connection from where the message has been received. (unused)
 */
void FirmwareUpdate::handleIncomingMessage(const msg_t &message, void *reference)
{
    (reference);
    log_hexdump16(LOG_VERBOSE1, "BUS I ", (const uint8_t *)&message.data, message.length);

    if (message.length > 0) {
        // incomming command handler
        switch (message.data[0]) {
        case eCMD_STATE_8BIT:
            parseSetReg8bitMsg(message);
            break;
        case eCMD_BLOCK_INFO:
            parseBlockInfoMsg(message);
            break;
        case eCMD_ACK:
            parseBlockAckMsg(message);
            break;
        case eCMD_NAK:
            parseBlockNakMsg(message);
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
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
int FirmwareUpdate::sendBlockStartMessage()
{
    int         rc = eRUNNING;
    msg_t       msg;

    memset(&msg, 0, sizeof(msg));
    msg.receiver = this->module_address;
    msg.data[0] = eCMD_BLOCK_START;
    msg.data[1] = 2; // blocktype external EEProm (firmware)
    msg.data[2] = (this->fw_firstaddress & 0xFF000000) >> 24; // first address (32bit) highest byte
    msg.data[3] = (this->fw_firstaddress & 0x00FF0000) >> 16; // first address (32bit)
    msg.data[4] = (this->fw_firstaddress & 0x0000FF00) >> 8;  // first address (32bit)
    msg.data[5] = (this->fw_firstaddress & 0x000000FF);       // first address (32bit) lowest byte
    msg.data[6] = (this->fw_size & 0xFF000000) >> 24; // size of data (32bit) highest byte
    msg.data[7] = (this->fw_size & 0x00FF0000) >> 16; // size of data (32bit)
    msg.data[8] = (this->fw_size & 0x0000FF00) >> 8;  // size of data (32bit)
    msg.data[9] = (this->fw_size & 0x000000FF);       // size of data (32bit) lowest byte
    msg.length = 10;

    try {
        this->connection->send(msg);
    }
    catch (Exception &e) {
        log_error("Unable to send block end message: %s", e.what());
        rc = eERR_PROCESS_FAILED;
    }
    return rc;
}

/**
 * Send a block data message to the target node. The block data message
 * contains an offset in the target memory and up to 32 byte of data.
 *
 * @returns     eRUNNING if message has been sent successfully and there is
 *              remaining data to be transmitted.
 * @returns     eERR_NONE if message has been sent successfully and all of the
 *              firmware data has been transmitted.
 * @returns     Otherwise, if an error occurs in a subroutine, the error-code is
 *              returned.
 */
int FirmwareUpdate::sendBlockDataMessage()
{
    int         rc = eRUNNING;
    uint32_t    remaining, ii, block;
    uint32_t    chunksize = 32;
    msg_t       msg;

    do {
        // bytes to send?
        if (this->fw_size > this->curr_offset) {
            remaining = this->fw_size - this->curr_offset;
        }
        else {
            rc = eERR_NONE;
            break;
        }

        // prepare message
        memset(&msg, 0, sizeof(msg));
        msg.receiver = this->module_address;
        msg.data[0] = eCMD_BLOCK_DATA;
        msg.data[1] = (this->curr_offset & 0xFF00) >> 8;
        msg.data[2] = (this->curr_offset & 0x00FF);

        // calculate data size of the message
        if (remaining > chunksize) {
            block = chunksize;
        }
        else {
            block = remaining;
        }

        // copy firmware memory chunk into message
        for (ii = 0; ii < block; ii++) {
            msg.data[3 + ii] = this->fw_memory[this->curr_offset + ii];
        }
        msg.length = 3 + block;

        // send message and check if all data has been sent
        this->last_offset = this->curr_offset;
        try {
            this->connection->send(msg);
        }
        catch (Exception &e) {
            log_error("Unable to send block data message: %s", e.what());
            rc = eERR_PROCESS_FAILED;
        }
        if (rc == eERR_NONE || rc == eRUNNING) {
            this->curr_offset += block;
            remaining -= block;
        }
        else {
            // error occurred or send buffer full
            break;
        }

        // still bytes to send?
        if (remaining == 0) {
            rc = eERR_NONE;
        }
        else {
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
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
int FirmwareUpdate::sendBlockEndMessage()
{
    int      rc = eERR_NONE;
    uint16_t crc;
    msg_t    msg;

    crc = calculateCRC16();

    memset(&msg, 0, sizeof(msg));
    msg.receiver = this->module_address;
    msg.data[0] = eCMD_BLOCK_END;
    msg.data[1] = (crc & 0xFF00) >> 8;
    msg.data[2] = (crc & 0x00FF);
    msg.length = 3;
    log_msg(LOG_VERBOSE1, "CRC to be expected by node = 0x%4X", crc);
    try {
        this->connection->send(msg);
    }
    catch (Exception &e) {
        log_error("Unable to send block end message: %s", e.what());
        rc =  eERR_PROCESS_FAILED;
    }
    return rc;
}

/**
 * Send a reset message to the target node.
 *
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
int FirmwareUpdate::sendResetMessage()
{
    int      rc = eERR_NONE;
    msg_t    msg;

    memset(&msg, 0, sizeof(msg));
    msg.receiver = this->module_address;
    msg.data[0] = eCMD_RESET;
    msg.length = 1;

    try {
        this->connection->send(msg);
    }
    catch (Exception &e) {
        log_error("Unable to send reset message: %s", e.what());
        rc =  eERR_PROCESS_FAILED;
    }
    return rc;
}

/**
 * Send a request register message to the target node.
 *
 * @returns     eERR_NONE if message has been sent successfully, otherwise
 *              errorcode.
 */
int FirmwareUpdate::sendRequestBldflagsMessage()
{
    int      rc = eERR_NONE;
    msg_t    msg;

    memset(&msg, 0, sizeof(msg));
    msg.receiver = this->module_address;
    msg.data[0] = eCMD_REQUEST_REG;
    msg.data[1] = MOD_eReg_BldFlag;
    msg.length = 2;

    try {
        this->connection->send(msg);
    }
    catch (Exception &e) {
        log_error("Unable to send request bldflags message: %s", e.what());
        rc = eERR_PROCESS_FAILED;
    }
    return rc;
}

/**
 * Check if bootloader flags have been received. Pase it if received.
 *
 * @returns     eERR_PROCESS_FAILED if bootloader flags signal an unsuccesful update
 * @returns     eERR_NONE if bootloader flags signal a succesful update
 * @returns     eRUNNING if bootloader flags haven't been received yet.
 */
int FirmwareUpdate::receiveAndCheckBldflags()
{
    int rc = eRUNNING;

    if (this->bldflags_received) {
        rc = eERR_NONE;
        log_msg(LOG_INFO, "BLDFlags: %d", this->bldflags);
        if ((this->bldflags & (1 << eBldFlagNewSWProgrammed)) == 0) {
            log_error("BLDFlags: Controller has NOT been flashed!");
            rc = eERR_PROCESS_FAILED;
        }
        else {
            log_msg(LOG_INFO, "BLDFlags: Controller has been flashed.");
        }
        if (this->bldflags & (1 << eBldFlagCRCMismatch)) {
            log_error("BLDFlags: Node CRC mismatch for firmware! Firmware not flashed into the controller!");
            if (rc <= eERR_NONE) {
                rc = eERR_PROCESS_FAILED;
            }
        }
        if (this->bldflags & (1 << eBldFlagControllerTypeMismatch)) {
            log_error("BLDFlags: Node controller type mismatch. Firmware not flashed into the controller!");
            if (rc <= eERR_NONE) {
                rc = eERR_PROCESS_FAILED;
            }
        }
        if (this->bldflags & (1 << eBldFlagBoardTypeMismatch)) {
            log_warning("BLDFlags: Node board type mismatch. Firmware flashed. Check if firmware is suitable for the node board!");
        }
        if (this->bldflags & (1 << eBldFlagAppIDChanged)) {
            log_msg(LOG_INFO, "BLDFlags: Node application ID changed");
        }
        if (this->bldflags & (1 << eBldFlagAppIDChanged)) {
            log_msg(LOG_INFO, "BLDFlags: Application version changed");
        }
        if (this->bldflags & (1 << eBldFlagAppProgram)) {
            log_error("BLDFlags: Node reset not performed!");
            if (rc <= eERR_NONE) {
                rc = eERR_PROCESS_FAILED;
            }
        }
    }
    return rc;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * standard ctor
 */
FirmwareUpdate::FirmwareUpdate()
    : reset_target_node(1)
    , progress_thd(5)
{
}

/**
 * Initialize the firmware update process runtime data, establish a connection
 * to the target node (bus, rs232 or network) and register the communication
 * device at the ioloop module.
 * @note Currently only the connection over a RS232 gateway is supported.
 *
 * @param[in]       connection  Pointer to established connection to the KWB system (gateway or router).
 */
FirmwareUpdate::FirmwareUpdate(std::shared_ptr<Connection> connection)
    : FirmwareUpdate()
{
    this->connection = connection;

    using std::placeholders::_1;
    using std::placeholders::_2;
    incom_func_t handle_incoming_message_func = std::bind(&FirmwareUpdate::handleIncomingMessage, this, _1, _2);
    this->connection->setIncomingHandler(handle_incoming_message_func);
}

/**
 * Close and end the firmware update process.
 */
FirmwareUpdate::~FirmwareUpdate()
{
    if (this->fw_memory != NULL) {
        free(this->fw_memory);
    }
}

/**
 * Start a firmware update process.
 *
 * @param[in]       filename        Path and filename of IHEX file to be sent to
 *                                  the target node.
 * @param[in]       module_address  Module address of the target node.
 *
 * @returns         eERR_NONE if the firmware update process has successfully
 *                  been started.
 * @returns         eERR_MALLOC if the memory is not sufficient to load the
 *                  IHEX file.
 */
int FirmwareUpdate::start(const char          *filename,
                          uint16_t             module_address)
{
    int rc = eERR_NONE;
    uint32_t target_last_addr;

    this->curr_state = eFWU_IDLE;
    strcpy_s(this->filename, sizeof(this->filename), filename);
    this->module_address = module_address;

    do {
        // calculate IHEX target memory size
        target_last_addr = 0;

        rc = ihex_read_file(this->filename,
                            &this->fw_startaddress,
                            &this->fw_firstaddress,
                            &target_last_addr, NULL, NULL);
        if (rc != eERR_NONE) {
            log_error("Failed to open %s, error %d", this->filename, rc);
            break;
        }
        this->fw_size = target_last_addr - this->fw_firstaddress + 1;
        log_msg(LOG_INFO, "target file: %s, module address %d", this->filename, this->module_address);
        log_msg(LOG_INFO, "target start address: 0x%04X", this->fw_startaddress);
        log_msg(LOG_INFO, "target first address: 0x%04X", this->fw_firstaddress);
        log_msg(LOG_INFO, "target last address:  0x%04X", target_last_addr);
        log_msg(LOG_INFO, "size:                 %d", this->fw_size);

        // allocate and initialie target memory. Initialize to 0xFF
        // because flash memory is 0xFF when it is erased.
        this->fw_memory = (uint8_t *)malloc(this->fw_size);
        memset(this->fw_memory, 0xFF, this->fw_size);
        if (this->fw_memory == NULL) {
            log_error("Out of memory. Failed to allocate %d bytes", this->fw_size);
            rc = eERR_RESOURCE;
            break;
        }

        // read ihex content to memory
        rc = ihex_read_file_mem(filename, NULL, NULL, NULL,
                                this->fw_memory, this->fw_size);
        if (rc != eERR_NONE) {
            log_error("Failed to open %s, error %d", this->filename, rc);
            free(&this->fw_memory);
            this->fw_memory = NULL;
            break;
        }
        this->curr_state = eFWU_START;
    } while (false);
    return rc;
}

/**
 * Run the firmware update process. This function has to be called as long as
 * eRUNNING is returned.
 *
 * @returns         eRUNNING if the firmware is still beening transmitted.
 * @returns         eERR_NONE if the firmware has been transmitted completely.
 * @returns         Otherwise, if an error occurs in a subroutine, the errorcode
 *                  is returned.
 */
int FirmwareUpdate::run()
{
    int     rc = eRUNNING;
    uint8_t progress = 0;

    switch (this->curr_state) {
    case eFWU_IDLE:
        rc = eERR_NONE;
        break;
    case eFWU_START:
        // start the block transfer
        sendBlockStartMessage();
        this->curr_state = eFWU_DATA;
        break;
    case eFWU_DATA:
        // send next chunk of data to the node
        this->block_info_received = 0;
        sendBlockDataMessage();
        this->curr_state = eFWU_WAIT_ACK;
        break;
    case eFWU_WAIT_ACK:
        // wait until acknowledge message has been received
        if (this->last_offset == this->last_node_offset) {
            if (this->fw_size > this->curr_offset) {
                this->curr_state = eFWU_DATA;
            }
            else {
                this->curr_state = eFWU_END;
            }
            // update progress
            progress = (uint8_t)(100 * this->curr_offset / this->fw_size);
            if ((progress - this->last_progress) >= this->progress_thd || progress == 100) {
                this->last_progress = progress;
                if (this->progress_func != NULL) {
                    this->progress_func(progress, this->progress_arg);
                }
            }
        }
        break;
    case eFWU_END:
        this->block_info_received = 0;
        sendBlockEndMessage();
        this->curr_state = eFWU_CRC_INFO;
        break;
    case eFWU_CRC_INFO:
        if (this->block_info_received) {
            log_msg(LOG_VERBOSE1, "CRC expected by node = 0x%4X", this->node_crc_expected);
            log_msg(LOG_VERBOSE1, "CRC calculated by node = 0x%4X", this->node_crc_calculated);

            if (this->node_crc_expected == this->node_crc_calculated) {
                if (this->reset_target_node) {
                    this->curr_state = eFWU_RESET_NODE;
                }
                else {
                    this->curr_state = eFWU_IDLE;
                }
            }
            else {
                this->curr_state = eFWU_ERROR_ABORT;
            }
        }
        break;
    case eFWU_RESET_NODE:
        sendResetMessage();
        this->curr_state = eFWU_WAIT_AFTER_RESET;
        this->wait_start = sys_time_get_usecs();
        break;
    case eFWU_WAIT_AFTER_RESET:
        // wait 10 seconds
        if ((sys_time_get_usecs() - this->wait_start) >= 10000 * 1000) {
            this->curr_state = eFWU_BLDFLAGS;
        }
        break;
    case eFWU_BLDFLAGS:
        sendRequestBldflagsMessage();
        this->curr_state = eFWU_WAIT_BLDFLAGS;
        this->wait_start = sys_time_get_usecs();
        break;
    case eFWU_WAIT_BLDFLAGS:
        if ((sys_time_get_usecs() - this->wait_start) >= 10000 * 1000) {
            this->curr_state = eFWU_ERROR_ABORT;
        }
        if ((rc = receiveAndCheckBldflags()) >= 0) {
            this->curr_state = eFWU_IDLE;
        }
        break;
    case eFWU_ERROR_ABORT:
        rc = eERR_PROCESS_FAILED;
        this->curr_state = eFWU_IDLE;
        break;
    default:
        break;
    }

    return rc;
}

/**
 * Register a function which is used to upgrade the progress of the firmware
 * upload.
 *
 * @param[in]       func            Progress update function.
 * @param[in]       arg             Additional information for the func()
 */
void FirmwareUpdate::registerProgressFunc(fwu_progress_func_t   func,
                                          void                 *arg)
{
    this->progress_func = func;
    this->progress_arg = arg;
}

/**
 * Set threshold for the progress update.
 *
 * The progress update function is called, if the progress increased
 * at least thd since the last call of the progress update function.
 *
 * @param[in]       thd             Threshold.
 */
void FirmwareUpdate::setProgressThd(uint8_t               thd)
{
    this->progress_thd = thd;
}

/**
  * Get the bootloader flags which have been retrieved from the updated module.
  *
  * @returns Bootloader flags
  */
uint8_t FirmwareUpdate::getBldFlags()
{
    return this->bldflags;
}

/**
 * Get the calculated CRC of the uploaded firmware
 * @return Node's calculated CRC of uploaded firmware
 */
uint16_t FirmwareUpdate::getNodeCrcCalculated()
{
    return this->node_crc_calculated;
}

/**
 * Get the expected CRC of uploaded firmware
 * @return Expected CRC of uploaded firmware
 */
uint16_t FirmwareUpdate::getNodeCrcExpected()
{
    return this->node_crc_expected;
}

/** @} */
