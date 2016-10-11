/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_libsystem_threads.c
 * @brief   This module contains the unittests for: libsystem/threads.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include "CUnit/CUnit.h"

// include
#include "prjtypes.h"
// libsystem
#include "systhread.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

int g_thread_value = 0;

int test_libsystem_threads_1 (void);

CU_TestInfo test_libsystem_threads[] = {
    { "threads1", test_libsystem_threads_1 },
    CU_TEST_INFO_NULL,
};

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int libsystem_threads_init (void)
{
    return CUE_SUCCESS;
}

int libsystem_threads_cleanup (void)
{
    return CUE_SUCCESS;
}

int test1_thread (void* ref)
{
    g_thread_value = 42;
    return 0;
}

int test_libsystem_threads_1 (void)
{
    g_thread_value = 0;
    sys_thread_start(test1_thread);
    sys_sleep_ms(10);
    CU_ASSERT(g_thread_value == 42);
    return CUE_SUCCESS;
}

/** @} */
