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
#include "contacts.h"
#include "control_temp.h"
#include "heatervalve.h"
#include "led_debug.h"
#include "messaging.h"
#include "remote_tempsense.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t g_timer;
static timer_data_t g_timer10s;
static bool     g_blocktransfer_active;

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

void display_temp_setpoint(void)
{
    uint16_t tempdec = (remts_get_desired_temp() - 27315) / 10;
    hv_lcd_disp_val(tempdec, 1); // temperature in 1/10 °C -> one decimal place
    hv_lcd_disp_sym('d', true);
}

void forward_temp_setpoint(void)
{
    uint8_t msg[4];

    uint16_t value = remts_get_desired_temp();

    // send block data
    msg[0] = eCMD_SET_REG_16BIT;
    msg[1] = app_rem_temp_setp_regno;
    msg[2] = value >> 8;
    msg[3] = value & 0x00FF;
    bus_send_message(&g_bus, app_rem_temp_setp_modid, sizeof(msg), msg);
}

// --- Module global functions -------------------------------------------------

void app_send_debug_string(const char* str)
{
    uint8_t msg[BUS_MAXSENDMSGLEN];

    if (app_debug_receiver == 0x0000 || app_debug_receiver == 0xFFFF) {
        return;
    }

    // send block data
    msg[0] = eCMD_STATE_STRING;
    uint8_t i = 1;
    while (i < sizeof(msg) && *str != '\0') {
        msg[i] = *str++;
        i++;
    }
    message_send(app_debug_receiver, i, msg);
}

bool app_block_start(uint16_t sender, uint8_t blocktype)
{
    (void)sender;
    (void)blocktype;
    spi_master_init_blk();
    hv_reset(true);
    g_blocktransfer_active = true;
    return true;
}

bool app_block_end(uint16_t sender, uint8_t blocktype)
{
    (void)sender;
    (void)blocktype;
    uart_init_blk1(UART_BAUDRATE);
    hv_initialize();
    g_blocktransfer_active = false;
    return true;
}

void app_on_encoder_value(int8_t diff)
{
    int16_t diff16 = diff;
    diff16 *= 50;
    remts_set_desired_temp(remts_get_desired_temp() + diff16);
    display_temp_setpoint();
    forward_temp_setpoint();
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
    //register_set_u16(MOD_eReg_ModuleID, 0xa60);
    LED_ERROR_DDR |=  (1<<LED_ERROR);
    LED_ERROR_OFF;
    LED_STATUS_DDR |= (1<<LED_STATUS);
    LED_STATUS_OFF;
    hv_initialize();
    ctrl_temp_initialize();
    remts_initialize();

    timer_start(&g_timer, TIMER_MS_2_TICKS(1000));
    timer_start(&g_timer10s, TIMER_MS_2_TICKS(10000));

    // load application parameters
    app_register_load();

    g_blocktransfer_active = false;

    contacts_initialize();
    PCMSK0 |= ((1<<PCINT1) | (1<<PCINT2)); // wake-up by pin change on contact inputs
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
            remts_set_current_temp(value);
        }
        else if (sender == app_rem_temp_setp_modid && regno == app_rem_temp_setp_regno) {
            remts_set_desired_temp(value);
            display_temp_setpoint();
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
        LED_ERROR_TOGGLE;
    }

    contacts_background();

    // anything beyond this line will only be executed when no block transfer is active
    if (g_blocktransfer_active) return;
    // ------------------------------------------------------------------------

    if (timer_is_elapsed(&g_timer10s)) {
        timer_start(&g_timer10s, TIMER_MS_2_TICKS(10000));
        ctrl_temp_background_10s();
    }

    hv_background();
}
/** @} */
