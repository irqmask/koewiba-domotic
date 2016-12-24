/**
 * @addtogroup BLINDCTRL
 * @brief Public interface of blind control.
 *
 * This module contains functions to control the blind. It makes use of the 
 * motor module and controls movement by timing.
 *
 * @{
 * @file    blindctrl.h
 * @brief   This module contains functions to control the blind.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _BLINDCTRL_H_
#define _BLINDCTRL_H_

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

void blind_initialize       (void);

void blind_move_to_position (uint8_t position);

void blind_stop             (void);

void blind_background       (void);

#endif /* _BLINDCTRL_H_ */
/** @} */
