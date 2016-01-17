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

// Note: The linker sorts the variables upside down. app_controller_id begins at 
//       the lowest address and app_version begins at the highest address.
const unsigned char app_versioninfo[MOD_LEN_CONTROLLERID + MOD_LEN_BOARDID + MOD_LEN_BOARDREV + MOD_LEN_APPID + MOD_LEN_APPVER] __attribute__((section(".versioninfo")))
                        = {SIGNATURE_0,SIGNATURE_1,SIGNATURE_2,0x00,
                           1,2,         // board ID
                           1,           // board revision
                           0,1,         // application ID
                           0,0};        // application version

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
