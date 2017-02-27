/**
 * @addtogroup TEST_BUS-MODULE_ATMEGA328
 *
 * @{
 * @file    main_test_bus-module_atmega328.c
 * @brief   Test hardware of bus-module_atmega328.
 * 
 * Test hardware of bus-module_atmega328:
 *  - EEProm 
 *  - RS485 transmitter (bus communication)
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "appconfig.h"
#include "pcbconfig.h"

#include "cmddef_common.h"
#include "moddef_common.h"

#include "bus.h"
#include "clock.h"
#include "crc16.h"
#include "eeprom_spi.h"
#include "led_debug.h"
#include "register.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t           g_bus;
static clock_timer_t    g_LED_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static bool test_eeprom (void)
{
    uint16_t 	ii, wanted_crc16, read_crc16;
    uint8_t		databyte;
    bool 		test_is_ok = true;

    // fill EEProm
    wanted_crc16 = CRC_START_VALUE;
    databyte = 0;
    for (ii = 0; ii<EEPROM_SIZE; ii++) {
    	eep_write(ii, 1, &databyte);
    	wanted_crc16 = crc_16_next_byte(wanted_crc16, databyte);
    	databyte++;
    	databyte ^= 0xFF;
    }

    // read EEProm content back an calculate checksum again
    read_crc16 = CRC_START_VALUE;
    for (ii = 0; ii<EEPROM_SIZE; ii++) {
    	eep_read(ii, 1, &databyte);
    	read_crc16 = crc_16_next_byte(read_crc16, databyte);
    }
    
    if (read_crc16 != wanted_crc16) {
    	test_is_ok = false;
    }

    return test_is_ok;
}

static void interpret_message (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    switch (msg[0]) {
    case eCMD_STATE_BITFIELDS:
        if (msglen < 2) break;
        if (msg[2]) LED_STATUS_OFF;
        else        LED_STATUS_ON;
        break;

    case eCMD_SLEEP:
        bus_sleep(&g_bus);
        break;

    default:
        break;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t sender = 0, module_id = 10;

    LED_STATUS_DDR |= (1<<LED_STATUS);
    LED_ERROR_DDR |= (1<<LED_ERROR);
    clk_initialize();

    register_set_u16(MOD_eReg_ModuleID, 0x7E);
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

    LED_ERROR_ON; LED_STATUS_ON;
    spi_master_init_blk();
    eep_initialize();
    sei();
    _delay_ms(1000);

    clk_timer_start(&g_LED_timer, CLOCK_MS_2_TICKS(1000));
    LED_ERROR_OFF; LED_STATUS_OFF;

    // run EEProm test
    // ------------------------------------------------------
    if (test_eeprom() == false) {
    	LED_ERROR_ON;
        msg[1] = 0x00; // 1 = test succeeded, 0 = test failed
    } else {
        msg[1] = 0x01; // 1 = test succeeded, 0 = test failed
    }
    // report EEProm test status
    msg[0] = 0xEF; // currently unused command number
    bus_send_message(&g_bus, 0x0002, 2, msg); // send message to address 0x0002

    // start main-loop and process incoming and outgoing messages
    // ------------------------------------------------------

    while (1) {
        // check for message and read it
        if (bus_get_message(&g_bus)) {
            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
        }
        // toggle status LED once a second
        if (clk_timer_is_elapsed(&g_LED_timer)) {
            clk_timer_start(&g_LED_timer, CLOCK_MS_2_TICKS(1000));
            LED_STATUS_TOGGLE;
        }
    }
    return 0;
}

/** @} */
