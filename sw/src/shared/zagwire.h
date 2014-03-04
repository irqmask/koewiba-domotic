/**
 * @addtogroup ZAGWIRE
 * @brief Zagwire protocoll decoder for TSIC temperature sensors.
 *
 * Contains functions to send and receive data over the SPI interface. Two
 * versions exist: A blocking version which halts the application until the data
 * is transmitted and a non-blocking version, which does the transmission in
 * background. In this case optional start and end-callbacks are called at the
 * beginning and at the end of the transmission.
 *
 * Only use the non-blocking option if the CPU frequency is at least times 10
 * faster than the SPI frequency. Otherwise the control overhead takes more CPU
 * time than the transmission itself.
 *
 * Supports pcbconfig.h for individual PCB settings of the SPI pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    zagwire.h
 * @brief   Zagwire protocoll decoder for TSIC temperature sensors..
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _ZAGWIRE_H_
#define _ZAGWIRE_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "ucontroller.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection ZAGWIRE_PCBCONFIG
 * Configure ZAGWIRE Pinout.
 * @{
 */
#ifndef ZAGWIRE_PCBCONFIG
 #define ZAGWIRE_PCBCONFIG  1
 //! Defines the DDR of a port which contains the Zagwire enable signal.
 #define ZAGW_DDR_EN    DDRB
 //! Defines the DDR of a port which contains the Zagwire data signal.
 #define ZAGW_DDR_DATA  DDRB
 //! Defines the PORT which contains the Zagwire enable signal.
 #define ZAGW_PORT_EN   PORTB
 //! Defines the PORT which contains the Zagwire data signal.
 #define ZAGW_PORT_DATA PORTB
 //! Defines the input PORT which contains the Zagwire data signal.
 #define ZAGW_PIN_DATA  PINB
 //! Defines the Zagwire enable pin (output).
 #define ZAGW_EN        PB7
 //! Defines the Zagwire data pin (input).
 #define ZAGW_DATA      PB4
#endif // ZAGWIRE_PCBCONFIG
/** @} */

/**
 * @subsection ZAGWIRE_APPCONFIG
 * Configure ZAGWIRE protocol decoder. E.g. blocking or IRQ functions.
 * @{
 */
#ifndef ZAGWIRE_APPCONFIG
 #define ZAGWIRE_APPCONFIG 1
#endif // ZAGWIRE_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

//! Zagwire status bits.
typedef enum zagw_status {
    ZAGW_eNewValue = 0,
} ZAGW_eStatus;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            ZAGW_vInit          (void);

void            ZAGW_vStartReception(void);

uint8_t         ZAGW_vRunReception  (void);

uint16_t        ZAGW_uGetData       (void);

void            ZAGW_vConvData2Temp (uint16_t               uData,
                                     uint8_t*               puTempInteger,
                                     uint8_t*               puTempTens);

#endif // _ZAGWIRE_H_
/** @} */
