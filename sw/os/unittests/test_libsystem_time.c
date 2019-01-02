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

#include "prjconf.h"

#include "CUnit/CUnit.h"

// include
#include "prjtypes.h"
// libsystem
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

void test_libsystem_time_1 (void);

CU_TestInfo test_libsystem_time[] = {
    { "time1", test_libsystem_time_1 },
    CU_TEST_INFO_NULL,
};

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int libsystem_time_init (void)
{
    return CUE_SUCCESS;
}

int libsystem_time_cleanup (void)
{
    return CUE_SUCCESS;
}

void test_libsystem_time_1 (void)
{
    sys_time_t start, end, diff;

    start = sys_time_get_usecs();
    sys_sleep_ms(100);
    end = sys_time_get_usecs();

    diff = end - start;
    printf("diff is: %lld\n", diff) ;

    CU_ASSERT(diff > 99 * 1000);
    CU_ASSERT(diff < 110 * 1000);
}

/** @} */
