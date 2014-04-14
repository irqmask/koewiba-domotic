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

uint8_t g_bTableIndexMax = 1;

// runtime data. If runtime data has to be changed, at first the data is changed
// in the page which is currently not used by the ISR. After changing the table,
// the tables are switched.
uint16_t g_auTimerEvent1[PWM_NUM_CHANNELS+1];   //!< differences to next timer event (page A).
uint16_t g_auTimerEvent2[PWM_NUM_CHANNELS+1];   //!< differences to next timer event (page B).

uint8_t  g_auMask1[PWM_NUM_CHANNELS+1];         //!< mask of port bits to be cleared (page A).
uint8_t  g_auMask2[PWM_NUM_CHANNELS+1];         //!< mask of port bits to be cleared (page B).

uint16_t g_auPWMCounts[PWM_NUM_CHANNELS+1];     //!< Current channel setting in timer-counts and sorted.
uint8_t  g_auPWMVal[PWM_NUM_CHANNELS];          //!< Current channel setting.

volatile uint8_t g_uTableIndexMax = 1;         //!< Maximum table index. Initialize to 1!
volatile uint8_t g_uSync;                      //!< Flag: Update is now possible.

// Pointer to time- and mask-pages

uint16_t *g_puISRTimerEvent = g_auTimerEvent1;  //!< Points to timing data for IRQ routine.
uint16_t *g_puMainTimerEvent = g_auTimerEvent2; //!< Points to timing data for main routine.

uint8_t *g_puISRMask        = g_auMask1;        //!< Points to mask data for IRQ routine.
uint8_t *g_puMainMask       = g_auMask2;        //!< Points to mask data for main routine.

