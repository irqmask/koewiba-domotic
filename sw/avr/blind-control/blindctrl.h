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


void blind_move_to_position         (uint8_t index, uint8_t position);

void blind_stop                     (uint8_t index);

uint8_t blind_get_current_position  (uint8_t index);

uint8_t blind_get_position_setpoint (uint8_t index);

uint8_t blind_get_reaction_delay    (uint8_t index);

void blind_set_reaction_delay       (uint8_t index, uint8_t reaction_delay);

uint16_t blind_get_duration_open    (uint8_t index);

void blind_set_duration_open        (uint8_t index, uint16_t duration);

uint16_t blind_get_duration_close   (uint8_t index);

void blind_set_duration_close       (uint8_t index, uint16_t duration);

void blind_set_mode                 (uint8_t index, uint8_t mode);

void blinds_initialize      (void);

void blinds_background      (sBus_t* bus);

bool blinds_are_moving      (void);

#endif /* _BLINDCTRL_H_ */
/** @} */
