#include <stdio.h>
#include <CUnit/CUnit.h>

#include "appconfig.h"
#include "pcbconfig.h"

#include "pwm.h"

uint8_t         g_uAvrTimer1_TCCRA  = 0;
uint8_t         g_uAvrTimer1_TCCRB  = 0;
uint8_t         g_uAvrTimer1_IRQMSK = 0;

uint8_t         g_uPWMCount         = 0;
uint8_t         g_uPWMCompare       = 0;
uint8_t         g_uPort             = 0;
uint8_t         g_uDDR              = 0;
uint16_t        g_auTimeChannelON[PWM_NUM_CHANNELS];

uint16_t pgm_read_word(uint16_t* puVal)
{
    return *puVal;
}

void cli(void)
{
}

void sei(void)
{
}

void vRunPWM (uint8_t uLoops)
{
    PWMCOUNT    = 0;
    PWMCOMPARE  = 0;
    g_auTimeChannelON[0] = 0;
    g_auTimeChannelON[1] = 0;
    g_auTimeChannelON[2] = 0;

    // do more than one loop because not every PWM cycle starts at count 0.
    do {
        PWMCOUNT++;
        if (PWMCOUNT == PWMCOMPARE) {
            PWM_vOnIRQ();
        }

        // measure duty-cycle
        if ((PWM_CHN_PORT & (1<<PWM_CHN0_PIN)) &&
            (g_auTimeChannelON[0] < PWM_MAX_COUNT)) g_auTimeChannelON[0]++;
        if ((PWM_CHN_PORT & (1<<PWM_CHN1_PIN)) &&
            (g_auTimeChannelON[1] < PWM_MAX_COUNT)) g_auTimeChannelON[1]++;
        if ((PWM_CHN_PORT & (1<<PWM_CHN2_PIN)) &&
            (g_auTimeChannelON[2] < PWM_MAX_COUNT)) g_auTimeChannelON[2]++;

        if (PWMCOUNT==0) {
            if (--uLoops) {
                g_auTimeChannelON[0] = 0;
                g_auTimeChannelON[1] = 0;
                g_auTimeChannelON[2] = 0;
            }
        }
    } while (uLoops);
}

void vPrintDutyCycle (void)
{
    printf("\nDuty cycle Chn1=%d%% Chn2=%d%% Chn3=%d%%\n",
           (100*g_auTimeChannelON[0]/PWM_MAX_COUNT),
           (100*g_auTimeChannelON[1]/PWM_MAX_COUNT),
           (100*g_auTimeChannelON[2]/PWM_MAX_COUNT));
}

void PWMTEST_vTestInit(void)
{
    g_uPWMCount     = 0;
    g_uPWMCompare   = 0;
    g_uPort         = 255;
    g_uDDR          = 0;
    g_auTimeChannelON[0] = 0;
    g_auTimeChannelON[1] = 0;
    g_auTimeChannelON[2] = 0;

    PWM_vInit();
    // Test if all needed ports are configured as output
    CU_ASSERT((g_uDDR & 0b00000111) == 0b00000111);
    // Test if all needed outputs are switched off
    CU_ASSERT((g_uPort & 0b11111000) == 0b11111000);
}


void PWMTEST_vTestAllDifferent(void)
{
    PWM_vSet(0,64);
    PWM_vSet(1,127);
    PWM_vSet(2,191);
    PWM_vUpdate();
    vRunPWM(3);
    // vPrintDutyCycle();

    // test measured duty-cycle
    CU_ASSERT_EQUAL(g_auTimeChannelON[0], 6);
    CU_ASSERT_EQUAL(g_auTimeChannelON[1], 20);
    CU_ASSERT_EQUAL(g_auTimeChannelON[2], 72);
}

void PWMTEST_vTestAllOn(void)
{
    PWM_vSet(0,255);
    PWM_vSet(1,255);
    PWM_vSet(2,255);
    PWM_vUpdate();
    vRunPWM(2);
    // vPrintDutyCycle();

    // test measured duty-cycle to be full-on
    CU_ASSERT_EQUAL(g_auTimeChannelON[0], PWM_MAX_COUNT);
    CU_ASSERT_EQUAL(g_auTimeChannelON[1], PWM_MAX_COUNT);
    CU_ASSERT_EQUAL(g_auTimeChannelON[2], PWM_MAX_COUNT);
}

void PWMTEST_vTestAllZero(void)
{
    PWM_vSet(0,0);
    PWM_vSet(1,0);
    PWM_vSet(2,0);
    PWM_vUpdate();
    vRunPWM(2);
    // vPrintDutyCycle();

    // test measured duty-cycle to be full-off
    CU_ASSERT_EQUAL(g_auTimeChannelON[0], 0);
    CU_ASSERT_EQUAL(g_auTimeChannelON[1], 0);
    CU_ASSERT_EQUAL(g_auTimeChannelON[2], 0);
}

void PWMTEST_vTest101(void)
{
    PWM_vSet(0,255);
    PWM_vSet(1,0);
    PWM_vSet(2,255);
    PWM_vUpdate();
    vRunPWM(3);
    // vPrintDutyCycle();

    // test measured duty-cycle to be full-on
    CU_ASSERT_EQUAL(g_auTimeChannelON[0], PWM_MAX_COUNT);
    CU_ASSERT_EQUAL(g_auTimeChannelON[1], 0);
    CU_ASSERT_EQUAL(g_auTimeChannelON[2], PWM_MAX_COUNT);
}

void PWMTEST_vTest010(void)
{
    PWM_vSet(0,0);
    PWM_vSet(1,255);
    PWM_vSet(2,0);
    PWM_vUpdate();
    vRunPWM(3);
    // vPrintDutyCycle();

    // test measured duty-cycle to be full-on
    CU_ASSERT_EQUAL(g_auTimeChannelON[0], 0);
    CU_ASSERT_EQUAL(g_auTimeChannelON[1], PWM_MAX_COUNT);
    CU_ASSERT_EQUAL(g_auTimeChannelON[2], 0);
}

CU_TestInfo TESTPWM_asTests[] = {
    {"Initialization of PWM module  ",  PWMTEST_vTestInit},
    {"All PWM channels different    ",  PWMTEST_vTestAllDifferent},
    {"All PWM channels on           ",  PWMTEST_vTestAllOn},
    {"All PWM channels off          ",  PWMTEST_vTestAllZero},
    {"All PWM channels on-off-on    ",  PWMTEST_vTest101},
    {"PWM channels off-on-off       ",  PWMTEST_vTest010},
    CU_TEST_INFO_NULL,
};
