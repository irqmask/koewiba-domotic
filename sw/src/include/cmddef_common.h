/**
 * @addtogroup CMDDEF_COMMON
 * @brief List of common command bytes.
 *
 * @{
 * @file    cmddef_common.h
 * @brief   List of common command bytes.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _CMDDEF_COMMON_H_
#define _CMDDEF_COMMON_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! Common defined commands.
typedef enum {
    CMD_eStateTypeless          = 0x00, //!< Send typeless state data.
    CMD_eStateBitfield          = 0x01, //!< Send bitfield state data.
    CMD_eState8bit              = 0x02, //!< Send 8bit state data.
    CMD_eState16bit             = 0x03, //!< Send 16bit state data.
    CMD_eState32bit             = 0x04, //!< Send 32bit state data.
    CMD_eStateDateTime          = 0x05, //!< Send date/time data.

    CMD_eRequestRegister        = 0x10, //!< Request register value.
    CMD_eSetRegister8bit        = 0x12, //!< Set 8bit register value.
    CMD_eSetRegister16bit       = 0x13, //!< Set 16bit register value.
    CMD_eSetRegister32bit       = 0x14, //!< Set 32bit register value.

    CMD_eFirstAppSpecific       = 0x40, //!< First application specific command.

    CMD_eAck                    = 0xF0, //!< Acknowledge message.
    CMD_eSleep                  = 0xF5, //!< Send module into sleep mode.
    CMD_eReset                  = 0xFE, //!< Reset module.

} CMD_eCommon_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _CMDDEF_COMMON_H_ */
/** @} */
