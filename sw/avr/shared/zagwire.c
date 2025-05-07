/**
 * @addtogroup ZAGWIRE
 * @brief Zagwire protocol decoder for TSIC temperature sensors.
 *
 * @{
 * @file    zagwire.c
 * @brief   Zagwire protocol decoder for TSIC temperature sensors.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2019  christian <irqmask@web.de>
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "zagwire.h"

// --- Definitions -------------------------------------------------------------

// The idle-level is 5V. Every bit starts with a falling edge.
// Zagwire pulse length is ~125us. A data packet starts with a so called
// strobe-bit, followed by 8 data bits, one parity bit.
// One complete temperature transmission takes two packets. Between the packets
// is a pause of one bit-length, where the signal is high.
// A strobe bit has 50% duty cycle: ~62.5us low-pulse followed by a 62.5us high pulse.
// A 1 bit has about 75% duty-cycle: a pulse with ~31.25us low level followed by
// ~93.75us high-level signal.
// A 0 bit has about 25% duty-cycle: a pulse with ~93.75us low level followed
// by ~31.25us high-level signal.
// Remember, every bit starts with a falling edge. This algorithm measures the
// low-pulse width and fits it in tolerance windows to select if it is a
// strobe-bit, a 0-bit or a 1-bit.

#define PW_1_MIN    15  ///< Minimum pulse width for a valid "one" pulse.
#define PW_1_MAX    35  ///< Maximum pulse width for a valid "one" pulse.
#define PW_STB_MIN  39  ///< Minimum pulse width for a valid "strobe" pulse.
#define PW_STB_MAX  60  ///< Maximum pulse width for a valid "strobe" pulse.
#define PW_0_MIN    63  ///< Minimum pulse width for a valid "zero" pulse.
#define PW_0_MAX    96  ///< Maximum pulse width for a valid "zero" pulse.

/// number of samples (two bytes, each having 1 strobe-bit + 8 data-bits + 1 parity-bit)
#define ZAGWIRE_SAMPLE_LEN          ((1 + 8 + 1) * 2)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t g_irq_neg_edgeL;
static uint8_t g_samples_neg_pulsewidths[ZAGWIRE_SAMPLE_LEN];
static uint8_t g_sample_idx;
static uint8_t g_bad_readings;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static bool isStrobe(uint8_t sample)
{
    return (sample >= PW_STB_MIN) && (sample <= PW_STB_MAX);
}

static bool isLow(uint8_t sample)
{
    return (sample >= PW_0_MIN) && (sample <= PW_0_MAX);
}

static bool isHigh(uint8_t sample)
{
    return (sample >= PW_1_MIN) && (sample <= PW_1_MAX);
}

// --- Interrupts --------------------------------------------------------------

/**
 * Timer 1 overflow vector resets statemachine if no further data is incoming.
 */
ISR(TIMER1_OVF_vect)
{
    TIMSK1 = 0;
}

/**
 * Pin capture interrupt occurs either for falling or rising edges.
 */
