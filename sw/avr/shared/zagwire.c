/**
 * @addtogroup ZAGWIRE
 * @brief Zagwire protocoll decoder for TSIC temperature sensors.
 *
 * @{
 * @file    zagwire.c
 * @brief   Zagwire protocoll decoder for TSIC temperature sensors.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

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

uint16_t    g_uTemperatureBits      = 0;
uint8_t     g_uTemperatureStatus    = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Interrupts --------------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            ZAGW_vInit          (void)
{
    // set anable port pin as output and disable sensor
    ZAGW_DDR_EN |= (1<<ZAGW_EN);
    ZAGW_PORT_EN &= ~(1<<ZAGW_EN);
    // set data port pin as input and disable pullup resistor
    ZAGW_DDR_DATA &= ~(1<<ZAGW_DATA);
    ZAGW_PORT_DATA &= ~(1<<ZAGW_DATA);

    g_uTemperatureStatus = 0;
    g_uTemperatureBits = 0;
}

void            ZAGW_vStartReception(void)
{
    g_uTemperatureStatus &= ~(1<<ZAGW_eNewValue);
}

uint8_t         ZAGW_uReceive  (void)
{
    uint8_t ii, retval, parity, temp_value1, temp_value2;

    retval = 1;

    // switch sensor on and wait for stabilization
    ZAGW_PORT_EN |= (1<<ZAGW_EN);
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

        g_uTemperatureBits = (temp_value1 << 8) | temp_value2;
        g_uTemperatureStatus |= (1<<ZAGW_eNewValue);
    } while (false);

    // disable sensor
    ZAGW_PORT_EN &= ~(1<<ZAGW_EN);

    return retval;
}

uint16_t        ZAGW_uGetBits       (void)
{
    return g_uTemperatureBits;
}

uint16_t        ZAGW_uGetTemperature(void)
{
    uint32_t temp;
    uint16_t digival, temperature;

    digival = g_uTemperatureBits;

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
