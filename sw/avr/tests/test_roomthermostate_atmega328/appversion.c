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
const unsigned char app_version[MOD_LEN_APPVER] __attribute__((section(".versioninfo"))) 
                        = {0,1};

const unsigned char app_id[MOD_LEN_APPID] __attribute__((section(".versioninfo"))) 
                        = {4,0x05};

const unsigned char app_board_rev[MOD_LEN_BOARDREV] __attribute__((section(".versioninfo"))) 
                        = {1};

const unsigned char app_board_id[MOD_LEN_BOARDID] __attribute__((section(".versioninfo"))) 
                        = {1,0x02};

const unsigned char app_controller_id[MOD_LEN_CONTROLLERID] __attribute__((section(".versioninfo"))) 
                        = {SIGNATURE_0,SIGNATURE_1,SIGNATURE_2,0x00};
                    
// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
