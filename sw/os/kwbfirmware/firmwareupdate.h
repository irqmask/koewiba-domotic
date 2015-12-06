/**
 * @addtogroup FIRMWAREUPDATE
 * @brief Process firmware update of a module.
 *
 * @{
 * @file    firmwareupdate.h
 * @brief   Process firmware update of a module.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _FIRMWAREUPDATE_H_
#define _FIRMWAREUPDATE_H_

// --- Include section ---------------------------------------------------------

#include "message_serial.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum fwu_states {
    eFWU_IDLE,
    eFWU_START,
    eFWU_DATA,
    eFWU_WAIT_INFO,
    eFWU_END,
    eFWU_CRC_INFO,
    eFWU_LAST
} fwu_states_t;

//! Firmware update process data structure.
typedef struct firmwareupdate {
    fwu_states_t    curr_state;         //!< Current state of firmware-update statemachine.
    msg_serial_t    msg_serial;         //!< Handle of the serial connection.
    char            filename[256];      //!< Path and filename to the IHEX file.
    uint16_t        module_address;     //!< target module address.
    
    uint8_t*        fw_memory;          //!< Pointer to the memory block wich 
                                        //!< contains the target module's firmware.
    uint32_t        fw_firstaddress;    //!< First (lowest) address of the target
                                        //!< module's firmware.
    uint32_t        fw_startaddress;    //!< Program entry point of the target's 
                                        //!< firmware.
    uint32_t        fw_size;            //!< Size on bytes of the firmware.
    
    uint32_t        curr_offset;        //!< Current offset in fw_memory during 
                                        //!< transmission.
    uint32_t        node_curr_offset;   //!< Current offset in fw_memory during 
                                        //!< transmission.
    uint16_t        node_crc_expected;  //!< Checksum expected by node.
    uint16_t        node_crc_calculated;//!< Checksum calculated by node.
    uint8_t         block_info_received;//!< Flag if block_info message has been
                                        //!< received. Reset by block_data and 
                                        //!< block_end message. Set by block_info
                                        //!< message.
} firmwareupdate_t;
    
// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int firmware_update_init    (firmwareupdate_t*  fwu, 
                             ioloop_t*          ioloop,
                             const char*        device, 
                             int                baudrate);

int firmware_update_start   (firmwareupdate_t*  fwu,
                             const char*        filename,
                             uint16_t           module_address);

int firmware_update_run     (firmwareupdate_t*  fwu);

void firmware_update_close  (firmwareupdate_t*  fwu);

#endif // _FIRMWAREUPDATE_H_
/** @} */
