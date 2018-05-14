/**
 * @addtogroup BLINDCONTROL_5+4_ATMEGA328
 * @addtogroup APPVERSION
 * @brief Version information of "blindcontrol_5+4_atmega328p" project.
 *
 * @{
 * @file    appversion.c
 * @brief   Version information of "blindcontrol_5+4_atmega328p" project.
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
                           0xF0,0x00,   // board ID            (high byte, low byte)
                           1,           // board revision
                           0xF0,0x00,   // application ID      (high byte, low byte)
                           0,2,0};      // application version (major, minor, bugfix)

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
