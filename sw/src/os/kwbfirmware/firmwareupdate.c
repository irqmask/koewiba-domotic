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

static int send_block_start_message (firmwareupdate_t* fwu)
{
    int rc = eERR_NONE;
    
    msg_t msg;
    
    memset_s(&msg, sizeof(msg), 0);
    msg.receiver = fwu->module_address;
    msg.data[0] = eCMD_BLOCK_START;
    msg.data[1] = 1; // blocktype firmware
    msg.data[2] = (fwu->fw_firstaddress & 0xFF000000) >> 24; // first address (32bit) highest byte
    msg.data[3] = (fwu->fw_firstaddress & 0x00FF0000) >> 16; // first address (32bit)
    msg.data[4] = (fwu->fw_firstaddress & 0x0000FF00) >> 8;  // first address (32bit)
    msg.data[5] = (fwu->fw_firstaddress & 0x000000FF);       // first address (32bit) lowest byte
    msg.length = 6;
    
    msg_log(msg);
    rc = msg_ser_send(&fwu->msg_serial, &msg);

    return rc;
}

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
            block = 32;
        } else {
            block = remaining;
        }

        // copy firmware memory chunk into message
        for (ii=0; ii<block; ii++) {
            msg.data[3 + ii] = fwu->fw_memory[fwu->curr_offset + ii];
        }
        msg.length = 3 + block;
    
        // send message and check if all data has been sent
        msg_log(msg);
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
    msg_log(msg);
    
    rc = msg_ser_send(&fwu->msg_serial, &msg);
    return rc;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

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

int firmware_update_start (firmwareupdate_t*    fwu,
                           const char*          filename,
                           uint16_t             module_address)
{
    int rc = 0;
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
        log_hexdump16(fwu->fw_memory, fwu->fw_size); 
        
        rc = send_block_start_message(fwu);
    } while (FALSE);
    return rc;
}

int firmware_update_run (firmwareupdate_t* fwu)
{
    int rc = eERR_NONE;
    printf("firmware update run()\n");
    
    rc = send_block_data_message(fwu);    
    if (rc == eERR_NONE) {
        rc = send_block_end_message(fwu);
    }
    return rc;
}

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
