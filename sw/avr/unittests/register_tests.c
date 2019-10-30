#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>

//extern CU_TestInfo          TESTBUSSCHED_asTests[];
//extern CU_TestInfo          TESTIHEX_asTests[];
//extern CU_TestInfo          TESTPWM_asTests[];
//extern CU_InitializeFunc    PWMTEST_iSuiteInit;
//extern CU_CleanupFunc       PWMTEST_iSuiteCleanup;

extern CU_TestInfo          datetime_tests[], 
                            alarmclock_tests[];

CU_SuiteInfo test_suites[] = {
    //{ "IHEX",       NULL,   NULL,   TESTIHEX_asTests },
    //{ "BUSSCHED",   NULL,   NULL,   TESTBUSSCHED_asTests },
    //{ "PWM",        NULL,   NULL,   TESTPWM_asTests },
    { "DateTime",     NULL,   NULL,   NULL,     NULL, datetime_tests },
    { "Alarmclock",   NULL,   NULL,   NULL,     NULL, alarmclock_tests },
    //{ NULL, PWMTEST_iSuiteInit, PWMTEST_iSuiteCleanup, TESTPWM_asTests },
    CU_SUITE_INFO_NULL,
};