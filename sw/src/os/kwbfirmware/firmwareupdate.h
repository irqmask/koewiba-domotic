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

//! Firmware update process data structure.
typedef struct firmwareupdate {
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
