/**
 * @addtogroup TEST_BUS-MODULE_ATTINY1634
 * @addtogroup TEST_BUS-MODULE_ATTINY1634_APPVERSION
 * @brief Version information of "test_bus-module_attiny1634" project.
 *
 * @{
 * @file    appversion.c
 * @brief   Version information of "test_bus-module_attiny1634" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include "moddef_common.h"
#include "version.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

//! Application's version information
//! @note The linker sorts the variables upside down. app_controller_id begins at
//!       the lowest address and app_version begins at the highest address.
//! @see BOARD_IDs, APP_IDs, APP_VERSIONING
const unsigned char app_versioninfo[MOD_VERSIONINFO_LEN] __attribute__((section(".versioninfo")))
                        = {SIGNATURE_0,SIGNATURE_1,SIGNATURE_2,0x00,
                           0x00,0x03,   // board ID
                           1,           // board revision
                           0xEF,0xFD,   // application ID
                           VERSION_MAJOR, VERSION_MINOR, VERSION_BUGFIX, // application version (major, minor, bugfix)
                            (VERSION_HASH & 0x000000FF),        // version hash
                           ((VERSION_HASH & 0x0000FF00) >> 8),
                           ((VERSION_HASH & 0x00FF0000) >> 16),
                           ((VERSION_HASH & 0xFF000000) >> 24)
};
                    
// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
