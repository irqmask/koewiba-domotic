/**
 * @addtogroup TESTEEPROM_ATTINY1634
 * @addtogroup TESTEEPROM_ATTINY1634_PCBCONFIG
 * @brief PCB configuration of the "testeeprom" application.
 *
 * @{
 * @file    pcbconfig.h
 * @brief   PCB configuration of the "testeeprom" application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _PCBCONFIG_H_
#define _PCBCONFIG_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

#define EEPROM_PCB_CONFIG  1
#define __25LC256__
#define EEP_CS_PORT        PORTB
#define EEP_CS_DDR         DDRB
#define EEP_CS             PB3

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif // _PCBCONFIG_H_
/** @} */
/** @} */
