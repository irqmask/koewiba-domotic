/**
 * @addtogroup PWM
 * @brief Pulse width modulation driver.
 *
 * @{
 * @file    pwm.c
 * @brief   Pulse width modulation driver.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#endif

#include "prjtypes.h"
#include "ucontroller.h"
#include "pwm.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

sPWMChannel_t   g_asChannels[PWM_NUM_CHANNELS];

#ifdef PWM_LINEAR_TABLE
#ifdef PRJCONF_UC_AVR
const uint16_t        g_auPWMValues[256] PROGMEM = {
#else
const uint16_t        g_auPWMValues[256] = {
#endif
    0, 512, 768, 1024, 1280, 1536, 1792, 2048, 2304, 2560, 2816, 3072, 3328,
    3584, 3840, 4096, 4352, 4608, 4864, 5120, 5376, 5632, 5888, 6144, 6400, 6656,
    6912, 7168, 7424, 7680, 7936, 8192, 8448, 8704, 8960, 9216, 9472, 9728, 9984,
    10240, 10496, 10752, 11008, 11264, 11520, 11776, 12032, 12288, 12544, 12800,
    13056, 13312, 13568, 13824, 14080, 14336, 14592, 14848, 15104, 15360, 15616,
    15872, 16128, 16384, 16640, 16896, 17152, 17408, 17664, 17920, 18176, 18432,
    18688, 18944, 19200, 19456, 19712, 19968, 20224, 20480, 20736, 20992, 21248,
    21504, 21760, 22016, 22272, 22528, 22784, 23040, 23296, 23552, 23808, 24064,
    24320, 24576, 24832, 25088, 25344, 25600, 25856, 26112, 26368, 26624, 26880,
    27136, 27392, 27648, 27904, 28160, 28416, 28672, 28928, 29184, 29440, 29696,
    29952, 30208, 30464, 30720, 30976, 31232, 31488, 31744, 32000, 32256, 32512,
    32768, 33023, 33279, 33535, 33791, 34047, 34303, 34559, 34815, 35071, 35327,
    35583, 35839, 36095, 36351, 36607, 36863, 37119, 37375, 37631, 37887, 38143,
    38399, 38655, 38911, 39167, 39423, 39679, 39935, 40191, 40447, 40703, 40959,
    41215, 41471, 41727, 41983, 42239, 42495, 42751, 43007, 43263, 43519, 43775,
    44031, 44287, 44543, 44799, 45055, 45311, 45567, 45823, 46079, 46335, 46591,
    46847, 47103, 47359, 47615, 47871, 48127, 48383, 48639, 48895, 49151, 49407,
    49663, 49919, 50175, 50431, 50687, 50943, 51199, 51455, 51711, 51967, 52223,
    52479, 52735, 52991, 53247, 53503, 53759, 54015, 54271, 54527, 54783, 55039,
    55295, 55551, 55807, 56063, 56319, 56575, 56831, 57087, 57343, 57599, 57855,
    58111, 58367, 58623, 58879, 59135, 59391, 59647, 59903, 60159, 60415, 60671,
    60927, 61183, 61439, 61695, 61951, 62207, 62463, 62719, 62975, 63231, 63487,
    63743, 63999, 64255, 64511, 64767, 65023, 65279, 65535
};
#else
#ifdef PRJCONF_UC_AVR
const uint16_t        g_auPWMValues[256] PROGMEM = {
#else
const uint16_t        g_auPWMValues[256] = {
#endif
    0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 7,
    7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 14, 15,
    15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    31, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 49, 52, 54, 56, 59,
    61, 64, 67, 70, 73, 76, 79, 83, 87, 91, 95, 99, 103, 108, 112,
    117, 123, 128, 134, 140, 146, 152, 159, 166, 173, 181, 189, 197,
    206, 215, 225, 235, 245, 256, 267, 279, 292, 304, 318, 332, 347,
    362, 378, 395, 412, 431, 450, 470, 490, 512, 535, 558, 583, 609,
    636, 664, 693, 724, 756, 790, 825, 861, 899, 939, 981, 1024, 1069,
    1117, 1166, 1218, 1272, 1328, 1387, 1448, 1512, 1579, 1649, 1722,
    1798, 1878, 1961, 2048, 2139, 2233, 2332, 2435, 2543, 2656, 2773,
    2896, 3025, 3158, 3298, 3444, 3597, 3756, 3922, 4096, 4277, 4467,
    4664, 4871, 5087, 5312, 5547, 5793, 6049, 6317, 6596, 6889, 7194,
    7512, 7845, 8192, 8555, 8933, 9329, 9742, 10173, 10624, 11094,
    11585, 12098, 12634, 13193, 13777, 14387, 15024, 15689, 16384,
    17109, 17867, 18658, 19484, 20346, 21247, 22188, 23170, 24196,
    25267, 26386, 27554, 28774, 30048, 31378, 32768, 34218, 35733,
    37315, 38967, 40693, 42494, 44376, 46340, 48392, 50534, 52772,
    55108, 57548, 60096, 62757, 65535
};
#endif

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Interrupts --------------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void PWM_vInit(void)
{
    uint8_t channel;

    for (channel=0; channel<PWM_NUM_CHANNELS; channel++) {
        g_asChannels[channel].uNextWakeUp = 0;
        g_asChannels[channel].uCurrentPulseWidth = 0;
        g_asChannels[channel].uInverseCurrentPulseWidth = PWM_MAX_COUNT;
    }
    PWM_CHN0_DDR |= (1<<PWM_CHN0_PIN);
    PWM_CHN1_DDR |= (1<<PWM_CHN1_PIN);
    PWM_CHN2_DDR |= (1<<PWM_CHN2_PIN);

    PWM_CHN0_PORT &= ~(1<<PWM_CHN0_PIN);
    PWM_CHN1_PORT &= ~(1<<PWM_CHN1_PIN);
    PWM_CHN2_PORT &= ~(1<<PWM_CHN2_PIN);

    // initialize timer 1 peripheral, normal-mode
    REG_TIMER1_TCCRA = 0;
    REG_TIMER1_TCCRB = (1<<REGBIT_TIMER1_CS0); // set prescaler to 1/1 (N=1)
    // enable timer 1 overflow A interrupt
    REG_TIMER1_IRQMSK |= (1<<REGBIT_TIMER1_OCIEA);

    PWMCOMPARE = 0;
}

void PWM_vSet(uint8_t uChannel, uint8_t uValue)
{
    if (uChannel >= PWM_NUM_CHANNELS) return;
    g_asChannels[uChannel].uCurrentPulseWidth = g_auPWMValues[uValue];
    g_asChannels[uChannel].uInverseCurrentPulseWidth = PWM_MAX_COUNT - g_asChannels[uChannel].uCurrentPulseWidth;
}

/**
 * Interrupt handler which does the pulse-with-modulation.
 *
 * It especially checks for full-ON and full-OFF cases and does not switch the
 * output in this special situation.
 *
 * Requirement for timer/interrupt:
 *  - The interrupt has to have count and compare register with the bit-width
 *    defined in PWM.h.
 *  - If the interrupt is triggered and if the compare register did not change,
 *    the next interrupt must occur after one complete timer cycle (65536 ticks).
 */
