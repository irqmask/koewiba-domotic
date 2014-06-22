/**
 * @addtogroup REGISTER
 * @brief Module to handle register read and write operations.
 *
 * Contains access functions for channels and registers for different data
 * types.
 *
 * @{
 * @file    register.h
 * @brief   Module to handle register read and write operations.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _REGISTER_H_
#define _REGISTER_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#include "bus.h"

// --- Definitions -------------------------------------------------------------

#define REG_MAX_MAPPINGS    32  //!< Maximum number of register mappings.

// --- Type definitions --------------------------------------------------------

//! Type of a register
typedef enum regtype {
    eRegType_Unkown,        //!< unknown register type
    eRegType_U8,            //!< 8bit register type
    eRegType_U16,           //!< 16bit register type
    eRegType_U32,           //!< 32bit register type
    eRegType_Last
} eRegType_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern uint8_t register_eeprom_array[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

BOOL        register_get            (uint8_t                uRegNo,
                                     eRegType_t*            peRegType,
                                     void*                  pvValue);

void        register_set_u8         (uint8_t                uRegNo,
                                     uint8_t                uValue);

void        register_set_u16        (uint8_t                uRegNo,
                                     uint16_t               uValue);

void        register_set_u32        (uint8_t                uRegNo,
                                     uint32_t               uValue);

void        register_do_command     (sBus_t*                psBus,
                                     uint8_t*               puMsg,
                                     uint8_t                uMsgLen,
                                     uint16_t               uSender);

void        register_send_u8        (sBus_t*                psBus,
                                     uint16_t               uReceiver,
                                     uint8_t                uRegNo,
                                     uint8_t                uValue);

void        register_send_u16       (sBus_t*                psBus,
                                     uint16_t               uReceiver,
                                     uint8_t                uRegNo,
                                     uint16_t               uValue);

void        register_send_u32       (sBus_t*                psBus,
                                     uint16_t               uReceiver,
                                     uint8_t                uRegNo,
                                     uint32_t               uValue);

BOOL        register_do_mapping     (uint16_t               uRemoteModuleAddr,
                                     uint8_t                uRemoteRegister,
                                     uint32_t               uValue);

#endif // _REGISTER_H_
/**
 * @}
 * @}
 */
