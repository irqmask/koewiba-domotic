/**
 * @addtogroup PUSHBUTTON8_APPCONFIG
 * @brief Configuration of modules used in "pushbutton8" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "pushbutton8" project.
 *
 * @author  Christian Verhalen, Robert Mueller
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

#define APP_NUM_KEYS        8       //!< The keys application supports eight keys.
#define APP_NUM_KEY_STUBS   8       //!< The keys application supports eight keys.
#define APP_NUM_MSG_STUBS  16       //!< Number of message receiver stubs

//! Application specific layout of registers
typedef enum appregisters {
    // registers that must be filled with data saved in EEProm
    APP_eReg_K0_Mode = MOD_eReg_FirstAppSpecific, //!< Key mode
    APP_eReg_K0_RemoteAddr,         //!< Receiver's address
    APP_eReg_K0_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K0_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K0_Reserved3,          //!< unused
    APP_eReg_K0_Reserved2,          //!< unused
    APP_eReg_K0_Reserved1,          //!< unused
    APP_eReg_K0_Reserved0,          //!< unused
    APP_eReg_K1_Mode,               //!< Key mode function
    APP_eReg_K1_RemoteAddr,         //!< Receiver's address
    APP_eReg_K1_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K1_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K1_Reserved3,          //!< unused
    APP_eReg_K1_Reserved1,          //!< unused
    APP_eReg_K1_Reserved2,          //!< unused
    APP_eReg_K1_Reserved0,          //!< unused
    APP_eReg_K2_Mode,               //!< Key mode function
    APP_eReg_K2_RemoteAddr,         //!< Receiver's address
    APP_eReg_K2_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K2_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K2_Reserved3,          //!< unused
    APP_eReg_K2_Reserved2,          //!< unused
    APP_eReg_K2_Reserved1,          //!< unused
    APP_eReg_K2_Reserved0,          //!< unused
    APP_eReg_K3_Mode,               //!< Key mode function
    APP_eReg_K3_RemoteAddr,         //!< Receiver's address
    APP_eReg_K3_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K3_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K3_Reserved3,          //!< unused
    APP_eReg_K3_Reserved2,          //!< unused
    APP_eReg_K3_Reserved1,          //!< unused
    APP_eReg_K3_Reserved0,          //!< unused
    APP_eReg_K4_Mode,               //!< Key mode function
    APP_eReg_K4_RemoteAddr,         //!< Receiver's address
    APP_eReg_K4_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K4_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K4_Reserved3,          //!< unused
    APP_eReg_K4_Reserved2,          //!< unused
    APP_eReg_K4_Reserved1,          //!< unused
    APP_eReg_K4_Reserved0,          //!< unused
    APP_eReg_K5_Mode,               //!< Key mode function
    APP_eReg_K5_RemoteAddr,         //!< Receiver's address
    APP_eReg_K5_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K5_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K5_Reserved3,          //!< unused
    APP_eReg_K5_Reserved2,          //!< unused
    APP_eReg_K5_Reserved1,          //!< unused
    APP_eReg_K5_Reserved0,          //!< unused
    APP_eReg_K6_Mode,               //!< Key mode function
    APP_eReg_K6_RemoteAddr,         //!< Receiver's address
    APP_eReg_K6_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K6_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K6_Reserved3,          //!< unused
    APP_eReg_K6_Reserved2,          //!< unused
    APP_eReg_K6_Reserved1,          //!< unused
    APP_eReg_K6_Reserved0,          //!< unused
    APP_eReg_K7_Mode,               //!< Key mode function
    APP_eReg_K7_RemoteAddr,         //!< Receiver's address
    APP_eReg_K7_RemoteReg,          //!< Receiver's register to set
    APP_eReg_K7_Value,              //!< Value to set (not used in all modes)
    APP_eReg_K7_Reserved3,          //!< unused
    APP_eReg_K7_Reserved2,          //!< unused
    APP_eReg_K7_Reserved1,          //!< unused
    APP_eReg_K7_Reserved0,          //!< unused
    APP_eReg_RecMsg0_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg0_Command,       //!< Message command
    APP_eReg_RecMsg0_Register,      //!< Messages Register number
    APP_eReg_RecMsg0_LEDFunc,       //!< LED function
    APP_eReg_RecMsg0_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg1_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg1_Command,       //!< Message command
    APP_eReg_RecMsg1_Register,      //!< Messages Register number
    APP_eReg_RecMsg1_LEDFunc,       //!< LED function
    APP_eReg_RecMsg1_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg2_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg2_Command,       //!< Message command
    APP_eReg_RecMsg2_Register,      //!< Messages Register number
    APP_eReg_RecMsg2_LEDFunc,       //!< LED function
    APP_eReg_RecMsg2_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg3_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg3_Command,       //!< Message command
    APP_eReg_RecMsg3_Register,      //!< Messages Register number
    APP_eReg_RecMsg3_LEDFunc,       //!< LED function
    APP_eReg_RecMsg3_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg4_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg4_Command,       //!< Message command
    APP_eReg_RecMsg4_Register,      //!< Messages Register number
    APP_eReg_RecMsg4_LEDFunc,       //!< LED function
    APP_eReg_RecMsg4_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg5_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg5_Command,       //!< Message command
    APP_eReg_RecMsg5_Register,      //!< Messages Register number
    APP_eReg_RecMsg5_LEDFunc,       //!< LED function
    APP_eReg_RecMsg5_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg6_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg6_Command,       //!< Message command
    APP_eReg_RecMsg6_Register,      //!< Messages Register number
    APP_eReg_RecMsg6_LEDFunc,       //!< LED function
    APP_eReg_RecMsg6_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg7_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg7_Command,       //!< Message command
    APP_eReg_RecMsg7_Register,      //!< Messages Register number
    APP_eReg_RecMsg7_LEDFunc,       //!< LED function
    APP_eReg_RecMsg7_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg8_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg8_Command,       //!< Message command
    APP_eReg_RecMsg8_Register,      //!< Messages Register number
    APP_eReg_RecMsg8_LEDFunc,       //!< LED function
    APP_eReg_RecMsg8_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg9_RemoteAddr,    //!< Sender's address
    APP_eReg_RecMsg9_Command,       //!< Message command
    APP_eReg_RecMsg9_Register,      //!< Messages Register number
    APP_eReg_RecMsg9_LEDFunc,       //!< LED function
    APP_eReg_RecMsg9_AddInfo,       //!< Function arguments
    APP_eReg_RecMsg10_RemoteAddr,   //!< Sender's address
    APP_eReg_RecMsg10_Command,      //!< Message command
    APP_eReg_RecMsg10_Register,     //!< Messages Register number
    APP_eReg_RecMsg10_LEDFunc,      //!< LED function
    APP_eReg_RecMsg10_AddInfo,      //!< Function arguments
    APP_eReg_RecMsg11_RemoteAddr,   //!< Sender's address
    APP_eReg_RecMsg11_Command,      //!< Message command
    APP_eReg_RecMsg11_Register,     //!< Messages Register number
    APP_eReg_RecMsg11_LEDFunc,      //!< LED function
    APP_eReg_RecMsg11_AddInfo,      //!< Function arguments
    APP_eReg_RecMsg12_RemoteAddr,   //!< Sender's address
    APP_eReg_RecMsg12_Command,      //!< Message command
    APP_eReg_RecMsg12_Register,     //!< Messages Register number
    APP_eReg_RecMsg12_LEDFunc,      //!< LED function
    APP_eReg_RecMsg12_AddInfo,      //!< Function arguments
    APP_eReg_RecMsg13_RemoteAddr,   //!< Sender's address
    APP_eReg_RecMsg13_Command,      //!< Message command
    APP_eReg_RecMsg13_Register,     //!< Messages Register number
    APP_eReg_RecMsg13_LEDFunc,      //!< LED function
    APP_eReg_RecMsg13_AddInfo,      //!< Function arguments
    APP_eReg_RecMsg14_RemoteAddr,   //!< Sender's address
    APP_eReg_RecMsg14_Command,      //!< Message command
    APP_eReg_RecMsg14_Register,     //!< Messages Register number
    APP_eReg_RecMsg14_LEDFunc,      //!< LED function
    APP_eReg_RecMsg14_AddInfo,      //!< Function arguments
    APP_eReg_RecMsg15_RemoteAddr,   //!< Sender's address
    APP_eReg_RecMsg15_Command,      //!< Message command
    APP_eReg_RecMsg15_Register,     //!< Messages Register number
    APP_eReg_RecMsg15_LEDFunc,      //!< LED function
    APP_eReg_RecMsg15_AddInfo,      //!< Function arguments
    // registers in ROM/RAM
    APP_eReg_0_LEDState,            //!< Current local LED state (on/off/blink)
    APP_eReg_1_LEDState,            //!< Current local LED state (on/off/blink)
    APP_eReg_2_LEDState,            //!< Current local LED state (on/off/blink)
    APP_eReg_3_LEDState,            //!< Current local LED state (on/off/blink)
    APP_eReg_4_LEDState,            //!< Current local LED state (on/off/blink)
    APP_eReg_5_LEDState,            //!< Current local LED state (on/off/blink)
    APP_eReg_6_LEDState,            //!< Current local LED state (on/off/blink)
    APP_eReg_7_LEDState,            //!< Current local LED state (on/off/blink)

    APP_eReg_Last                //!< APP_eReg_Last
} eRegisters_t;

#define NUM_REGS_PER_KEY  (APP_eReg_K1_Mode - APP_eReg_K0_Mode)
#define NUM_REGS_PER_STUB (APP_eReg_RecMsg1_RemoteAddr - APP_eReg_RecMsg0_RemoteAddr)

//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_K0_Mode = MOD_eCfg_FirstAppSpecific, //!< Key mode
    APP_eCfg_K0_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K0_RemoteAddr_Low = APP_eCfg_K0_RemoteAddr,
    APP_eCfg_K0_RemoteAddr_High,
    APP_eCfg_K0_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K0_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K0_Reserved3,      //!< unused
    APP_eCfg_K0_Reserved2,      //!< unused
    APP_eCfg_K0_Reserved1,      //!< unused
    APP_eCfg_K0_Reserved0,      //!< unused
    APP_eCfg_K1_Mode,           //!< Key mode
    APP_eCfg_K1_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K1_RemoteAddr_Low = APP_eCfg_K1_RemoteAddr,
    APP_eCfg_K1_RemoteAddr_High,
    APP_eCfg_K1_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K1_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K1_Reserved3,      //!< unused
    APP_eCfg_K1_Reserved2,      //!< unused
    APP_eCfg_K1_Reserved1,      //!< unused
    APP_eCfg_K1_Reserved0,      //!< unused
    APP_eCfg_K2_Mode,           //!< Key mode
    APP_eCfg_K2_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K2_RemoteAddr_Low = APP_eCfg_K2_RemoteAddr,
    APP_eCfg_K2_RemoteAddr_High,
    APP_eCfg_K2_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K2_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K2_Reserved3,      //!< unused
    APP_eCfg_K2_Reserved2,      //!< unused
    APP_eCfg_K2_Reserved1,      //!< unused
    APP_eCfg_K2_Reserved0,      //!< unused
    APP_eCfg_K3_Mode,           //!< Key mode
    APP_eCfg_K3_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K3_RemoteAddr_Low = APP_eCfg_K3_RemoteAddr,
    APP_eCfg_K3_RemoteAddr_High,
    APP_eCfg_K3_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K3_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K3_Reserved3,      //!< unused
    APP_eCfg_K3_Reserved2,      //!< unused
    APP_eCfg_K3_Reserved1,      //!< unused
    APP_eCfg_K3_Reserved0,      //!< unused
    APP_eCfg_K4_Mode,           //!< Key mode
    APP_eCfg_K4_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K4_RemoteAddr_Low = APP_eCfg_K4_RemoteAddr,
    APP_eCfg_K4_RemoteAddr_High,
    APP_eCfg_K4_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K4_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K4_Reserved3,      //!< unused
    APP_eCfg_K4_Reserved2,      //!< unused
    APP_eCfg_K4_Reserved1,      //!< unused
    APP_eCfg_K4_Reserved0,      //!< unused
    APP_eCfg_K5_Mode,           //!< Key mode
    APP_eCfg_K5_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K5_RemoteAddr_Low = APP_eCfg_K5_RemoteAddr,
    APP_eCfg_K5_RemoteAddr_High,
    APP_eCfg_K5_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K5_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K5_Reserved3,      //!< unused
    APP_eCfg_K5_Reserved2,      //!< unused
    APP_eCfg_K5_Reserved1,      //!< unused
    APP_eCfg_K5_Reserved0,      //!< unused
    APP_eCfg_K6_Mode,           //!< Key mode
    APP_eCfg_K6_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K6_RemoteAddr_Low = APP_eCfg_K6_RemoteAddr,
    APP_eCfg_K6_RemoteAddr_High,
    APP_eCfg_K6_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K6_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K6_Reserved3,      //!< unused
    APP_eCfg_K6_Reserved2,      //!< unused
    APP_eCfg_K6_Reserved1,      //!< unused
    APP_eCfg_K6_Reserved0,      //!< unused
    APP_eCfg_K7_Mode,           //!< Key mode
    APP_eCfg_K7_RemoteAddr,     //!< Receiver's address
    APP_eCfg_K7_RemoteAddr_Low = APP_eCfg_K7_RemoteAddr,
    APP_eCfg_K7_RemoteAddr_High,
    APP_eCfg_K7_RemoteReg,      //!< Receiver's register to set
    APP_eCfg_K7_Value,          //!< Value to set (not used in all modes)
    APP_eCfg_K7_Reserved3,      //!< unused
    APP_eCfg_K7_Reserved2,      //!< unused
    APP_eCfg_K7_Reserved1,      //!< unused
    APP_eCfg_K7_Reserved0,      //!< unused
    APP_eCfg_RecMsg0_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg0_RemoteAddr_Low = APP_eCfg_RecMsg0_RemoteAddr,
    APP_eCfg_RecMsg0_RemoteAddr_High,
    APP_eCfg_RecMsg0_Command,    //!< Message command
    APP_eCfg_RecMsg0_Register,   //!< Messages Register number
    APP_eCfg_RecMsg0_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg0_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg1_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg1_RemoteAddr_Low = APP_eCfg_RecMsg1_RemoteAddr,
    APP_eCfg_RecMsg1_RemoteAddr_High,
    APP_eCfg_RecMsg1_Command,    //!< Message command
    APP_eCfg_RecMsg1_Register,   //!< Messages Register number
    APP_eCfg_RecMsg1_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg1_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg2_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg2_RemoteAddr_Low = APP_eCfg_RecMsg2_RemoteAddr,
    APP_eCfg_RecMsg2_RemoteAddr_High,
    APP_eCfg_RecMsg2_Command,    //!< Message command
    APP_eCfg_RecMsg2_Register,   //!< Messages Register number
    APP_eCfg_RecMsg2_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg2_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg3_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg3_RemoteAddr_Low = APP_eCfg_RecMsg3_RemoteAddr,
    APP_eCfg_RecMsg3_RemoteAddr_High,
    APP_eCfg_RecMsg3_Command,    //!< Message command
    APP_eCfg_RecMsg3_Register,   //!< Messages Register number
    APP_eCfg_RecMsg3_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg3_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg4_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg4_RemoteAddr_Low = APP_eCfg_RecMsg4_RemoteAddr,
    APP_eCfg_RecMsg4_RemoteAddr_High,
    APP_eCfg_RecMsg4_Command,    //!< Message command
    APP_eCfg_RecMsg4_Register,   //!< Messages Register number
    APP_eCfg_RecMsg4_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg4_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg5_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg5_RemoteAddr_Low = APP_eCfg_RecMsg5_RemoteAddr,
    APP_eCfg_RecMsg5_RemoteAddr_High,
    APP_eCfg_RecMsg5_Command,    //!< Message command
    APP_eCfg_RecMsg5_Register,   //!< Messages Register number
    APP_eCfg_RecMsg5_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg5_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg6_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg6_RemoteAddr_Low = APP_eCfg_RecMsg6_RemoteAddr,
    APP_eCfg_RecMsg6_RemoteAddr_High,
    APP_eCfg_RecMsg6_Command,    //!< Message command
    APP_eCfg_RecMsg6_Register,   //!< Messages Register number
    APP_eCfg_RecMsg6_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg6_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg7_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg7_RemoteAddr_Low = APP_eCfg_RecMsg7_RemoteAddr,
    APP_eCfg_RecMsg7_RemoteAddr_High,
    APP_eCfg_RecMsg7_Command,    //!< Message command
    APP_eCfg_RecMsg7_Register,   //!< Messages Register number
    APP_eCfg_RecMsg7_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg7_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg8_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg8_RemoteAddr_Low = APP_eCfg_RecMsg8_RemoteAddr,
    APP_eCfg_RecMsg8_RemoteAddr_High,
    APP_eCfg_RecMsg8_Command,    //!< Message command
    APP_eCfg_RecMsg8_Register,   //!< Messages Register number
    APP_eCfg_RecMsg8_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg8_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg9_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg9_RemoteAddr_Low = APP_eCfg_RecMsg9_RemoteAddr,
    APP_eCfg_RecMsg9_RemoteAddr_High,
    APP_eCfg_RecMsg9_Command,    //!< Message command
    APP_eCfg_RecMsg9_Register,   //!< Messages Register number
    APP_eCfg_RecMsg9_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg9_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg10_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg10_RemoteAddr_Low = APP_eCfg_RecMsg10_RemoteAddr,
    APP_eCfg_RecMsg10_RemoteAddr_High,
    APP_eCfg_RecMsg10_Command,    //!< Message command
    APP_eCfg_RecMsg10_Register,   //!< Messages Register number
    APP_eCfg_RecMsg10_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg10_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg11_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg11_RemoteAddr_Low = APP_eCfg_RecMsg11_RemoteAddr,
    APP_eCfg_RecMsg11_RemoteAddr_High,
    APP_eCfg_RecMsg11_Command,    //!< Message command
    APP_eCfg_RecMsg11_Register,   //!< Messages Register number
    APP_eCfg_RecMsg11_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg11_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg12_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg12_RemoteAddr_Low = APP_eCfg_RecMsg12_RemoteAddr,
    APP_eCfg_RecMsg12_RemoteAddr_High,
    APP_eCfg_RecMsg12_Command,    //!< Message command
    APP_eCfg_RecMsg12_Register,   //!< Messages Register number
    APP_eCfg_RecMsg12_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg12_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg13_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg13_RemoteAddr_Low = APP_eCfg_RecMsg13_RemoteAddr,
    APP_eCfg_RecMsg13_RemoteAddr_High,
    APP_eCfg_RecMsg13_Command,    //!< Message command
    APP_eCfg_RecMsg13_Register,   //!< Messages Register number
    APP_eCfg_RecMsg13_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg13_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg14_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg14_RemoteAddr_Low = APP_eCfg_RecMsg14_RemoteAddr,
    APP_eCfg_RecMsg14_RemoteAddr_High,
    APP_eCfg_RecMsg14_Command,    //!< Message command
    APP_eCfg_RecMsg14_Register,   //!< Messages Register number
    APP_eCfg_RecMsg14_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg14_AddInfo,    //!< Function arguments
    APP_eCfg_RecMsg15_RemoteAddr, //!< Sender's address
    APP_eCfg_RecMsg15_RemoteAddr_Low = APP_eCfg_RecMsg15_RemoteAddr,
    APP_eCfg_RecMsg15_RemoteAddr_High,
    APP_eCfg_RecMsg15_Command,    //!< Message command
    APP_eCfg_RecMsg15_Register,   //!< Messages Register number
    APP_eCfg_RecMsg15_LEDFunc,    //!< LED function
    APP_eCfg_RecMsg15_AddInfo,    //!< Function arguments
    APP_eCfg_Last = APP_eCfg_RecMsg15_AddInfo
} eConfiguration_t;

typedef enum msg_dependend_functions {
    eMsgDepFunc_NoFunction = 0,
    eMsgDepFunc_Led_setting_command,
    eMsgDepFunc_Led_on_bitset,
    eMsgDepFunc_Led_off_bitset,
    eMsgDepFunc_Led_blink0_bitset,
    eMsgDepFunc_Led_blink1_bitset,
    eMsgDepFunc_Led_blink2_bitset,
    eMsgDepFunc_Led_blink3_bitset,
    eMsgDepFunc_Led_blindcontrol
} app_msg_dep_func_t;

#define NUM_EEBYTES_PER_KEY  (APP_eCfg_K1_Mode - APP_eCfg_K0_Mode)
#define NUM_EEBYTES_PER_STUB (APP_eCfg_RecMsg1_RemoteAddr - APP_eCfg_RecMsg0_RemoteAddr)

// --- Type definitions --------------------------------------------------------

typedef struct _app_led_and_mode {
    uint8_t     index;
    uint8_t     mode;
} app_led_and_mode_t;

typedef struct _app_on_msg_received {
    uint16_t    receiver;
    uint8_t     command;
    uint8_t     register_id;
    uint8_t     func_id;
    uint8_t     add_info;
} app_on_msg_received_t;

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
