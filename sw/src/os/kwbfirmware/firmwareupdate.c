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
 *
 * Datatype: 0 undefined
 *           1 internal EEProm
 *           2 ext. firmware EEProm
 *           3 string
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <assert.h>
#include <malloc.h>
#include <safe_lib.h>

#include "cmddef_common.h"
#include "crc16.h"
#include "firmwareupdate.h"
#include "message.h"
#include "message_serial.h"
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
void log_hexdump16 (uint8_t* data, uint16_t length)
{
    uint16_t offset = 0;

    assert (data != NULL);

    while (offset < length) {
        if ((offset % 16) == 0) {
            if (offset != 0) printf("\n");
            printf("%04X: ", offset);
        }
        printf("%02X ", data[offset]);
        offset++;
    }
    printf("\n");
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
    int rc = eERR_NONE;

    msg_t msg;

    memset_s(&msg, sizeof(msg), 0);
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

    //TODO msg_log(msg);
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
        memset_s(&msg, sizeof(msg), 0);
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
        //TODO msg_log(msg);
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
    } while (FALSE);
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
    int rc = eERR_NONE;
    uint16_t crc;
    msg_t    msg;

    crc = crc_calc16(fwu->fw_memory, fwu->fw_size);

    memset_s(&msg, sizeof(msg), 0);
    msg.receiver = fwu->module_address;
    msg.data[0] = eCMD_BLOCK_END;
    msg.data[1] = (crc & 0xFF00) >> 8;
    msg.data[2] = (crc & 0x00FF);
    msg.length = 3;
    //TODOmsg_log(msg);

    rc = msg_ser_send(&fwu->msg_serial, &msg);
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
        printf("firmware update init()\n");
        assert(fwu != NULL);
        memset_s(fwu, sizeof(firmwareupdate_t), 0);

        msg_ser_init(&fwu->msg_serial);

        rc = msg_ser_open(&fwu->msg_serial, ioloop, device, baudrate);
        if (rc != eERR_NONE) break;

    } while (FALSE);
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

    printf("firmware update start()\n");

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
            fprintf(stderr, "Failed to open %s, error %d\n", fwu->filename, rc);
            break;
        }
        fwu->fw_size = target_last_addr - fwu->fw_firstaddress + 1;
        printf("target file: %s, module address %d\n", fwu->filename, fwu->module_address);
        printf("target start address: 0x%04X\n", fwu->fw_startaddress);
        printf("target first address: 0x%04X\n", fwu->fw_firstaddress);
        printf("target last address:  0x%04X\n", target_last_addr);
        printf("target memory size:   %d\n", fwu->fw_size);

        // allocate and initialie target memory. Initialize to 0xFF
        // because flash memory is 0xFF when it is erased.
        fwu->fw_memory = malloc(fwu->fw_size);
        memset_s(fwu->fw_memory, fwu->fw_size, 0xFF);
        if (fwu->fw_memory == NULL) {
            fprintf(stderr, "Out of memory. Failed to allocate %d bytes\n", fwu->fw_size);
            rc = eERR_MALLOC;
            break;
        }

        // read ihex content to memory
        rc = ihex_read_file_mem(filename, NULL, NULL, NULL,
                                fwu->fw_memory, fwu->fw_size);
        if (rc != eERR_NONE) {
            fprintf(stderr, "Failed to open %s, error %d\n", fwu->filename, rc);
            free(&fwu->fw_memory);
            fwu->fw_memory = NULL;
            break;
        }
        //log_hexdump16(fwu->fw_memory, fwu->fw_size);

        rc = send_block_start_message(fwu);
    } while (FALSE);
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
    int rc = eERR_NONE;
    //printf("firmware update run()\n");

    rc = send_block_data_message(fwu);
    if (rc == eERR_NONE) {
        rc = send_block_end_message(fwu);
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
    printf("firmware update close()\n");
    if (fwu->fw_memory != NULL) {
        free(fwu->fw_memory);
        fwu->fw_memory = NULL;
    }
    fwu->fw_size = 0;
    fwu->curr_offset = 0;

    msg_ser_close(&fwu->msg_serial);
}

/** @} */