#ifdef PRJCONF_UC_AVR
const uint16_t        g_auPWMValues[256] PROGMEM = {
#else
const uint16_t g_auPWMValues[256] = {
#endif
        0, 256, 256, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512,
        512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 768, 768,
        768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768, 768,
        1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
        1024, 1280, 1280, 1280, 1280, 1280, 1280, 1280, 1280, 1280, 1280, 1536,
        1536, 1536, 1536, 1536, 1536, 1536, 1536, 1792, 1792, 1792, 1792, 1792,
        1792, 1792, 2048, 2048, 2048, 2048, 2048, 2048, 2304, 2304, 2304, 2304,
        2304, 2304, 2560, 2560, 2560, 2560, 2816, 2816, 2816, 2816, 2816, 3072,
        3072, 3072, 3072, 3328, 3328, 3328, 3328, 3584, 3584, 3584, 3840, 3840,
        3840, 3840, 4096, 4096, 4096, 4352, 4352, 4352, 4608, 4608, 4864, 4864,
        4864, 5120, 5120, 5376, 5376, 5376, 5632, 5632, 5888, 5888, 6144, 6144,
        6400, 6400, 6656, 6656, 6912, 6912, 7168, 7168, 7424, 7680, 7680, 7936,
        7936, 8192, 8448, 8448, 8704, 8960, 9216, 9216, 9472, 9728, 9984, 9984,
        10240, 10496, 10752, 11008, 11264, 11264, 11520, 11776, 12032, 12288,
        12544, 12800, 13056, 13312, 13568, 13824, 14336, 14592, 14848, 15104,
        15360, 15872, 16128, 16384, 16640, 17152, 17408, 17664, 18176, 18432,
        18944, 19200, 19712, 19968, 20480, 20992, 21248, 21760, 22272, 22784,
        23040, 23552, 24064, 24576, 25088, 25600, 26112, 26624, 27136, 27904,
        28416, 28928, 29440, 30208, 30720, 31488, 32000, 32768, 33280, 34048,
        34816, 35584, 36096, 36864, 37632, 38400, 39168, 40192, 40960, 41728,
        42496, 43520, 44288, 45312, 46336, 47104, 48128, 49152, 50176, 51200,
        52224, 53248, 54272, 55552, 56576, 57856, 58880, 60160, 61440, 62720,
        64000, 65280, 65350 };

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void vSwitchPages(void)
{
    uint16_t* tmp_ptr16;
    uint8_t* tmp_ptr8;

    tmp_ptr16 = g_puISRTimerEvent;
    g_puISRTimerEvent = g_puMainTimerEvent;
    g_puMainTimerEvent = tmp_ptr16;

    tmp_ptr8 = g_puISRMask;
    g_puISRMask = g_puMainMask;
    g_puMainMask = tmp_ptr8;
}

// --- Interrupts --------------------------------------------------------------

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
    static uint8_t table_index = 0;
    uint8_t output_mask;

    PWMCOMPARE  += g_puISRTimerEvent[table_index];
    output_mask = g_puISRMask[table_index];

    if (table_index == 0) {
        PWM_CHN_PORT = output_mask;         // set port pins at the beginning                         // Ports setzen zu Begin der PWM
        table_index++;
    }
    else {
        PWM_CHN_PORT &= output_mask;        // reset port pins
        if (table_index == g_uTableIndexMax) {
            g_uSync     = 1;                // update is now possible
            table_index = 0;
        }
        else table_index++;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void PWM_vInit(void)
{
    uint8_t channel;

    PWM_CHN_DDR |= ((1<<PWM_CHN0_PIN) | (1<<PWM_CHN1_PIN) | (1<<PWM_CHN2_PIN));
    PWM_CHN_PORT |= ((1<<PWM_CHN0_PIN) | (1<<PWM_CHN1_PIN) | (1<<PWM_CHN2_PIN));

    // initialize timer 1 peripheral, normal-mode
    REG_TIMER1_TCCRA = 0;
    REG_TIMER1_TCCRB = (1<<REGBIT_TIMER1_CS0); // set prescaler to 1/1 (N=1)
    // enable timer 1 overflow A interrupt
    REG_TIMER1_IRQMSK |= (1<<REGBIT_TIMER1_OCIEA);

    PWMCOMPARE = 0;

    g_puISRTimerEvent  = g_auTimerEvent1;
    g_puMainTimerEvent = g_auTimerEvent2;
    g_puISRMask        = g_auMask1;
    g_puMainMask       = g_auMask2;
    g_uTableIndexMax   = 1;
    g_uSync            = 0;
    g_auMask1[0] = 0;
    g_auMask2[0] = 0;
    g_auTimerEvent1[0] = PWM_MAX_COUNT / 2;
    g_auTimerEvent2[0] = PWM_MAX_COUNT / 2;
    for (channel=0; channel<PWM_NUM_CHANNELS; channel++) {
        g_auPWMVal[channel] = 0;
        g_auMask1[channel+1] = 255;
        g_auMask2[channel+1] = 255;
        g_auTimerEvent1[channel+1] = PWM_MAX_COUNT / 2;
        g_auTimerEvent2[channel+1] = PWM_MAX_COUNT / 2;
    }
}

void PWM_vSet(uint8_t uChannel, uint8_t uValue)
{
    if (uChannel >= PWM_NUM_CHANNELS) return;
    g_auPWMVal[uChannel] = uValue;
}

void PWM_vUpdate(void)
{
    uint16_t min, tmp_set;
    uint8_t index, jj, kk, mask_bits_to_set, mask_current_bit;

    mask_bits_to_set = 0;
    mask_current_bit = 1;
    // generate mask of bits to set/clear
    // this function implies that the channels are corresponding to bits of one
    // port. The port bits have to be continuous.
    for (index=1; index <= PWM_NUM_CHANNELS; index++) {
        g_puMainMask[index] = ~mask_current_bit;
        // copy PWM setting
        g_auPWMCounts[index] = pgm_read_word(&g_auPWMValues[g_auPWMVal[index-1]]);
        if (g_auPWMCounts[index] != 0) mask_bits_to_set |= mask_current_bit;
        mask_current_bit <<= 1;
    }
    g_puMainMask[0] = mask_bits_to_set;

    // sort list by PWM settings
    for(index=1; index<=PWM_NUM_CHANNELS; index++) {
        min = PWM_MAX_COUNT;
        kk = index;
        for(jj=index; jj<=PWM_NUM_CHANNELS; jj++) {
            if (g_auPWMCounts[jj] < min) {
                kk=jj;                  // save index of elements to swap
                min = g_auPWMCounts[jj];
            }
        }
        if (kk != index) {
            // swap elements
            tmp_set = g_auPWMCounts[kk];
            g_auPWMCounts[kk] = g_auPWMCounts[index];
            g_auPWMCounts[index] = tmp_set;
            mask_current_bit = g_puMainMask[kk];
            g_puMainMask[kk] = g_puMainMask[index];
            g_puMainMask[index] = mask_current_bit;
        }
    }

    // find duplicate elements and elements which are zero
    kk = PWM_NUM_CHANNELS;
    index = 1;

    while (kk > index) {
        while (((g_auPWMCounts[index] == g_auPWMCounts[index+1]) ||
                (g_auPWMCounts[index] == 0)) && (kk > index)) {
            // joins bit-clear masks
            if (g_auPWMCounts != 0) g_puMainMask[index+1] &= g_puMainMask[index];

            // shift everything
            for (jj=index; jj<kk; jj++) {
                g_auPWMCounts[jj] = g_auPWMCounts[jj+1];
                g_puMainMask[jj] = g_puMainMask[jj+1];
            }
            kk--;
        }
        index++;
    }

    // check last element
    if (g_auPWMCounts[index] == 0) kk--;

    // calculate differences
    if (kk == 0) {
        g_puMainTimerEvent[0] = PWM_MAX_COUNT / 2;
        g_puMainTimerEvent[1] = PWM_MAX_COUNT / 2;
        kk = 1;
    } else {
        index = kk;
        g_puMainTimerEvent[index] = PWM_MAX_COUNT - g_auPWMCounts[index];
        tmp_set = g_auPWMCounts[index];
        index--;

        for (; index>0; index--) {
            g_puMainTimerEvent[index] = tmp_set - g_auPWMCounts[index];
            tmp_set = g_auPWMCounts[index];
        }
        g_puMainTimerEvent[0] = tmp_set;
    }

    // wait for sync
    g_uSync = 0;
    while (g_uSync == 0);

    cli();
    vSwitchPages();
    g_uTableIndexMax = kk;
    sei();
}

void PWM_vSetAndUpdate(uint8_t uChannel, uint8_t uValue)
{
    PWM_vSet(uChannel, uValue);
    PWM_vUpdate();
}

/** @} */
