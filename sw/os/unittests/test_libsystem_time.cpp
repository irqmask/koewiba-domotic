/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_libsystem_time.c
 * @brief   This module contains the unittests for: libsystem/time.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "gtest/gtest.h"

#include "prjconf.h"

// include
#include "prjtypes.h"
// libsystem
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

TEST(libsystem, time_1)
{
    sys_time_t start, end, diff;

    start = sys_time_get_usecs();
    sys_sleep_ms(100);
    end = sys_time_get_usecs();

    diff = end - start;

    ASSERT_GT(diff, 99 * 1000);
    ASSERT_LT(diff, 110 * 1000);
}

/** @} */
