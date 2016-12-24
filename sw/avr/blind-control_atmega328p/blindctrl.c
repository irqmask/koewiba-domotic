/**
 * @addtogroup BLINDCTRL
 * @brief This module contains functions to control the blind.
 *
 * @{
 * @file    blindctrl.c
 * @brief   This module contains functions to control the blind.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "blindctrl.h"
#include "clock.h"
#include "motor.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum {
    idle,
    moving_up,
    moving_down,
    stopping
} blind_state_t;

// --- Local variables ---------------------------------------------------------

static blind_state_t g_blind_state = idle;
static uint8_t g_current_position;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void blind_initialize       (void)
{
    g_blind_state = idle;
}

/**
 * Move the blind to the position.
 *
 * @param[in] position
 * Position to move to. 0 = completely closed. 100 = completely open.
 */
void blind_move_to_position (uint8_t position)
{
    if (g_blind_state == idle) {
        // full close
        if (position == 0) {
            // move completely down
        } else if (position >= 100) {
            // move completely up
        } else if (position < g_current_position) {
            // move down
        } else if (position > g_current_position) {
            // move up
        }
    } else if (g_blind_state != stopping){
        blind_stop();
    }
}

/**
 * Stop currently moving blind and update current position.
 */
void blind_stop             (void)
{
    motor_stop();
    //TODO save elapsed movement time
    //TODO broadcast current position
}

void blind_background       (void)
{
    switch (g_blind_state) {
    case idle:
        break;

    case moving_up:
        break;

    case moving_down:
        break;

    case stopping:
        break;
    }
}

/** @} */
