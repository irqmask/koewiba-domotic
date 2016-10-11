/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    tests.c
 * @brief   This module contains the table of test-suites
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include "CUnit/CUnit.h"

// include
#include "prjtypes.h"


// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern int libsystem_threads_init (void);
extern int libsystem_threads_cleanup (void);
extern CU_TestInfo test_libsystem_threads[];

extern int libsystem_time_init (void);
extern int libsystem_time_cleanup (void);
extern CU_TestInfo test_libsystem_time[];

CU_SuiteInfo test_suites[] = {
    { "libsystem_threads", libsystem_threads_init, libsystem_threads_cleanup, NULL, NULL, test_libsystem_threads },
    { "libsystem_time",    libsystem_time_init,    libsystem_time_cleanup,    NULL, NULL, test_libsystem_time    },
    CU_SUITE_INFO_NULL,
};

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------



// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/** @} */
