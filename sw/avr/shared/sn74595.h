/**
 * @addtogroup SN74595
 * @brief This module contains functions to control a 74595 latch.
 *
 * This module contains functions to control a 74595 latch using SPI0 and
 * additional latch and output-enable pins.
 *
 * @{
 * @file    sn74595.h
 * @brief   This module contains functions to control a 74595 latch.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _SN74595_H_
#define _SN74595_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void sn74595_initialize     (void);

void sn74595_latch          (void);

void sn74595_OE_on          (void);

void sn74595_OE_off         (void);

void sn74595_send           (uint8_t data);

#endif /* _SN74595_H_ */
/** @} */
