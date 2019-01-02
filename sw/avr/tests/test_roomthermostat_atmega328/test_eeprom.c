/*
 * @addtogroup TEST_ROOMTHERMOSTAT_ATMEGA328
 * @addtogroup TEST_EEPROM
 * @brief Test to read and write the external EEProm.
 *
 * @{
 * @file    test_eeprom.c
 * @brief   Test to read and write the external EEProm.
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

#include "prjtypes.h"

#include "crc16.h"
#include "eeprom_spi.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum
{
    eTEST_EEP_IDLE,
    eTEST_EEP_WRITE,
    eTEST_EEP_READ,
    eTEST_EEP_FINISHED,
} test_eep_states_t;

// --- Local variables ---------------------------------------------------------

static test_eep_states_t    g_state = eTEST_EEP_IDLE;
static uint16_t             g_curr_byte = 0;
static uint16_t             g_wanted_crc16 = 0;
static uint16_t             g_read_crc16 = 0;
static uint8_t              g_databyte = 0;
static uint8_t              g_test_is_ok = 1;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void test_eeprom_start(void)
{
    g_state = eTEST_EEP_WRITE;
    g_curr_byte = 0;
    g_databyte = 0;
    g_wanted_crc16 = 0;
    g_read_crc16 = 0;
    g_test_is_ok = 0;
    crc_16_start();
}
    
uint8_t test_eeprom_run(void)
{
    uint8_t rc = 1;
    
    switch (g_state) {
    case eTEST_EEP_WRITE:
        eep_write(g_curr_byte, 1, &g_databyte);
        g_wanted_crc16 = crc_16_next_byte(g_wanted_crc16, g_databyte);
        g_databyte++;
        g_databyte ^= 0xFF;
        g_curr_byte++;
        if (g_curr_byte >= EEPROM_SIZE) {
            g_state = eTEST_EEP_READ;
            g_databyte = 0;
            g_curr_byte = 0;
            crc_16_start();
        }
        break;
        
    case eTEST_EEP_READ:
        eep_read(g_curr_byte, 1, &g_databyte);
        g_read_crc16 = crc_16_next_byte(g_read_crc16, g_databyte);
        g_curr_byte++;
        if (g_curr_byte >= EEPROM_SIZE) {
            g_state = eTEST_EEP_FINISHED;
            if (g_wanted_crc16 == g_read_crc16) {
                g_test_is_ok = 1;
            }
            rc = 0;
        }
        break;
        
    default:
        break;
    }
                   
    return rc;
}

uint8_t test_eeprom_is_ok(void)
{
    return g_test_is_ok;
}

uint8_t test_eeprom_get_progress(void)
{
    uint32_t val = 0;

    switch (g_state) {
    case eTEST_EEP_IDLE:
        val = 0;
        break;

    case eTEST_EEP_WRITE:
        val = (50 * (uint32_t)g_curr_byte) / EEPROM_SIZE;
        break;

    case eTEST_EEP_READ:
        val = 50 + ((50 * (uint32_t)g_curr_byte) / EEPROM_SIZE);
        break;

    case eTEST_EEP_FINISHED:
        val = 100;
        break;

    default:
        break;
    }

    return val;
}

/**
 * @}
 * @}
 */
