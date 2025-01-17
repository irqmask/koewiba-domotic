/**
 * @addtogroup TEST_ROOMTHERMOSTAT_ATMEGA328
 *
 * @{
 * @file    main_test_bus-module_atmega328.c
 * @brief   Test hardware of bus-module_atmega328.
 * 
 * Test hardware of bus-module_atmega328:
 *  - EEProm 
 *  - RS485 transmitter (bus communication)
 *  - Zagwire temperature sensor
 *  - Display
 *  - Buttons
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
#include <avr/pgmspace.h>

#include "appconfig.h"
#include "pcbconfig.h"

#include "cmddef_common.h"
#include "moddef_common.h"

#include "alarmclock.h"
#include "bus.h"
#include "gdisplay.h"
#include "eeprom_spi.h"
#include "led_debug.h"
#include "register.h"
#include "spi.h"
#include "timer.h"
#include "zagwire.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t           g_bus;
static timer_data_t     g_LED_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

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

void display_space_x8(void)
{
    uint8_t ii;

    for (ii=0; ii<8; ii++)
        gdisp_put_spacer(8);
}

void display_dec_u8(uint8_t value)
{
    uint8_t digit, draw_next_digit = 0;
    if (value >= 100) {
        digit = value / 100;
        value %= 100;
        gdisp_put_char('0' + digit);
        draw_next_digit = 1;
    } else {
        display_space_x8();
    }
    if (value >= 10 || draw_next_digit) {
        digit = value / 10;
        value %= 10;
        gdisp_put_char('0' + digit);
    } else {
        display_space_x8();
    }
    gdisp_put_char('0' + value);
}

void draw_temp (uint16_t temperature)
{
    uint8_t byte, firstdigit = 0;

    if (temperature >= 27315) {
        temperature -= 27315;

        byte = temperature / 10000;
        if (byte > 0) {
            temperature -= byte * 10000;
        }

        byte = temperature / 1000;
        if (byte > 0 || firstdigit) {
            temperature -= byte * 1000;
            gdisp_put_char(byte + 0x30);
            firstdigit = 1;
        } else {
            gdisp_put_char(' ');
        }

        byte = temperature / 100;
        if (byte > 0 || firstdigit) {
            temperature -= byte * 100;
            gdisp_put_char(byte + 0x30);
            firstdigit = 1;
        } else {
            gdisp_put_char(' ');
        }
        gdisp_put_char(',');
        byte = temperature / 10;
        temperature -= byte * 10;
        gdisp_put_char(byte + 0x30);
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

extern void test_eeprom_start(void);
extern uint8_t test_eeprom_run(void);
extern uint8_t test_eeprom_is_ok(void);
extern uint8_t test_eeprom_get_progress(void);

int main(void)
{
    uint8_t msglen = 0, pinc, oldpinc;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t sender = 0, module_id = 0x20, raw_temp = 0, temperature = 0;

    LED_STATUS_DDR |= (1<<LED_STATUS);
    LED_ERROR_DDR |= (1<<LED_ERROR);
    timer_initialize();

    // initialize keys
    PINC &= ~(1<<PC2 | 1<<PC1 | 1<<PC0);
    PORTC |= (1<<PC2 | 1<<PC1 | 1<<PC0);

    //register_set_u16(MOD_eReg_ModuleID, 0x20);
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

    LED_ERROR_ON; LED_STATUS_ON;
    spi_master_init_blk();
    eep_initialize();
    gdisp_initialize();
    zagw_initialize();
    sei();

    gdisp_set_startline(0);
    timer_start(&g_LED_timer, TIMER_MS_2_TICKS(1000));
    LED_ERROR_OFF; LED_STATUS_OFF;

    gdisp_goto_col_line(0, 0);
    gdisp_put_text("Test Roomthermostate V");
    gdisp_put_char('0' + pgm_read_byte(&app_versioninfo[MOD_eVerAppMajor]));
    gdisp_put_char('.');
    gdisp_put_char('0' + pgm_read_byte(&app_versioninfo[MOD_eVerAppMinor]));

    // start EEProm test
    test_eeprom_start();
    gdisp_goto_col_line(0, 2);
    display_dec_u8(module_id & 0x00ff);
    gdisp_goto_col_line(0, 3);
    gdisp_put_text("EEProm Status: ");

    // initialize buttons
    pinc = PINC & (1<<PC2 | 1<<PC1 | 1<<PC0);
    oldpinc = ~pinc;

    // start temperature measurement
    gdisp_goto_col_line(0, 4);
    gdisp_put_text("Temperature:       °C");

    // start main-loop and process incoming and outgoing messages
    // ------------------------------------------------------

    while (1) {
        // run EEProm test

        uint8_t eep_test_running = test_eeprom_run();
        if (eep_test_running == 0) {
            uint8_t test_passed = test_eeprom_is_ok();
            gdisp_goto_col_line(0, 3);
            if (test_passed == 0) {
                gdisp_put_text("EEProm Status:  ERR");
                msg[1] = 0x00; // 1 = test succeeded, 0 = test failed
            } else {
                gdisp_put_text("EEProm Status:    OK");
                msg[1] = 0x01; // 1 = test succeeded, 0 = test failed
            }
            // report EEProm test status
            msg[0] = 0xEF; // currently unused command number
            bus_send_message(&g_bus, 0x0002, 2, msg); // send message to address 0x0002
        }

        // check for message and read it
        if (bus_get_message(&g_bus)) {
            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
        }

        // toggle status LED once a second
        if (timer_is_elapsed(&g_LED_timer)) {
            timer_start(&g_LED_timer, TIMER_MS_2_TICKS(1000));
            LED_STATUS_TOGGLE;
            if (eep_test_running == 1) {
                uint8_t val = test_eeprom_get_progress();
                gdisp_goto_col_line(80, 3);
                display_dec_u8(val);
                gdisp_put_char('%');
            }
        }

        // read and display key states
        pinc = PINC & (1<<PC2 | 1<<PC1 | 1<<PC0);
        if (oldpinc != pinc) {
            gdisp_goto_col_line(0, 6);
            if (pinc & (1<<PC0)) gdisp_put_char('0'); else gdisp_put_char('1');
            gdisp_goto_col_line(62, 6);
            if (pinc & (1<<PC1)) gdisp_put_char('0'); else gdisp_put_char('1');
            gdisp_goto_col_line(116, 6);
            if (pinc & (1<<PC2)) gdisp_put_char('0'); else gdisp_put_char('1');
        }
        oldpinc = pinc;

        // read and display temperature
        if (zagw_sampling_done()) {
            raw_temp = zagw_get_raw();
            zagw_start_sampling();
            gdisp_goto_col_line(76, 4);
            temperature = zagw_get_temperature(raw_temp);
            draw_temp(temperature);
        }
    }

    return 0;
}

/** @} */
