#include <stdio.h>
#include <CUnit/CUnit.h>

#include "ihex.h"

void IHEXTEST_vTestFileOpen(void)
{
    uint32_t retval;
    retval = ihex_read_file("filenotexisting.hex", NULL, NULL, NULL);
    CU_ASSERT(retval == 0);
}



CU_TestInfo TESTPWM_asTests[] = {
    {"Opening intgel hex files      ",  IHEXTEST_vTestFileOpen},
    CU_TEST_INFO_NULL,
};