ISR(TIMER1_CAPT_vect)
{
    if ((TCCR1B & (1<<ICES1)) == 0) {
        // falling edge detected
        g_irq_neg_edgeL = ICR1L;
        TCCR1B |= (1<<ICES1);
        TIFR1 |= (1<<ICF1); // clear interrupt
    } else {
        // rising edge detected
        g_samples_neg_pulsewidths[g_sample_idx++] = ICR1L - g_irq_neg_edgeL;
        TCCR1B &= ~(1<<ICES1);
        TIFR1 |= (1<<ICF1); // clear interrupt
    }
    if (g_sample_idx >= ZAGWIRE_SAMPLE_LEN) {
        // disable overflow and capture interrupt
        TIMSK1 = 0;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize zagwire protocol decoder.
 */
void            zagw_initialize     (void)
{
    // set enable port pin as output and disable sensor
#ifdef ZAGW_PORT_EN
    ZAGW_DDR_EN |= (1<<ZAGW_EN);
    ZAGW_PORT_EN &= ~(1<<ZAGW_EN);
#endif
    // configure Zagwire input pin. does not need pull-up
    ZAGW_DDR_DATA &= ~(1<<ZAGW_DATA);
    ZAGW_PORT_DATA &= ~(1<<ZAGW_DATA);

    // normal port operation: COM1A0 = COM1A1 = COM1B0 = COM1B2 = 0
    // no waveform generator operation: WGM13...WGM10 = 0
    TCCR1A = 0;
    // set clock prescaler to 8: CS12 = 0, CS11 = 1, CS10 = 0
    // Input noise canceler ICNC1 = 1, Start capturing the falling edge ICES1 = 0
    TCCR1B = (1<<ICNC1) | (1<<CS11);

    TIMSK1 = 0;
    g_sample_idx = 0;
    g_bad_readings = 0;
}

/**
 * Enable or disable sensor.
 */
void zagw_enable(bool enable)
{
#ifdef ZAGW_PORT_EN
    if (enable) {
        ZAGW_PORT_EN |= (1 << ZAGW_EN);
    }
    else {
        ZAGW_PORT_EN &= ~(1 << ZAGW_EN);
    }
#else
    (void)enable;
#endif
}

/**
 * Start sampling of temperature. Sampling stops automatically when done.
 */
void zagw_start_sampling(void)
{
    if (TIMSK1 == 0) {
        // the sampling may start in between a running transmission
        // from the sensor.
        // since the error rate is high anyway, this is an additional, neglectable error.

        // helper code to check that signal is idle (high). When used, an improvement
        // is not really measurable.
        /*
        uint8_t to;

        do {
            to = 175;
            while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) && to > 0)
            {
                to--;
                _delay_us(1);
            }
        } while (to > 0);*/
        TCNT1H = 0;
        TCNT1L = 1;
        // capture falling edge first
        TCCR1B &= ~(1<<ICES1);
        // clear interrupt flags
        TIFR1 |= ((1<<ICF1) | (1<<TOV1));
        // activate input capture and overflow interrupt
        TIMSK1 = (1<<ICIE1) | (1<<TOV1);
        g_sample_idx = 0;
    }
}

/**
 * Check if it is still sampling temparature
 */
bool zagw_sampling_done(void)
{
    return (TIMSK1 == 0);
}

/**
 * check if sampling is finished and return last received raw bitvalue
 * of temperature sensor.
 *
 * @returns Raw temperature value if successful, otherwise 0.
 */
uint16_t zagw_get_raw(void)
{
    uint16_t retval = 0;
    uint8_t idx, parity, highbyte, lowbyte;
    do {
        if (TIMSK1 != 0) break;

        if (g_sample_idx != ZAGWIRE_SAMPLE_LEN) break;

        // check both strobe bits
        if (!isStrobe(g_samples_neg_pulsewidths[0])) break;
        if (!isStrobe(g_samples_neg_pulsewidths[10])) break;

        parity = 0;
        highbyte = 0;
        for (idx = 0; idx < 8; idx++) {
            uint8_t sample = g_samples_neg_pulsewidths[1 + idx];
            if (isLow(sample)) {
                continue;
            }
            else if (isHigh(sample)) {
                highbyte |= (1<<(7-idx));
                parity++;
            }
            else {
                idx = 0; // indicate error
                break;
            }
        }
        if (idx != 8) break;

        // check parity
        uint8_t psample = g_samples_neg_pulsewidths[9];
        if (isLow(psample)) {
            if ((parity & 1) != 0) break;
        }
        else if (isHigh(psample)) {
            if ((parity & 1) == 0) break;
        }
        else break;

        // low byte
        parity = 0;
        lowbyte = 0;
        for (idx = 0; idx < 8; idx++) {
            uint8_t sample = g_samples_neg_pulsewidths[11 + idx];
            if (isLow(sample)) {
                continue;
            }
            else if (isHigh(sample)) {
                lowbyte |= (1<<(7-idx));
                parity++;
            }
            else {
                idx = 0; // indicate error
                break;
            }
        }
        if (idx != 8) break;

        // check parity
        psample = g_samples_neg_pulsewidths[19];
        if (isLow(psample)) {
            if ((parity & 1) != 0) break;
        }
        else if (isHigh(psample)) {
            if ((parity & 1) == 0) break;
        }
        else break;

        retval = highbyte;
        retval = retval << 8;
        retval = retval | lowbyte;

    } while (false);

    if (retval == 0 && g_bad_readings < 255) g_bad_readings++;

    return retval;
}

/**
 * Return last received temperature value in 1/100K.
 * 0°C equals to a value of 27315, 100°C equals to a value of 37315.
 *
 * @param[in]   raw     Raw temperature.
 *
 * @returns Converted temperature in 1/100K.
 */
uint16_t zagw_get_temperature(uint16_t raw)
{
    uint32_t temp;

    if (raw >= 767) raw += 1;

    // rangecheck: digival = 0..2048
    temp = (uint32_t)raw * 2500;
    temp >>= 8;
    // rangecheck: temp = 0..20000

    temp += (27315 - 5000);
    return (uint16_t)temp;
}

/**
 * return accumulated number of bad readings
 */
uint8_t zagw_debug_get_bad_readings(void)
{
    return g_bad_readings;
}

/**
 * Reset number of bad readings
 */
void zagw_debug_reset_bad_readings(void)
{
    g_bad_readings = 0;
}

/** @} */
