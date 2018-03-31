/**
 * @addtogroup ELEKTOR_BUS-NODE_ATMEGA88
 * @addtogroup ELEKTOR_BUS-NODE_ATMEGA88_APPVERSION
 * @brief Version information of "elektor_bus-node_atmega88" project.
 *
 * @{
 * @file    appversion.c
 * @brief   Version information of "elektor_bus-node_atmega88" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

//! Application's version information
//! @note The linker sorts the variables upside down. app_controller_id begins at
//!       the lowest address and app_version begins at the highest address.
//! @see BOARD_IDs, APP_IDs, APP_VERSIONING
const unsigned char app_versioninfo[MOD_LEN_CONTROLLERID + MOD_LEN_BOARDID + MOD_LEN_BOARDREV + MOD_LEN_APPID + MOD_LEN_APPVER] // not for atmega88 // __attribute__((section(".versioninfo")))
                        = {SIGNATURE_0,SIGNATURE_1,SIGNATURE_2,0x00,
                           0xFF,0xFE,   // board ID
                           1,           // board revision
                           0xFF,0xFE,   // application ID
                           0,1,2};      // application version

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
