/*
 * @addtogroup TEST_EEPROM
 * @addtogroup EEPROM
 * @brief Test to read and write the external EEProm.
 *
 * @{
 * @file    test_eeprom.c
 * @brief   Test to read and write the external EEProm.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

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
static BOOL                 g_test_is_ok = TRUE;

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
    g_test_is_ok = FALSE;
    crc_16_start();
}
    
BOOL test_eeprom_run(void)
{
    BOOL rc = TRUE;
    
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
                g_test_is_ok = TRUE;
            }
            rc = FALSE;
        }
        break;
        
    default:
        break;
    }
                   
    return rc;
}

BOOL test_eeprom_is_ok(void)
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

/** @} */
