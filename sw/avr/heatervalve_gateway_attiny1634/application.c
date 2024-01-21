/**
 * @addtogroup HEATERVALVE_GATEWAY_ATTINY1634
 * @addtogroup APPLICATION
 * @brief Application specific code of "heatervalve_gateway_attiny1634" project.
 *
 * Contains application specific initialization, command-interpreter,
 * register code and background loop.
 *
 * @{
 * @file    application.c
 * @brief   Application specific code of "heatervalve_gateway_attiny1634" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
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
#include "prjtypes.h"
#include "cmddef_common.h"
#include "register.h"
#include "spi.h"
#include "timer.h"
#include "uart.h"

#include "appconfig.h"
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t g_timer;
static bool     g_blocktransfer_active;
static uint16_t g_remote_temp_curr;
static uint16_t g_remote_temp_setpoint;
static bool     g_send_remote_temp_update;
static char     g_uart_rx_buffer[32];
static uint8_t  g_uart_rx_idx;
static uint16_t g_kp;

// --- Global variables --------------------------------------------------------

extern sBus_t   g_bus;

// --- Module global variables -------------------------------------------------

uint16_t        app_rem_temp_curr_modid;    //!< module id of remote temperature sensor
uint8_t         app_rem_temp_curr_regno;    //!< register number of remote temperature sensor
uint16_t        app_rem_temp_setp_modid;    //!< module id of remote temperature setpoint
uint8_t         app_rem_temp_setp_regno;    //!< register number of remote temperature setpoint
uint16_t        app_debug_receiver;         //!< module id of receiver of debug messages
                                            //!< if set to 0x0000 or 0xFFFF no debug messages will be sent.

// --- Local functions ---------------------------------------------------------

static void send_temp_curr_and_setpoint(void)
{
    if (g_blocktransfer_active) return;

    // send remote temperatures
    // srt <current> <desired>
    uart_put_string_blk1_p("srt ");
    uart_put_hex16_blk1(g_remote_temp_curr);
    uart_put_char_blk1(' ');
    uart_put_hex16_blk1(g_remote_temp_setpoint);
    uart_put_char_blk1('\n');
}

static void send_kp(void)
{
    if (g_blocktransfer_active) return;

    uart_put_string_blk1_p("skp ");
    uart_put_hex16_blk1(g_kp);
    uart_put_char_blk1('\n');
}

static void forward_uart_buffer(void)
{
    uint8_t msg[40];

    if (app_debug_receiver == 0x0000 || app_debug_receiver == 0xFFFF) {
        return;
    }

    // send block data
    msg[0] = eCMD_STATE_STRING;
    for (uint8_t i = 0; i < g_uart_rx_idx; i++) {
        msg[1 + i] = g_uart_rx_buffer[i];
    }
    bus_send_message(&g_bus, app_debug_receiver, 1 + g_uart_rx_idx, msg);
}

// --- Module global functions -------------------------------------------------

bool app_block_start(uint16_t sender, uint8_t blocktype)
{
    (void)sender;
    (void)blocktype;
    spi_master_init_blk();
    g_blocktransfer_active = true;
    return true;
}

bool app_block_end(uint16_t sender, uint8_t blocktype)
{
    (void)sender;
    (void)blocktype;
    uart_init_blk1(UART_BAUDRATE);
    g_blocktransfer_active = false;
    return true;
}

void app_set_kp(uint16_t kp)
{
    g_kp = kp;
    send_kp();
}

uint16_t app_get_kp(void)
{
    return g_kp;
}

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
    register_set_u16(MOD_eReg_ModuleID, 0x520);

    LED_ERROR_DDR |=  (1<<LED_ERROR);
    LED_ERROR_OFF;
    LED_STATUS_DDR |= (1<<LED_STATUS);
    LED_STATUS_OFF;
    uart_init_blk1(UART_BAUDRATE);

    timer_start(&g_timer, TIMER_MS_2_TICKS(1000));

    // load application parameters
    app_register_load();
    g_blocktransfer_active = false;
    g_remote_temp_setpoint = 15 * 100 + 27315;
    g_remote_temp_curr = 15 * 100 + 27315;
    g_uart_rx_idx = 0;
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
    uint8_t regno;
    uint16_t value;

    switch (msg[0]) {
    case eCMD_STATE_16BIT:
        regno = msg[1];
        value = (uint16_t)msg[2] << 8;
        value |= msg[3];
        if (sender == app_rem_temp_curr_modid && regno == app_rem_temp_curr_regno) {
            g_remote_temp_curr = value;
            g_send_remote_temp_update = true;
        }
        else if (sender == app_rem_temp_setp_modid && regno == app_rem_temp_setp_regno) {
            g_remote_temp_setpoint = value;
            g_send_remote_temp_update = true;
        }
        break;

    default:
        break;
    }
}

/**
 * Application specific background code.
 *
 * Executed once per main loop cycle.
 */
void app_background (void)
{
    if (timer_is_elapsed(&g_timer)) {

        timer_start(&g_timer, TIMER_MS_2_TICKS(1000));
    }

    // anything beyond this line will only be executed when no block transfer is active
    if (g_blocktransfer_active) return;
    // ------------------------------------------------------------------------
    if (g_send_remote_temp_update) {
        g_send_remote_temp_update = false;
        send_temp_curr_and_setpoint();
    }

    if (uart_is_rx_pending1()) {
        char c;
        c = uart_get_char_blk1();
        if (c == '\n') {
            g_uart_rx_buffer[g_uart_rx_idx] = '\0';
            if (g_uart_rx_idx > 0 && g_uart_rx_buffer[0] == '#') {
                forward_uart_buffer();
            }
            g_uart_rx_idx = 0;
        } else {
            if (g_uart_rx_idx < (sizeof(g_uart_rx_buffer) - 1)) {
                g_uart_rx_buffer[g_uart_rx_idx++] = c;
            }
        }
    }
}
/** @} */
