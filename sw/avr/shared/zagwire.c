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

#define PACKET_SIZE 2   ///< Number of bytes in a zagwire packet
#define PW_1_MIN    19  ///< Minimum pulse width for a valid "one" pulse.
#define PW_1_MAX    39  ///< Maximum pulse width for a valid "one" pulse.
#define PW_STB_MIN  48  ///< Minimum pulse width for a valid "strobe" pulse.
#define PW_STB_MAX  68  ///< Maximum pulse width for a valid "strobe" pulse.
#define PW_0_MIN    76  ///< Minimum pulse width for a valid "zero" pulse.
#define PW_0_MAX    96  ///< Maximum pulse width for a valid "zero" pulse.

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t g_irq_neg_edgeL;
static uint8_t g_irq_neg_pulse;
static uint8_t g_irq_pkt_idx; ///< packet index
static uint8_t g_irq_bit;
static uint8_t g_irq_byte[PACKET_SIZE];
static uint8_t g_irq_parity;
static uint8_t g_irq_parity_ok;

static uint8_t g_data_read;
static uint8_t g_datah;
static uint8_t g_datal;
static uint8_t g_parity_ok;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Interrupts --------------------------------------------------------------

/**
 * Timer 1 overflow vector resets statemachine if no further data is incoming.
 */
ISR(TIMER1_OVF_vect)
{
    if (g_irq_pkt_idx > PACKET_SIZE) {
        // long time no see -> a new temperature transmission
        g_irq_pkt_idx = 255;
    }
}

/**
 * Pin capture interrupt occurrs either for falling or rising edges.
 */
ISR(TIMER1_CAPT_vect)
{
    if ((TCCR1B & (1<<ICES1)) == 0) {
        // falling edge detected
        g_irq_neg_edgeL = ICR1L;
        TCCR1B |= (1<<ICES1);
    } else {
        // rising edge detected
        g_irq_neg_pulse = ICR1L - g_irq_neg_edgeL;
        TCCR1B &= ~(1<<ICES1);

        if (g_irq_neg_pulse >= PW_STB_MIN && g_irq_neg_pulse <= PW_STB_MAX) {
            // strobe-bit detected
            g_irq_bit = 8;
            g_irq_parity = 0;
            if (g_irq_pkt_idx == 255) {
                g_irq_pkt_idx++; // prepare to receive first byte
                g_irq_parity_ok = 1;
                g_irq_byte[0] = 0;
                g_irq_byte[1] = 0;
            }
        }
        if (g_irq_pkt_idx >= 0 && g_irq_pkt_idx < PACKET_SIZE) {
            if (g_irq_neg_pulse >= PW_1_MIN && g_irq_neg_pulse <= PW_1_MAX) {
                // 1-bit detected
                if (g_irq_bit > 0) {
                    g_irq_bit--;
                    g_irq_byte[g_irq_pkt_idx] |= (1<<g_irq_bit);
                    g_irq_parity++;
                }
                else {
                    g_irq_pkt_idx++;
                    // check if odd parity
                    if ((g_irq_parity & 1) == 0) {
                        g_irq_parity_ok = 0;
                    }
                }
            }
            else if (g_irq_neg_pulse >= PW_0_MIN && g_irq_neg_pulse <= PW_0_MAX) {
                // 0-bit detected
                if (g_irq_bit > 0) {
                    g_irq_bit--;
                }
                else {
                    g_irq_pkt_idx++;
                    // check if even parity
                    if ((g_irq_parity & 1) != 0) {
                        g_irq_parity_ok = 0;
                    }
                }
            }
        }
        else if (g_irq_pkt_idx == PACKET_SIZE) {
            if (g_data_read != 0) {
                g_datah = g_irq_byte[0];
                g_datal = g_irq_byte[1];
                g_parity_ok = g_irq_parity_ok;
                g_data_read = 0;
                g_irq_pkt_idx++; // only save bytes once
            }
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize zagwire protocol decoder.
 */
void            zagw_initialize     (void)
{
    // set anable port pin as output and disable sensor
#ifdef ZAGW_PORT_EN
    ZAGW_DDR_EN |= (1<<ZAGW_EN);
    ZAGW_PORT_EN &= ~(1<<ZAGW_EN);
#endif
    // configure Zagwire input pin. does not need pull-up
    ZAGW_DDR_DATA &= ~(1<<ZAGW_DATA);
    ZAGW_PORT_DATA &= ~(1<<ZAGW_DATA);

    // normal port operation: COM1A0 = COM1A1 = COM1B0 = COM1B2 = 0
    // normal waveform generator operation: WGM13...WGM10 = 0
    TCCR1A = 0;
    // set clock prescaler to 8: CS12 = 0, CS11 = 1, CS10 = 0
    // Input noise canceler ICNC1 = 1, Start capturing the falling edge ICES1 = 0
    TCCR1B = (1<<ICNC1) | (1<<CS11);
    // activate input capture interrupt
    TIMSK1 = (1<<ICIE1) | (1<<TOV1);

    g_data_read = 1;
    g_parity_ok = 0;
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
 * Return last received raw bitvalue of temperature sensor.
 *
 * @param[out]  val     Raw temperature value.
 *
 * @returns A valid temperature was received, otherwise false.
 */
bool zagw_get_raw(uint16_t *val)
{
    if (g_data_read == 1) return false;
    g_data_read = 1;
    if (g_parity_ok == 0 || (g_datah & 0xF0) != 0) return false;
    *val = g_datah << 8;
    *val |= g_datal;
    return true;
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

/** @} */
