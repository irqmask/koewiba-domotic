/**
 * @addtogroup ROOMTHERMOSTATE
 * @addtogroup APPREGISTER
 * @brief Channels and registers of application roomthermostate.
 *
 * Contains access functions for channels and registers for different data
 * types.
 *
 * @{
 * @file    appchannelregister.h
 * @brief   Channels and registers of application roomthermostate.
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

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

uint8_t         REG_uGetU8Register      (uint8_t                uRegNo);

uint16_t        REG_uGetU16Register     (uint8_t                uRegNo);

uint32_t        REG_uGetU32Register     (uint8_t                uRegNo);

void            REG_vSetU8Register      (uint8_t                uRegNo,
                                         uint8_t                uValue);

void            REG_vSetU16Register     (uint8_t                uRegNo,
                                         uint16_t               uValue);

void            REG_vSetU32Register     (uint8_t                uRegNo,
                                         uint32_t               uValue);

void            REG_vDoCommand          (sBus_t*                psBus,
                                         uint8_t*               puMsg,
                                         uint8_t                uMsgLen,
                                         uint16_t               uSender);

void            REG_vSendStateU8        (sBus_t*                psBus,
                                         uint16_t               uReceiver,
                                         uint8_t                uRegNo,
                                         uint8_t                uValue);

void            REG_vSendStateU16       (sBus_t*                psBus,
                                         uint16_t               uReceiver,
                                         uint8_t                uRegNo,
                                         uint16_t               uValue);

void            REG_vSendStateU32       (sBus_t*                psBus,
                                         uint16_t               uReceiver,
                                         uint8_t                uRegNo,
                                         uint32_t               uValue);
#endif // _REGISTER_H_
/**
 * @}
 * @}
 */
