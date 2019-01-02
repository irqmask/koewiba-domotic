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

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

uint16_t    temperature_bits      = 0;
uint8_t     temperature_status    = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Interrupts --------------------------------------------------------------

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
    // set data port pin as input and disable pullup resistor
    ZAGW_DDR_DATA &= ~(1<<ZAGW_DATA);
    ZAGW_PORT_DATA &= ~(1<<ZAGW_DATA);

    temperature_status = 0;
    temperature_bits = 0;
}

/**
 * Start zagwire protocol decoding.
 */
void            zagw_start_reception(void)
{
    temperature_status &= ~(1<<ZAGW_eNewValue);
}

/**
 * Receive zagwire data.
 */
uint8_t         zagw_receive        (void)
{
    uint8_t ii, retval, parity, temp_value1, temp_value2;

    retval = 1;

    // switch sensor on and wait for stabilization
#ifdef ZAGW_PORT_EN
    ZAGW_PORT_EN |= (1<<ZAGW_EN);
#endif
    _delay_us(120);

    do {
        // START BIT
        // wait for falling edge of start-bit
        while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0);

        // wait for raising edge
        while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) == 0);

        // FIRST DATA BYTE
        parity = 0;
        temp_value1 = 0;
        // read eight data bits
        for (ii=0; ii<8; ii++) {
            while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0);
            _delay_us(60);
            if ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0) {
                temp_value1 |= 1<<(7-ii);
                parity++;
            } else {
                while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) == 0);
            }
        }
        // read parity bit
        while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0);
        _delay_us(60);
        if ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0) {
            parity++;
        } else {
            while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) == 0);
        }

        // check parity
        if (parity & 0x01) {
            retval = 0;
            break;
        }

        // wait for falling edge of start-bit
        while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0);

        // wait for raising edge
        while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) == 0);

        // SECOND DATA BYTE
        parity = 0;
        temp_value2 = 0;
        // read eight data bits
        for (ii=0; ii<8; ii++) {
            while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0);
            _delay_us(60);
            if ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0) {
                temp_value2 |= 1<<(7-ii);
                parity++;
            } else {
                while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) == 0);
            }
        }
        // read parity bit
        while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0);
        _delay_us(60);
        if ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) != 0) {
            parity++;
        } else {
            while ((ZAGW_PIN_DATA & (1<<ZAGW_DATA)) == 0);
        }

        // check parity
        if (parity & 0x01) {
            retval = 0;
            break;
        }

        temperature_bits = (temp_value1 << 8) | temp_value2;
        temperature_status |= (1<<ZAGW_eNewValue);
    } while (0);

    // disable sensor
#ifdef ZAGW_PORT_EN
    ZAGW_PORT_EN &= ~(1<<ZAGW_EN);
#endif

    return retval;
}

/**
 * Return last received raw bitvalue of temperature sensor.
 */
uint16_t        zagw_get_bits       (void)
{
    return temperature_bits;
}

/**
 * Return last received temperature value in 1/100K.
 */
uint16_t        zagw_get_temperature(void)
{
    uint32_t temp;
    uint16_t digival, temperature;

    digival = temperature_bits;

    if (digival >= 767) digival += 1;

    // rangecheck: digival = 0..2048
    temp = (uint32_t)digival * 2500;
    temp >>= 8;
    // rangecheck: temp = 0..20000

    temp += (27315 - 5000);
    temperature = (uint16_t)temp;
    return temperature;
}

/** @} */
