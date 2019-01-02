/**
 * @addtogroup ROOMTHERMOSTAT
 *
 * @{
 * @file    main_roomthermostat.c
 * @brief   Main entry point of roomthermostat.
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

#include "cmddef_common.h"

#include "bus.h"
#include "clock.h"
#include "gdisplay.h"
#include "register.h"
#include "sleepmode.h"
#include "spi.h"
#include "ucontroller.h"
#include "zagwire.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t           g_bus;
static clock_timer_t    g_app_timer;
static uint16_t         g_target_temp;
static uint16_t         g_current_temp;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void draw_nibble (uint8_t nibble)
{
    nibble &= 0x0F;
    if (nibble > 9) {
        nibble -= 10;
        nibble += 'A';
    } else {
        nibble += '0';
    }
    gdisp_put_char(nibble);
}

void draw_hex8_value (uint8_t value)
{
    gdisp_put_char('0');
    gdisp_put_char('x');
    draw_nibble(value >> 4);
    draw_nibble(value & 0x0F);
}

void draw_hex16_value (uint16_t value)
{
    gdisp_put_char('0');
    gdisp_put_char('x');
    draw_nibble((value >> 12) & 0x000F);
    draw_nibble((value >> 8) & 0x000F);
    draw_nibble((value >> 4) & 0x000F);
    draw_nibble(value & 0x000F);
}

void draw_temp (uint16_t temperature)
{
    uint8_t byte;
    bool firstdigit = false;

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
            firstdigit = true;
        } else {
            gdisp_put_char(' ');
        }

        byte = temperature / 100;
        if (byte > 0 || firstdigit) {
            temperature -= byte * 100;
            gdisp_put_char(byte + 0x30);
            firstdigit = true;
        } else {
            gdisp_put_char(' ');
        }
        gdisp_put_char(',');
        byte = temperature / 10;
        temperature -= byte * 10;
        gdisp_put_char(byte + 0x30);
    }
}

void draw_temperatures (void)
{
    gdisp_goto_col_line(0, 1);
    gdisp_choose_font(gdisp_font1_x16);
    draw_temp(g_current_temp);
    gdisp_choose_font(gdisp_symbols_x16);
    gdisp_put_text(" "); //32
    gdisp_choose_font(gdisp_font1_x16);
    gdisp_put_text("C");

    gdisp_goto_col_line(61, 1);
    gdisp_choose_font(gdisp_font1_x16);
    draw_temp(g_target_temp);
    gdisp_choose_font(gdisp_symbols_x16);
    gdisp_put_text(" "); //32
    gdisp_choose_font(gdisp_font1_x16);
    gdisp_put_text("C");
}

void draw_window_opened (void)
{
    gdisp_goto_col_line(118, 0);
    gdisp_choose_font(gdisp_symbols_x24);
    gdisp_put_text("\""); //34
}

void draw_window_closed (void)
{
    gdisp_goto_col_line(118, 0);
    gdisp_choose_font(gdisp_symbols_x24);
    gdisp_put_text("!"); //33
}

// Interpret message
static void interpret_message (sBus_t* bus, uint8_t* msg, uint8_t msg_len, uint16_t sender)
{
    if (msg[0] <= eCMD_STATE_DATE_TIME) {
        // state messages
        if (msg[0] == eCMD_STATE_BITFIELDS) {
            if (sender == 0x0B) {
                g_target_temp += 10;
                draw_temperatures();
            } else {
                if (msg[2] & 0b00000001) draw_window_opened();
                else                     draw_window_closed();
            }
        }

    } else if (msg[0] <= eCMD_SET_REG_32BIT) {
        if (msg[0] == eCMD_SET_REG_16BIT && msg[1] == APP_eReg_CurrentDesiredTemperature) {
            g_target_temp = msg[2];
            g_target_temp <<= 8;
            g_target_temp |= msg[3];
            draw_temperatures();
            if (g_target_temp %2) draw_window_opened();
            else draw_window_closed();
        }
        // register messages
        register_do_command(bus, sender, msg_len, msg);
    } else {
        // system messages
        switch (msg[0]) {
        case eCMD_ACK:
            g_bus.eModuleState = eMod_Running;
            break;
        case eCMD_SLEEP:
            sleep_pinchange_enable();
            bus_sleep(bus);
            sleep_pinchange_disable();
            break;
        default:
            break;
        }
    }
}

static void send_temperature(uint16_t temperature)
{
    uint8_t msg[4];
    msg[0] = eCMD_STATE_16BIT;
    msg[1] = APP_eReg_CurrentTemperature;
    msg[2] = temperature >> 8;
    msg[3] = temperature & 0x00FF;
    bus_send_message(&g_bus, 0x0000, 4, msg);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main (void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t module_id = BUS_UNKNOWNADR, sender = 0, val;
    uint16_t new_temp;

    DDRC |= ((1<<PC3) | (1<<PC4));
    PORTC &= ~((1<<PC3) | (1<<PC4));

    clk_initialize();

    //register_set_u16(MOD_eReg_ModuleID, 0x000E);

    // configure a bus node with address X
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

    spi_master_init_blk();
    zagw_initialize();

    sei();

    register_get(APP_eReg_DesiredTempDay1, 0, &g_target_temp);

    gdisp_initialize();
    gdisp_goto_col_line(0,0);
    gdisp_put_text("aktuell");

    gdisp_goto_col_line(61,0);
    gdisp_put_text("Soll");

    draw_temperatures();
    draw_window_closed();

    gdisp_choose_font(gdisp_font1_x8);
    gdisp_goto_col_line(0,3);
    gdisp_put_text("Addr: ");
    draw_hex16_value(g_bus.sCfg.uOwnNodeAddress);

    clk_timer_start(&g_app_timer, 100);
    while (1) {
        if (bus_get_message(&g_bus)) {
            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
                interpret_message(&g_bus, msg, msglen, sender);
            }
        }
        if (clk_timer_is_elapsed(&g_app_timer)) {
            clk_timer_start(&g_app_timer,10);
            gdisp_choose_font(gdisp_font1_x8);
            gdisp_goto_col_line(0,3);
            gdisp_put_text("Temp: ");
            if (zagw_receive()) {
                val = zagw_get_bits();
                draw_hex16_value(val);
                new_temp = zagw_get_temperature();
                if (new_temp != g_current_temp) {
                    send_temperature(new_temp);
                }
                g_current_temp = new_temp;
                draw_temperatures();
            } else {
                gdisp_put_text("PERR");
            }
        }
    }
    return 0;
}

/** @} */
