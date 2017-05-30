/**
 * @addtogroup SLEEPMODE
 * @brief Control of sleep modes.
 *
 * @{
 * @file    sleepmode.c
 * @brief   Control of sleep modes..
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "sleepmode.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// Currently we don't need to save, if seeping was requested.
// There is no mechanism to detect, when sleeping is no longer wanted
//static uint8_t g_sleep_required = 0;
static uint8_t g_sleep_prevent_mask = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void sleep_activate(void)
{
    //g_sleep_required = 1;
    if (g_sleep_prevent_mask == 0) {
        sleep_mode();
        //g_sleep_required = 0;
    }
}

/**
 * Prevent controller from goind into sleep mode.
 * @param[in] prevent_mask  Bitmask to set or clear.
 * @param[in] on            != 0 set prevent mask == 0 clear prevent mask
 *
 * If all bits in prevent mask are cleared, we check, if sleeping is
 * required and go to sleep mode then..
 */
void sleep_prevent(uint8_t prevent_mask, uint8_t on)
{
    if (on != 0) {
        g_sleep_prevent_mask |= prevent_mask;
    } else {
        g_sleep_prevent_mask &= ~prevent_mask;
        //if (g_sleep_prevent_mask == 0 && g_sleep_required != 0) {
        //    sleep_mode();
        //    g_sleep_required = 0;
        //}
    }
}

/** @} */