PWM_INTERRUPT
{
    uint16_t diff, mindiff;

    mindiff = 0xFFFF;

    // channel 0
    if (g_asChannels[0].uNextWakeUp == PWMCOUNT) {
        // set next wakeup time for channel 0
        if (PWM_CHN0_PORT & (1<<PWM_CHN0_PIN)) {
            // only switch off if off-time is > 0
            if (g_asChannels[0].uInverseCurrentPulseWidth != 0) {
                PWM_CHN0_PORT &= ~(1<<PWM_CHN0_PIN);
            }
            diff = g_asChannels[0].uInverseCurrentPulseWidth;
        } else {
            if (g_asChannels[0].uCurrentPulseWidth != 0) {
                PWM_CHN0_PORT |= (1<<PWM_CHN0_PIN);
            }
            diff = g_asChannels[0].uCurrentPulseWidth;
        }
        g_asChannels[0].uNextWakeUp = PWMCOUNT + diff;
    } else {
        // calculate new difference from now for channel 0
        diff = g_asChannels[0].uNextWakeUp - PWMCOUNT;
    }
    if (diff < mindiff) mindiff = diff;

    // channel 1
    if (g_asChannels[1].uNextWakeUp == PWMCOUNT) {
        // set next wakeup time for channel 0
        if (PWM_CHN1_PORT & (1<<PWM_CHN1_PIN)) {
            // only switch off if off-time is > 0
            if (g_asChannels[1].uInverseCurrentPulseWidth != 0) {
                PWM_CHN1_PORT &= ~(1<<PWM_CHN1_PIN);
            }
            diff = g_asChannels[1].uInverseCurrentPulseWidth;
        } else {
            PWM_CHN1_PORT |= (1<<PWM_CHN1_PIN);
            if (g_asChannels[0].uCurrentPulseWidth != 0) {
                diff = g_asChannels[1].uCurrentPulseWidth;
            }
        }
        g_asChannels[1].uNextWakeUp = PWMCOUNT + diff;
    } else {
        // calculate new difference from now for channel 1
        diff = g_asChannels[1].uNextWakeUp - PWMCOUNT;
    }
    if (diff < mindiff) mindiff = diff;

    // channel 2
    if (g_asChannels[2].uNextWakeUp == PWMCOUNT) {
        // set next wakeup time for channel 0
        if (PWM_CHN2_PORT & (1<<PWM_CHN2_PIN)) {
            // only switch off if off-time is > 0
            if (g_asChannels[2].uInverseCurrentPulseWidth != 0) {
                PWM_CHN2_PORT &= ~(1<<PWM_CHN2_PIN);
            }
            diff = g_asChannels[2].uInverseCurrentPulseWidth;
        } else {
            PWM_CHN2_PORT |= (1<<PWM_CHN2_PIN);
            if (g_asChannels[0].uCurrentPulseWidth != 0) {
                diff = g_asChannels[2].uCurrentPulseWidth;
            }
        }
        g_asChannels[2].uNextWakeUp = PWMCOUNT + diff;
    } else {
        // calculate new difference from now for channel 2
        diff = g_asChannels[2].uNextWakeUp - PWMCOUNT;
    }
    if (diff < mindiff) mindiff = diff;

    PWMCOMPARE = PWMCOUNT + mindiff;
}

/** @} */
