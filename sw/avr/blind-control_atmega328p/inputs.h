/**
 * @addtogroup INPUTS
 * @brief Public interface to detect the state of inputs.
 *
 * This module contains functions to detect the state of inputs.
 *
 * @{
 * @file    inputs.h
 * @brief   This module contains functions to detect the state of inputs.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _INPUTS_H_
#define _INPUTS_H_

// --- Include section ---------------------------------------------------------

#include <stdbool.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void input_initialize       (void);

bool input_up               (void);

bool input_down             (void);

bool input_window_closed    (void);

#endif /* _INPUTS_H_ */
/** @} */
