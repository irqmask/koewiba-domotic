/**
 * @addtogroup PUSHBUTTON8_APPCONFIG
 * @brief Configuration of modules used in "pushbutton8" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "pushbutton8" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG       1
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

#define APP_NUM_KEYS        8   //!< The keys application supports eight keys.

//! Application specific layout of registers
typedef enum appregisters {
    // registers saved in EEProm
    APP_eReg_K0_Mode = MOD_eReg_FirstAppSpecific, //!< Key mode
    APP_eReg_K0_RemoteAddr,     //!< Receiver's address
    APP_eReg_K0_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K0_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K0_Reserved4,      //!< unused
    APP_eReg_K0_Reserved3,      //!< unused
    APP_eReg_K0_Reserved2,      //!< unused
    APP_eReg_K0_Reserved1,      //!< unused
    APP_eReg_K1_Mode,           //!< Key mode
    APP_eReg_K1_RemoteAddr,     //!< Receiver's address
    APP_eReg_K1_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K1_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K1_Reserved4,      //!< unused
    APP_eReg_K1_Reserved3,      //!< unused
    APP_eReg_K1_Reserved2,      //!< unused
    APP_eReg_K1_Reserved1,      //!< unused
    APP_eReg_K2_Mode,           //!< Key mode
    APP_eReg_K2_RemoteAddr,     //!< Receiver's address
    APP_eReg_K2_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K2_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K2_Reserved4,      //!< unused
    APP_eReg_K2_Reserved3,      //!< unused
    APP_eReg_K2_Reserved2,      //!< unused
    APP_eReg_K2_Reserved1,      //!< unused
    APP_eReg_K3_Mode,           //!< Key mode
    APP_eReg_K3_RemoteAddr,     //!< Receiver's address
    APP_eReg_K3_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K3_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K3_Reserved4,      //!< unused
    APP_eReg_K3_Reserved3,      //!< unused
    APP_eReg_K3_Reserved2,      //!< unused
    APP_eReg_K3_Reserved1,      //!< unused
    APP_eReg_K4_Mode,           //!< Key mode
    APP_eReg_K4_RemoteAddr,     //!< Receiver's address
    APP_eReg_K4_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K4_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K4_Reserved4,      //!< unused
    APP_eReg_K4_Reserved3,      //!< unused
    APP_eReg_K4_Reserved2,      //!< unused
    APP_eReg_K4_Reserved1,      //!< unused
    APP_eReg_K5_Mode,           //!< Key mode
    APP_eReg_K5_RemoteAddr,     //!< Receiver's address
    APP_eReg_K5_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K5_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K5_Reserved4,      //!< unused
    APP_eReg_K5_Reserved3,      //!< unused
    APP_eReg_K5_Reserved2,      //!< unused
    APP_eReg_K5_Reserved1,      //!< unused
    APP_eReg_K6_Mode,           //!< Key mode
    APP_eReg_K6_RemoteAddr,     //!< Receiver's address
    APP_eReg_K6_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K6_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K6_Reserved4,      //!< unused
    APP_eReg_K6_Reserved3,      //!< unused
    APP_eReg_K6_Reserved2,      //!< unused
    APP_eReg_K6_Reserved1,      //!< unused
    APP_eReg_K7_Mode,           //!< Key mode
    APP_eReg_K7_RemoteAddr,     //!< Receiver's address
    APP_eReg_K7_RemoteReg,      //!< Receiver's register to set
    APP_eReg_K7_Value,          //!< Value to set (not used in all modes)
    APP_eReg_K7_Reserved4,      //!< unused
    APP_eReg_K7_Reserved3,      //!< unused
    APP_eReg_K7_Reserved2,      //!< unused
    APP_eReg_K7_Reserved1,      //!< unused

    // registers in ROM/RAM
    APP_eReg_0_LEDState,        //!< Current local LED state (on/off/blink)
    APP_eReg_1_LEDState,        //!< Current local LED state (on/off/blink)
    APP_eReg_2_LEDState,        //!< Current local LED state (on/off/blink)
    APP_eReg_3_LEDState,        //!< Current local LED state (on/off/blink)
    APP_eReg_4_LEDState,        //!< Current local LED state (on/off/blink)
    APP_eReg_5_LEDState,        //!< Current local LED state (on/off/blink)
    APP_eReg_6_LEDState,        //!< Current local LED state (on/off/blink)
    APP_eReg_7_LEDState,        //!< Current local LED state (on/off/blink)

    APP_eReg_Last
} eRegisters_t;

#define NUM_REGS_PER_KEY (APP_eReg_K1_Mode - APP_eReg_K0_Mode)

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_K0_Mode = MOD_eCfg_FirstAppSpecific, //!< Key mode
    APP_eCfg_K0_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K0_RemoteAddr_Low = APP_eCfg_K0_RemoteAddr,
    APP_eCfg_K0_RemoteAddr_High,
    APP_eCfg_K0_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K0_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K0_Reserved4,      //!< unused
    APP_eCfg_K0_Reserved3,      //!< unused
    APP_eCfg_K0_Reserved2,      //!< unused
    APP_eCfg_K0_Reserved1,      //!< unused
    APP_eCfg_K1_Mode,           //!< Key mode
    APP_eCfg_K1_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K1_RemoteAddr_Low = APP_eCfg_K1_RemoteAddr,
    APP_eCfg_K1_RemoteAddr_High,
    APP_eCfg_K1_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K1_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K1_Reserved4,      //!< unused
    APP_eCfg_K1_Reserved3,      //!< unused
    APP_eCfg_K1_Reserved2,      //!< unused
    APP_eCfg_K1_Reserved1,      //!< unused
    APP_eCfg_K2_Mode,           //!< Key mode
    APP_eCfg_K2_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K2_RemoteAddr_Low = APP_eCfg_K2_RemoteAddr,
    APP_eCfg_K2_RemoteAddr_High,
    APP_eCfg_K2_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K2_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K2_Reserved4,      //!< unused
    APP_eCfg_K2_Reserved3,      //!< unused
    APP_eCfg_K2_Reserved2,      //!< unused
    APP_eCfg_K2_Reserved1,      //!< unused
    APP_eCfg_K3_Mode,           //!< Key mode
    APP_eCfg_K3_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K3_RemoteAddr_Low = APP_eCfg_K3_RemoteAddr,
    APP_eCfg_K3_RemoteAddr_High,
    APP_eCfg_K3_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K3_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K3_Reserved4,      //!< unused
    APP_eCfg_K3_Reserved3,      //!< unused
    APP_eCfg_K3_Reserved2,      //!< unused
    APP_eCfg_K3_Reserved1,      //!< unused
    APP_eCfg_K4_Mode,           //!< Key mode
    APP_eCfg_K4_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K4_RemoteAddr_Low = APP_eCfg_K4_RemoteAddr,
    APP_eCfg_K4_RemoteAddr_High,
    APP_eCfg_K4_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K4_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K4_Reserved4,      //!< unused
    APP_eCfg_K4_Reserved3,      //!< unused
    APP_eCfg_K4_Reserved2,      //!< unused
    APP_eCfg_K4_Reserved1,      //!< unused
    APP_eCfg_K5_Mode,           //!< Key mode
    APP_eCfg_K5_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K5_RemoteAddr_Low = APP_eCfg_K5_RemoteAddr,
    APP_eCfg_K5_RemoteAddr_High,
    APP_eCfg_K5_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K5_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K5_Reserved4,      //!< unused
    APP_eCfg_K5_Reserved3,      //!< unused
    APP_eCfg_K5_Reserved2,      //!< unused
    APP_eCfg_K5_Reserved1,      //!< unused
    APP_eCfg_K6_Mode,           //!< Key mode
    APP_eCfg_K6_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K6_RemoteAddr_Low = APP_eCfg_K6_RemoteAddr,
    APP_eCfg_K6_RemoteAddr_High,
    APP_eCfg_K6_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K6_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K6_Reserved4,      //!< unused
    APP_eCfg_K6_Reserved3,      //!< unused
    APP_eCfg_K6_Reserved2,      //!< unused
    APP_eCfg_K6_Reserved1,      //!< unused
    APP_eCfg_K7_Mode,           //!< Key mode
    APP_eCfg_K7_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K7_RemoteAddr_Low = APP_eCfg_K7_RemoteAddr,
    APP_eCfg_K7_RemoteAddr_High,
    APP_eCfg_K7_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K7_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K7_Reserved4,      //!< unused
    APP_eCfg_K7_Reserved3,      //!< unused
    APP_eCfg_K7_Reserved2,      //!< unused
    APP_eCfg_K7_Reserved1,      //!< unused

    APP_eCfg_Last
} eConfiguration_t;

#define NUM_EEBYTES_PER_KEY (APP_eCfg_K1_Mode - APP_eCfg_K0_Mode)

// --- Type definitions --------------------------------------------------------

typedef struct _app_on_key_set_register {
    uint8_t     key_mode;
    uint16_t    receiver;
    uint8_t     register_id;
    uint8_t     value;
} app_on_key_set_register_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _APPCONFIG_H_ */
/** @} */
