#include "prjconf.h"

#include <stdio.h>
#include <stdint.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>

extern CU_SuiteInfo TEST_asSuites[];

int main (int argc, char* argv[])
{
    int retval;

    do {
        if ((retval = CU_initialize_registry()) != CUE_SUCCESS) {
            break;
        }

        if ((retval = CU_register_suites(TEST_asSuites)) != CUE_SUCCESS) {
            break;
        }
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
    } while (0);
    return retval;
}