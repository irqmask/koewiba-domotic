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

#include "gtest/gtest.h"

#include "prjconf.h"

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

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------


void* test1_thread(void* user_arg)
{
    g_thread_value = 42;
    return NULL;
}

TEST(libsystem, threads_1)
{
    g_thread_value = 0;
    sys_thread_start(test1_thread);
    sys_sleep_ms(10);
    ASSERT_EQ(g_thread_value , 42);
}

/** @} */
