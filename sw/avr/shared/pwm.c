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
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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

typedef uint8_t uPWMCount_t;

// --- Local variables ---------------------------------------------------------

uint8_t g_bTableIndexMax = 1;

// runtime data. If runtime data has to be changed, at first the data is changed
// in the page which is currently not used by the ISR. After changing the table,
// the tables are switched.
uPWMCount_t g_auTimerEvent1[PWM_NUM_CHANNELS+1];   //!< differences to next timer event (page A).
uPWMCount_t g_auTimerEvent2[PWM_NUM_CHANNELS+1];   //!< differences to next timer event (page B).

uint8_t  g_auMask1[PWM_NUM_CHANNELS+1];         //!< mask of port bits to be cleared (page A).
uint8_t  g_auMask2[PWM_NUM_CHANNELS+1];         //!< mask of port bits to be cleared (page B).

uPWMCount_t g_auPWMCounts[PWM_NUM_CHANNELS+1];     //!< Current channel setting in timer-counts and sorted.
uint8_t  g_auPWMVal[PWM_NUM_CHANNELS];          //!< Current channel setting.

volatile uint8_t g_uTableIndexMax = 1;         //!< Maximum table index. Initialize to 1!
volatile uint8_t g_uSync;                      //!< Flag: Update is now possible.

// Pointer to time- and mask-pages

uPWMCount_t *g_puISRTimerEvent = g_auTimerEvent1;  //!< Points to timing data for IRQ routine.
uPWMCount_t *g_puMainTimerEvent = g_auTimerEvent2; //!< Points to timing data for main routine.

uint8_t *g_puISRMask        = g_auMask1;        //!< Points to mask data for IRQ routine.
uint8_t *g_puMainMask       = g_auMask2;        //!< Points to mask data for main routine.

#ifdef PRJCONF_UC_AVR
const uPWMCount_t g_auPWMValues[256] PROGMEM = {
#else
const uPWMCount_t g_auPWMValues[256] = {
#endif
        0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6,
        6, 6, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 10, 10,
        10, 10, 11, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14,
        15, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 19, 19, 19, 20, 20, 21,
        21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 30,
        30, 31, 31, 32, 33, 33, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 42, 43,
        44, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 56, 57, 58, 59, 60, 62,
        63, 64, 65, 67, 68, 69, 71, 72, 74, 75, 77, 78, 80, 82, 83, 85, 87, 89,
        90, 92, 94, 96, 98, 100, 102, 104, 106, 109, 111, 113, 115, 118, 120,
        123, 125, 128, 130, 133, 136, 139, 141, 144, 147, 150, 153, 157, 160,
        163, 166, 170, 173, 177, 181, 184, 188, 192, 196, 200, 204, 208, 212,
        217, 221, 226, 230, 235, 240, 245, 250, 253, 255 };

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// switch counter and mask data after update between irq and non-irq functions
static void switch_pages(void)
{
    uPWMCount_t*    tmp_ptr_count;
    uint8_t*        tmp_ptr_mask;

    tmp_ptr_count = g_puISRTimerEvent;
    g_puISRTimerEvent = g_puMainTimerEvent;
    g_puMainTimerEvent = tmp_ptr_count;

    tmp_ptr_mask = g_puISRMask;
    g_puISRMask = g_puMainMask;
    g_puMainMask = tmp_ptr_mask;
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

/**
 * Initialize PWM data and hardware. Therefore the 8bit timer 2 is used and
 * runs with about 100Hz.
 */
void pwm_init(void)
{
    uint8_t channel;

    PWM_CHN_DDR |= ((1<<PWM_CHN0_PIN) | (1<<PWM_CHN1_PIN) | (1<<PWM_CHN2_PIN));
    PWM_CHN_PORT &= ~((1<<PWM_CHN0_PIN) | (1<<PWM_CHN1_PIN) | (1<<PWM_CHN2_PIN));

    // initialize timer 1 peripheral, normal-mode
    REG_TIMER2_TCCRA = 0;
    // set prescaler to 1/256 (N=256)
    REG_TIMER2_TCCRB = (1<<REGBIT_TIMER2_CS2) | (1<<REGBIT_TIMER2_CS1);
    // enable timer 1 overflow A interrupt
    REG_TIMER2_IRQMSK |= (1<<REGBIT_TIMER2_OCIEA);

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

/**
 * Set PWM level for each channel.
 *
 * @param[in] uChannel      Channel number 0..2
 * @param[in] uValue        Value 0..255
 */
void pwm_set(uint8_t uChannel, uint8_t uValue)
{
    if (uChannel >= PWM_NUM_CHANNELS) return;
    g_auPWMVal[uChannel] = uValue;
}

/**
 * Update PWM data with new duty-cycles for each channel.
 */
void pwm_update(void)
{
    uPWMCount_t min, tmp_set;
    uint8_t     index, jj, kk, mask_bits_to_set, mask_current_bit;

    mask_bits_to_set = 0;
    mask_current_bit = 1;
    // generate mask of bits to set/clear
    // this function implies that the channels are corresponding to bits of one
    // port. The port bits have to be continuous.
    for (index=1; index <= PWM_NUM_CHANNELS; index++) {
        // special case: 100% duty-cycle
        if (g_auPWMVal[index-1] == 255) {
            g_puMainMask[index] = 255; // don't switch of anything
            mask_bits_to_set |= mask_current_bit;
            g_auPWMCounts[index] = PWM_MAX_COUNT / 2;
        }
        else if (g_auPWMVal[index-1] != 0) {
            g_puMainMask[index] = ~mask_current_bit;
            mask_bits_to_set |= mask_current_bit;
            g_auPWMCounts[index] = pgm_read_word(&g_auPWMValues[g_auPWMVal[index-1]]);
        }
        else { // g_auPWMVal[index-1] == 0
            g_puMainMask[index] = ~mask_current_bit;
            g_auPWMCounts[index] = PWM_MAX_COUNT / 2;
        }
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
#ifdef PRJCONF_UC_AVR
    while (g_uSync == 0);
#endif
    cli();
    switch_pages();
    g_uTableIndexMax = kk;
    sei();
}

/**
 * Set new channel PWM value and update.
 * pwm_set() and pwm_update() in one call.
 *
 * @param[in] uChannel      Channel number 0..2
 * @param[in] uValue        Value 0..255
 */
void pwm_set_and_update(uint8_t uChannel, uint8_t uValue)
{
    pwm_set(uChannel, uValue);
    pwm_update();
}

/** @} */
