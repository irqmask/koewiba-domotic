/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    unittests_main.c
 * @brief   Test application for unittests of os code
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>

// include
#include "prjtypes.h"

// os/include
#include "error_codes.h"

// os/libsystem
#include "sysgetopt.h"
#include "systhread.h"
#include "sysserial.h"
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern CU_SuiteInfo test_suites[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (int argc, char* argv[])
{
    int             rc = eERR_NONE;

    printf("\nunittests...\n");
    if ((rc = CU_initialize_registry()) != CUE_SUCCESS) return rc;

    do {
        if ((rc = CU_register_suites(test_suites)) != CUE_SUCCESS) break;
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
    } while (0);
    CU_cleanup_registry();
    return rc;
}

/** @} */
