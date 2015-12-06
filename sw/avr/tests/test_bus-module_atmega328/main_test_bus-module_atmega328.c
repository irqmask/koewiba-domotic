/** * @addtogroup TEST_BUS-MODULE_ATMEGA328 * * @{ * @file    main_test_bus-module_atmega328.c * @brief   Test hardware of bus-module_atmega328. *  * Test hardware of bus-module_atmega328: *  - EEProm  *  - RS485 transmitter (bus communication) * * @author  Christian Verhalen *///---------------------------------------------------------------------------// --- Include section ---------------------------------------------------------#include <avr/io.h>#include <avr/interrupt.h>#include "appconfig.h"#include "pcbconfig.h"#include "cmddef_common.h"#include "moddef_common.h"#include "bus.h"#include "clock.h"#include "crc16.h"#include "eeprom_spi.h"#include "led_debug.h"#include "register.h"#include "spi.h"// --- Definitions -------------------------------------------------------------// --- Type definitions --------------------------------------------------------// --- Local variables ---------------------------------------------------------static sBus_t           g_bus;static clock_timer_t    g_LED_timer;// --- Global variables --------------------------------------------------------// --- Module global variables -------------------------------------------------// --- Local functions ---------------------------------------------------------static BOOL test_eeprom (void){    uint16_t 	ii, wanted_crc16 = 0, read_crc16 = 0;    uint8_t		byte_to_write;    BOOL 		test_is_ok = TRUE;    // fill EEProm    crc_16_start();    byte_to_write = 0;    for (ii = 0; ii<EEPROM_SIZE; ii++) {    	eep_write(ii, 1, &byte_to_write);    	wanted_crc16 = crc_16_next_byte(wanted_crc16, byte_to_write);    	byte_to_write++;    	byte_to_write ^= 0xFF;    }    // read EEProm content back an calculate checksum again    crc_16_start();    for (ii = 0; ii<EEPROM_SIZE; ii++) {    	read_crc16 = crc_16_next_byte(read_crc16, byte_to_write);    }        if (read_crc16 != wanted_crc16) {    	test_is_ok = FALSE;    }    return test_is_ok;}static void interpret_message (uint16_t sender, uint8_t msglen, uint8_t* msg){    switch (msg[0]) {    case eCMD_STATE_BITFIELDS:        if (msglen < 2) break;        if (msg[2]) LED_STATUS_OFF;        else        LED_STATUS_ON;        break;    case eCMD_SLEEP:        bus_sleep(&g_bus);        break;    default:        break;    }}// --- Module global functions -------------------------------------------------// --- Global functions --------------------------------------------------------int main(void){    uint8_t msglen = 0;    uint8_t msg[BUS_MAXRECVMSGLEN];    uint16_t sender = 0, module_id = 10;    LED_STATUS_DDR |= (1<<LED_STATUS);    LED_ERROR_DDR |= (1<<LED_ERROR);    clk_initialize();    //register_set_u8(MOD_eReg_ModuleID, 13);    register_get(MOD_eReg_ModuleID, 0, &module_id);    bus_configure(&g_bus, module_id);    bus_initialize(&g_bus, 0);// initialize bus on UART 0    spi_master_init_blk();    LED_ERROR_OFF;    eep_initialize();    sei();    clk_timer_start(&g_LED_timer, CLOCK_MS_2_TICKS(1000));    LED_ERROR_ON; LED_STATUS_ON;    if (test_eeprom() == FALSE) {    	LED_ERROR_OFF;    }    while (1) {        // check for message and read it        if (bus_get_message(&g_bus)) {            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {                interpret_message(sender, msglen, msg);            }        }        if (clk_timer_is_elapsed(&g_LED_timer)) {            // TODO remove after debug            clk_timer_start(&g_LED_timer, CLOCK_MS_2_TICKS(1000));            LED_STATUS_TOGGLE;        }    }    return 0;}/** @} */