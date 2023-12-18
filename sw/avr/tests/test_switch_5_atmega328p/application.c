/**
 * @addtogroup TEST_SWITCH_5_ATMEGA328
 * @addtogroup APPLICATION
 * @brief Application specific code of "test_switch_5_atmega328" project.
 *
 * Test hardware: eeprom, inputs, outputs. Therefore connect input 0 - 4 to
 * output 0 - 4.
 * 
 * @{
 * @file    application.c
 * @brief   Application specific code of "test_switch_5_atmega328" project.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2019  robert <djrfm@gmx.de>
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

#include "eeprom_spi.h"
#include "register.h"
#include "sleepmode.h"

// --- Definitions -------------------------------------------------------------

#define TEST_EEPROM_MASK    (1<<0)  //!< bit is set if EEProm test succeeds
#define TEST_RELAY_MASK     (1<<1)  //!< bit is set if relay test succeeds

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t g_test_result_msg[2];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern void    test_eeprom_start(void);
extern uint8_t test_eeprom_run(void);
extern uint8_t test_eeprom_is_ok(void);

extern void test_io_initialize(void);
extern uint8_t test_io_run(void);
extern uint8_t test_io_is_ok(void);

// --- Global functions --------------------------------------------------------

/**
 * Application specific initializations.
 * 
 * Executed if common code initialization passed.
 * @note Global interrupts are still switched off! Will be switched on in global
 * main after this initialization code.
 */
void app_init (void) 
{
    //TODO insert application specific initializations here!
    //register_set_u16(MOD_eReg_ModuleID, 0x20);
    test_eeprom_start();
	test_io_initialize();

    g_test_result_msg[0] = 0xEF; // currently unused command number
    g_test_result_msg[1] = 0;

    sleep_prevent(TEST_EEPROM_MASK | TEST_RELAY_MASK, true);
}

/**
 * Application specific ISR for pin change interrupt.
 *
 * @param[in] pinchange_interruptflags  The value of PCIFR is for determining, 
 *                                      which register has to be checked for the
 *                                      source of interrupt.
 */
void app_on_pinchangeinterrupt(uint8_t pinchange_interruptflags)
{
    (void)pinchange_interruptflags;
    // nothing to do, here
}

/**
 * Application specific command interpreter code.
 * 
 * Executed if a not-common command is received.
 */
void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    switch (msg[0]) {
    // no commands are processed
    default:
        break;
    }
}

/**
 * Application specific background code.
 * 
 * Executed once per main loop cycle.
 */
void app_background (sBus_t* g_bus)
{
	uint8_t eep_test_running, io_test_running;

	eep_test_running = test_eeprom_run();
	if (eep_test_running == 0) {
	    sleep_prevent(TEST_EEPROM_MASK, false);

		if (test_eeprom_is_ok() == 0) {
		    g_test_result_msg[1] &= ~TEST_EEPROM_MASK;
		} else {
		    g_test_result_msg[1] |= TEST_EEPROM_MASK;
		}
		// report EEProm test status
		bus_send_message(g_bus, 0x0000, sizeof(g_test_result_msg), g_test_result_msg);
	}

	io_test_running = test_io_run();
	if (io_test_running == 0) {
        sleep_prevent(TEST_RELAY_MASK, false);

        if (test_io_is_ok() == 0) {
		    g_test_result_msg[1] &= ~TEST_RELAY_MASK;
		} else {
		    g_test_result_msg[1] |= TEST_RELAY_MASK;
		}
		// report relay test status
		bus_send_message(g_bus, 0x0000, sizeof(g_test_result_msg), g_test_result_msg); // send message to address 0x0002
	}
}
/** @} */
