/**
 * @addtogroup ROOMTHERMOSTATE_ATMEGA324
 * @addtogroup ROOMTHERMOSTATE_ATMEGA324_APPVERSION
 * @brief Version information of "roomthermostate" project.
 *
 * @{
 * @file    appversion.c
 * @brief   Version information of "roomthermostate" project.
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
const unsigned char app_versioninfo[MOD_LEN_CONTROLLERID + MOD_LEN_BOARDID + MOD_LEN_BOARDREV + MOD_LEN_APPID + MOD_LEN_APPVER] __attribute__((section(".versioninfo")))
                        = {SIGNATURE_0,SIGNATURE_1,SIGNATURE_2,0x00,
                           0xFF,0xFE,   // (not defined yet)board ID            (high byte, low byte)
                           2,           // board revision
                           0xFF,0xFE,   // (not defined yet)application ID      (high byte, low byte)
                           0,1,0};      // application version (major, minor, bugfix)
                    
// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
