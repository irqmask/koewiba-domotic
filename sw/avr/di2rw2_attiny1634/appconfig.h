/**
 * @addtogroup DI2RW2_ATTINY1634
 * @addtogroup APPCONFIG
 * @brief Configuration of modules used in "di2rw2_attiny1634" project.
 *
 * @{
 * @file    appconfig.h
 * @brief   Configuration of modules used in "di2rw2_attiny1634" project.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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
#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

#define BUS_APPCONFIG   1
#undef BUS_SCHEDULER                //!< This program has no scheduling capabilities.
#define BUS_TX_QUEUE_SIZE  100      //!< Size in bytes of the message transmit queue.
#define BUS_MAXSENDMSGLEN   16      //!< Maximum length of a message to be sent.
#define BUS_MAXRECVMSGLEN   64 + 2  //!< Maximum length of a message to be received.

#define APP_NUM_KEYS        2   //!< The keys application supports eight keys.


//! Application specific layout of registers
typedef enum appregisters {
    // registers in ROM/RAM
    APP_eReg_InputState_Bits = MOD_eReg_FirstAppSpecific, //!< Current input state bits (In8|In7|...|In1)
    APP_eReg_OutputState_Bits,  //!< Current Output state bits (Out8|Out7|Out6|Out5|Out4|Out3|Out2|Out1)
    APP_eReg_Output1_State,     //!< State of Output 1
    APP_eReg_Output2_State,     //!< State of Output 2
    APP_eReg_Output3_State,     //!< State of Output 3
    APP_eReg_Output4_State,     //!< State of Output 4
    APP_eReg_Output5_State,     //!< State of Output 5
    APP_eReg_Output6_State,     //!< State of Output 6
    APP_eReg_Output7_State,     //!< State of Output 7
    APP_eReg_Output8_State,     //!< State of Output 8
    // registers saved in EEProm
    APP_eReg_Output1_Mode,      //!< Output 1 mode (on/off/delayed)
    APP_eReg_Output2_Mode,      //!< Output 2 mode (on/off/delayed)
    APP_eReg_Output3_Mode,      //!< Output 3 mode (on/off/delayed)
    APP_eReg_Output4_Mode,      //!< Output 4 mode (on/off/delayed)
    APP_eReg_Output5_Mode,      //!< Output 5 mode (on/off/delayed)
    APP_eReg_Output6_Mode,      //!< Output 6 mode (on/off/delayed)
    APP_eReg_Output7_Mode,      //!< Output 7 mode (on/off/delayed)
    APP_eReg_Output8_Mode,      //!< Output 8 mode (on/off/delayed)
    APP_eReg_In1_Mode,          //!< Key mode
    APP_eReg_In1_RemoteAddr,    //!< Receiver's address
    APP_eReg_In1_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In1_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In1_Reserved4,     //!< unused
    APP_eReg_In1_Reserved3,     //!< unused
    APP_eReg_In1_Reserved2,     //!< unused
    APP_eReg_In1_Reserved1,     //!< unused
    APP_eReg_In2_Mode,          //!< Key mode
    APP_eReg_In2_RemoteAddr,    //!< Receiver's address
    APP_eReg_In2_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In2_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In2_Reserved4,     //!< unused
    APP_eReg_In2_Reserved3,     //!< unused
    APP_eReg_In2_Reserved2,     //!< unused
    APP_eReg_In2_Reserved1,     //!< unused
    APP_eReg_In3_Mode,          //!< Key mode
    APP_eReg_In3_RemoteAddr,    //!< Receiver's address
    APP_eReg_In3_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In3_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In3_Reserved4,     //!< unused
    APP_eReg_In3_Reserved3,     //!< unused
    APP_eReg_In3_Reserved2,     //!< unused
    APP_eReg_In3_Reserved1,     //!< unused
    APP_eReg_In4_Mode,          //!< Key mode
    APP_eReg_In4_RemoteAddr,    //!< Receiver's address
    APP_eReg_In4_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In4_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In4_Reserved4,     //!< unused
    APP_eReg_In4_Reserved3,     //!< unused
    APP_eReg_In4_Reserved2,     //!< unused
    APP_eReg_In4_Reserved1,     //!< unused
    APP_eReg_In5_Mode,          //!< Key mode
    APP_eReg_In5_RemoteAddr,    //!< Receiver's address
    APP_eReg_In5_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In5_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In5_Reserved4,     //!< unused
    APP_eReg_In5_Reserved3,     //!< unused
    APP_eReg_In5_Reserved2,     //!< unused
    APP_eReg_In5_Reserved1,     //!< unused
    APP_eReg_In6_Mode,          //!< Key mode
    APP_eReg_In6_RemoteAddr,    //!< Receiver's address
    APP_eReg_In6_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In6_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In6_Reserved4,     //!< unused
    APP_eReg_In6_Reserved3,     //!< unused
    APP_eReg_In6_Reserved2,     //!< unused
    APP_eReg_In6_Reserved1,     //!< unused
    APP_eReg_In7_Mode,          //!< Key mode
    APP_eReg_In7_RemoteAddr,    //!< Receiver's address
    APP_eReg_In7_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In7_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In7_Reserved4,     //!< unused
    APP_eReg_In7_Reserved3,     //!< unused
    APP_eReg_In7_Reserved2,     //!< unused
    APP_eReg_In7_Reserved1,     //!< unused
    APP_eReg_In8_Mode,          //!< Key mode
    APP_eReg_In8_RemoteAddr,    //!< Receiver's address
    APP_eReg_In8_RemoteReg,     //!< Receiver's register to set
    APP_eReg_In8_Value,         //!< Value to set (not used in all modes)
    APP_eReg_In8_Reserved4,     //!< unused
    APP_eReg_In8_Reserved3,     //!< unused
    APP_eReg_In8_Reserved2,     //!< unused
    APP_eReg_In8_Reserved1,     //!< unused
    APP_eReg_Last,              //!< Value to set (not used in all modes)
} eRegisters_t;

#define APP_NUM_REGS_PER_INPUT (APP_eReg_In2_Mode - APP_eReg_In1_Mode)


//! Application specific layout of non volatile parameters (internal EEProm)
typedef enum appconfig {
    APP_eCfg_Output1_Mode = MOD_eCfg_FirstAppSpecific,
    APP_eCfg_Output2_Mode,      //!< Output 2 mode (on/off/delayed)
    APP_eCfg_Output3_Mode,      //!< Output 3 mode (on/off/delayed)
    APP_eCfg_Output4_Mode,      //!< Output 4 mode (on/off/delayed)
    APP_eCfg_Output5_Mode,      //!< Output 5 mode (on/off/delayed)
    APP_eCfg_Output6_Mode,      //!< Output 6 mode (on/off/delayed)
    APP_eCfg_Output7_Mode,      //!< Output 7 mode (on/off/delayed)
    APP_eCfg_Output8_Mode,      //!< Output 8 mode (on/off/delayed)
    APP_eCfg_In1_Mode,          //!< Key mode
    APP_eCfg_In1_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In1_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In1_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In1_Reserved4,     //!< unused
    APP_eCfg_In1_Reserved3,     //!< unused
    APP_eCfg_In1_Reserved2,     //!< unused
    APP_eCfg_In1_Reserved1,     //!< unused
    APP_eCfg_In2_Mode,          //!< Key mode
    APP_eCfg_In2_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In2_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In2_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In2_Reserved4,     //!< unused
    APP_eCfg_In2_Reserved3,     //!< unused
    APP_eCfg_In2_Reserved2,     //!< unused
    APP_eCfg_In2_Reserved1,     //!< unused
    APP_eCfg_In3_Mode,          //!< Key mode
    APP_eCfg_In3_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In3_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In3_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In3_Reserved4,     //!< unused
    APP_eCfg_In3_Reserved3,     //!< unused
    APP_eCfg_In3_Reserved2,     //!< unused
    APP_eCfg_In3_Reserved1,     //!< unused
    APP_eCfg_In4_Mode,          //!< Key mode
    APP_eCfg_In4_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In4_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In4_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In4_Reserved4,     //!< unused
    APP_eCfg_In4_Reserved3,     //!< unused
    APP_eCfg_In4_Reserved2,     //!< unused
    APP_eCfg_In4_Reserved1,     //!< unused
    APP_eCfg_In5_Mode,          //!< Key mode
    APP_eCfg_In5_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In5_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In5_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In5_Reserved4,     //!< unused
    APP_eCfg_In5_Reserved3,     //!< unused
    APP_eCfg_In5_Reserved2,     //!< unused
    APP_eCfg_In5_Reserved1,     //!< unused
    APP_eCfg_In6_Mode,          //!< Key mode
    APP_eCfg_In6_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In6_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In6_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In6_Reserved4,     //!< unused
    APP_eCfg_In6_Reserved3,     //!< unused
    APP_eCfg_In6_Reserved2,     //!< unused
    APP_eCfg_In6_Reserved1,     //!< unused
    APP_eCfg_In7_Mode,          //!< Key mode
    APP_eCfg_In7_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In7_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In7_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In7_Reserved4,     //!< unused
    APP_eCfg_In7_Reserved3,     //!< unused
    APP_eCfg_In7_Reserved2,     //!< unused
    APP_eCfg_In7_Reserved1,     //!< unused
    APP_eCfg_In8_Mode,          //!< Key mode
    APP_eCfg_In8_RemoteAddr,    //!< Receiver's address
    APP_eCfg_In8_RemoteReg,     //!< Receiver's register to set
    APP_eCfg_In8_Value,         //!< Value to set (not used in all modes)
    APP_eCfg_In8_Reserved4,     //!< unused
    APP_eCfg_In8_Reserved3,     //!< unused
    APP_eCfg_In8_Reserved2,     //!< unused
    APP_eCfg_In8_Reserved1,     //!< unused
    // insert application specific configuration here
    APP_eCfg_Last
} eConfiguration_t;

#define APP_NUM_EEBYTES_PER_INPUT (APP_eCfg_In2_Mode - APP_eCfg_In1_Mode)

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
