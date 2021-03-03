/**
 * @addtogroup INPUT
 * @brief This module contains functions to read, debounce and evaluate input 
 * signals.
 *
 * @{
 * @file    input.h
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _INPUT_H_
#define _INPUT_H_

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

/**
 * Initialize input ports.
 */
void input_initialize(void);

/**
 * Input background loop
 * This function performs actual input reading and debouncing.
 */
void input_background(void);

/**
 * Get current input state.
 * @returns current state of inputs
 */
uint8_t input_state(void);

/**
 * Get inputs where inputs went high.
 * @returns rising edges on inputs
 */
uint8_t input_went_high(void);

/**
 * Get inputs where inputs went low.
 * @returns falling edges on inputs
 */
uint8_t input_went_low(void);

/**
 * Get inputs where inputs has changed.
 * @returns toggled inputs
 */
uint8_t input_toggled(void);

#endif // _INPUT_H_
/** @} */
