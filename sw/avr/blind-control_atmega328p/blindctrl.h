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
#include "bus.h"

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

void blind_background       (sBus_t* bus);

uint8_t blind_get_current_position  (void);

uint8_t blind_get_position_setpoint (void);

uint8_t blind_get_reaction_delay    (void);

void blind_set_reaction_delay       (uint8_t reaction_delay);

uint16_t blind_get_duration_open    (void);

void blind_set_duration_open        (uint16_t duration);

uint16_t blind_get_duration_close   (void);

void blind_set_duration_close       (uint16_t duration);

#endif /* _BLINDCTRL_H_ */
/** @} */