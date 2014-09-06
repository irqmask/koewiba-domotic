#include <CUnit/CUnit.h>

extern CU_TestInfo          TESTBUSSCHED_asTests[];
extern CU_TestInfo          TESTIHEX_asTests[];
extern CU_TestInfo          TESTPWM_asTests[];
extern CU_InitializeFunc    PWMTEST_iSuiteInit;
extern CU_CleanupFunc       PWMTEST_iSuiteCleanup;

CU_SuiteInfo TEST_asSuites[] = {
    { "IHEX",       NULL,   NULL,   TESTIHEX_asTests },
    { "BUSSCHED",   NULL,   NULL,   TESTBUSSCHED_asTests },
    { "PWM",        NULL,   NULL,   TESTPWM_asTests },
    //{ NULL, PWMTEST_iSuiteInit, PWMTEST_iSuiteCleanup, TESTPWM_asTests },
    CU_SUITE_INFO_NULL,
};
