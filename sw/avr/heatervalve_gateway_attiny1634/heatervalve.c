/**
 * @addtogroup HEATERVALVE
 *
 * @{
 * @file    heatervalve.c
 * @brief   API for heatervalve sub-module.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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

#include "heatervalve.h"

#include <stdint.h>
#include <avr/io.h>

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif
#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif

// include
#include "cmddef_common.h"
#include "prjtypes.h"

// shared
#include "bus.h"
#include "led_debug.h"
#include "util.h"

// avr shared
#include "uart.h"

// --- Definitions -------------------------------------------------------------

#define UART_RX_BUFLEN 32

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static char     g_uart_rx_buffer[32];
static uint8_t  g_uart_rx_idx;
static uint8_t  g_readidx;

static uint8_t  g_enc_lastval;
static uint16_t g_motor_pos;

// --- Global variables --------------------------------------------------------

extern sBus_t   g_bus;
extern void app_on_encoder_value(int8_t diff);

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void eat_whitespace(void)
{
    while (g_readidx < UART_RX_BUFLEN && g_uart_rx_buffer[g_readidx] == ' ') {
        g_readidx++;
    }
}

static bool starts_with(const char* cmd)
{
    uint8_t idx = 0;

    while (g_uart_rx_buffer[idx] != '\0' && cmd[idx] != '\0' && idx < UART_RX_BUFLEN) {

        if (g_uart_rx_buffer[idx] != cmd[idx]) {
            return false;
        }
        idx++;
    }
    if (cmd[idx] == '\0') {
        g_readidx = idx;
        eat_whitespace();
        return true;
    }
    return false;
}

static void interpret_message(void)
{
    uint8_t val8;

    if (g_uart_rx_idx > 0 && g_uart_rx_buffer[0] == '#') {
        app_send_debug_string(g_uart_rx_buffer);
    }
    else if (starts_with("enc")) {
        if (decode_hex8(&g_uart_rx_buffer[g_readidx], &val8)) {;
            app_on_encoder_value(val8 - g_enc_lastval);
            g_enc_lastval = val8;
        }
    }
    else if (starts_with("mp")) {
        decode_hex16(&g_uart_rx_buffer[g_readidx], &g_motor_pos);
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize heatervalve sub-module
 */
void hv_initialize(void)
{
    // setup reset line
    DDRB &= ~(1<<PB3);
    PORTB &= ~(1<<PB3);

    hv_reset(true);
    uart_init_blk1(UART_BAUDRATE);
    hv_reset(false);

    g_uart_rx_idx = 0;
    g_motor_pos = 0;
}

/**
 * Activate or deactivate heatervalve's reset line.
 * @param[in]   on  true, activate reset line, false, release reset line.
 */
void hv_reset(bool on)
{
    if (on) {
        DDRB |= (1<<PB3);
    }
    else {
        DDRB &= ~(1<<PB3);
    }
}

/**
 * Display decimal value on LCD display with decimal points.
 * @param[in] value     Value to be displayed.
 * @param[in] decpoints Decimalpoint beeing displayed.
 */
void hv_lcd_disp_val(uint16_t value, uint8_t decpoints)
{
    uart_put_string_blk1_p("dv ");
    uart_put_hex16_blk1(value);
    uart_put_char_blk1(' ');
    uart_put_hex8_blk1(decpoints);
    uart_put_char_blk1('\n');
}

/**
 * Display or hide a symbol
 */
void hv_lcd_disp_sym(char symbol, bool on)
{
    uart_put_string_blk1_p("ds ");
    uart_put_char_blk1(symbol);
    uart_put_char_blk1(on + '0');
    uart_put_char_blk1('\n');
}

/**
 * Start motor homing sequence
 */
void hv_motor_homing(void)
{
    uart_put_string_blk1_p("mh\n");
}

/**
 * Start a motor movement to specific position
 * @param[in]   pos     Desired motor position
 */
void hv_motor_move_pos(uint16_t pos)
{
    uart_put_string_blk1_p("mp ");
    uart_put_hex16_blk1(pos);
    uart_put_char_blk1('\n');
}

/**
 * @returns last received motor position
 */
uint16_t hv_motor_get_cached_pos(void)
{
    return g_motor_pos;
}

/**
 * Heatervalve background processing, e.g. listening for incoming messages from sub-module
 */
void hv_background(void)
{
    if (uart_is_rx_pending1()) {
        char c;
        c = uart_get_char_blk1();
        LED_STATUS_TOGGLE;
        if (c == '\n') {
            g_uart_rx_buffer[g_uart_rx_idx] = '\0';
            interpret_message();
            g_uart_rx_idx = 0;
        } else {
            if (g_uart_rx_idx < (sizeof(g_uart_rx_buffer) - 1)) {
                g_uart_rx_buffer[g_uart_rx_idx++] = c;
            }
        }
    }
}

/** @} */
