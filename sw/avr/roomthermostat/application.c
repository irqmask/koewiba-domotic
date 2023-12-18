/**
 * @addtogroup ROOMTHERMOSTAT
 * @addtogroup APPLICATION
 * @brief Application specific code of "roomthermostat_atmega328p" project.
 *
 * Contains application specific initialization, command-interpreter,
 * register code and background loop.
 *
 * @{
 * @file    application.c
 * @brief   Application specific code of "roomthermostat_atmega328p" project.
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
#include "cmddef_common.h"
#include "alarmclock.h"
#include "datetime.h"
#include "gdisplay.h"
#include "inputs.h"
#include "register.h"
#include "timer.h"
#include "zagwire.h"

#include "disp_sh1106.h"

#include "application.h"

// --- Definitions -------------------------------------------------------------

#define APP_TEMP_INCR       50  //!< Temperature increment on keypress
#define APP_INPUT_DOWN      0   //!< Input key "down"
#define APP_INPUT_OK        1   //!< Input key "OK"
#define APP_INPUT_UP        2   //!< Input key "up"
#define APP_MAX_CONTACTS    2   //!< Maximum number of window contacts

#define DISP_TIMEOUT_DIM    10  //!< timeout when display gets dimmed
#define DISP_TIMEOUT_OFF    60  //!< timeout when display is switched off

#define TEMP_GLITCH_LIMIT   500 //!< absolute glitch limit +/- in Kelvin

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// volatile data
static timer_data_t g_seconds_timer;
static uint16_t     g_last_temperature;
static uint8_t      g_display_timeout;
static uint8_t      g_temperature_glitches;
static uint16_t     g_old_desired_temp;
static uint8_t      g_window_open;

// register data
static int16_t      g_temperature_offset;
static uint16_t     g_windowcontact_moduleid;
static uint8_t      g_windowcontact_reg[APP_MAX_CONTACTS];
static uint16_t     g_window_open_temperature;
static uint16_t     g_vacation_temperature;
static uint8_t      g_mode;


// --- Global variables --------------------------------------------------------

extern sBus_t  g_bus;

// --- Module global variables -------------------------------------------------

uint16_t app_current_temp, app_desired_temp;

// --- Local functions ---------------------------------------------------------

void draw_value8(uint8_t v)
{
    char c;
    gdisp_choose_font(gdisp_font1_x16);
    c = v / 100;
    v = v % 100;
    gdisp_put_char(c + '0');
    c = v / 10;
    v = v % 10;
    gdisp_put_char(c + '0');
    c = v;
    gdisp_put_char(c + '0');
}

void draw_temp (uint16_t temperature)
{
    uint8_t byte, firstdigit = 0;

    if (temperature >= 27315) {
        temperature -= 27315;

        gdisp_choose_font(gdisp_font1_x16);
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

void draw_time(void)
{
    uint8_t c, v;

    gdisp_choose_font(gdisp_font1_x16);
    v = dt_get_hour();
    c = (v / 10) + '0';
    gdisp_put_char(c);
    c = (v % 10) + '0';
    gdisp_put_char(c);
    gdisp_put_char(':');

    v = dt_get_minute();
    c = (v / 10) + '0';
    gdisp_put_char(c);
    c = (v % 10) + '0';
    gdisp_put_char(c);
    gdisp_put_char(':');

    v = dt_get_second();
    c = (v / 10) + '0';
    gdisp_put_char(c);
    c = (v % 10) + '0';
    gdisp_put_char(c);
}

static void draw_window(uint8_t open)
{
    gdisp_choose_font(gdisp_symbols_x24);
    if (open != 0) {
        gdisp_put_char(34);
    }
    else {
        gdisp_put_char(33);

    }
}

static void reset_display_timeout(void)
{
    if (g_display_timeout > DISP_TIMEOUT_DIM) {
        sh1106_contrast(0xE0);
        sh1106_display_on(1);
    }
    g_display_timeout = 0;
}

static void check_display_timeout(void)
{
    if (g_display_timeout < 255) g_display_timeout++;
    if (g_display_timeout == DISP_TIMEOUT_DIM) {
        sh1106_contrast(0);
    }
    else if (g_display_timeout == DISP_TIMEOUT_OFF) {
        sh1106_display_on(0);
    }
}


static void set_remote_window_state(uint8_t open)
{
    g_window_open = open;
    if (open != 0) {
        if (g_old_desired_temp == 0) {
            g_old_desired_temp = app_desired_temp;
        }
        app_set_desired_temp(g_window_open_temperature, true);
    }
    else {
        if (g_mode == eMODE_VACATION) {
            app_set_desired_temp(g_vacation_temperature, true);
        }
        else if (g_old_desired_temp != 0) {
            app_set_desired_temp(g_old_desired_temp, true);
            g_old_desired_temp = 0;
        }
    }
    app_draw_desired_temp();
    app_draw_window();
    reset_display_timeout();
}

static void on_cmd_state_8bit_received(uint16_t sender, uint8_t reg, uint8_t value)
{
    if (sender == g_windowcontact_moduleid &&
        ((reg == g_windowcontact_reg[0]) || (reg == g_windowcontact_reg[1]))) {
        set_remote_window_state(value);
    }
}

// --- Module global functions -------------------------------------------------

void app_set_desired_temp(uint16_t desired_temp, bool publish)
{
	if (desired_temp < 27315 || desired_temp > 27315 + 4000)
		return;

    app_desired_temp = desired_temp;
    app_draw_desired_temp();
    if (publish) {
    	register_send_u16(&g_bus, BUS_BRDCSTADR, APP_eReg_TempSetPoint, app_desired_temp);
    }
}

void app_set_temperature_offset(int16_t offset)
{
    g_temperature_offset = offset;
}

int16_t app_get_temperature_offset(void)
{
    return g_temperature_offset;
}

void app_set_windowcontact_moduleid(uint16_t moduleid)
{
    g_windowcontact_moduleid = moduleid;
}

uint16_t app_get_windowcontact_moduleid(void)
{
    return g_windowcontact_moduleid;
}

void app_set_windowcontact_reg(uint8_t contact_idx, uint8_t reg)
{
    if (contact_idx >= APP_MAX_CONTACTS)
    {
        return;
    }
    g_windowcontact_reg[contact_idx] = reg;
}

uint8_t app_get_windowcontact_reg(uint8_t contact_idx)
{
    if (contact_idx >= APP_MAX_CONTACTS)
    {
        return 0;
    }
    return g_windowcontact_reg[contact_idx];
}

void app_set_mode(uint8_t mode)
{
    if (g_mode == mode) return;

    g_mode = mode;
    switch (g_mode) {
    case eMODE_NORMAL:
        if (g_window_open) {
            return;
        }
        else if (g_old_desired_temp != 0) {
            app_set_desired_temp(g_old_desired_temp, true);
            g_old_desired_temp = 0;
        }
        break;

    case eMODE_VACATION:
        if (g_window_open == 0) {
            if (g_old_desired_temp == 0) {
                g_old_desired_temp = app_desired_temp;
                app_set_desired_temp(g_vacation_temperature, true);
            }
        }
        break;

    default:
        break;
    }
}

uint8_t app_get_mode(void)
{
    return g_mode;
}

void app_set_windowopen_temperature(uint16_t temp)
{
    g_window_open_temperature = temp;
}

uint16_t app_get_windowopen_temperature(void)
{
    return g_window_open_temperature;
}

void app_set_vacation_temperature(uint16_t temp)
{
    g_vacation_temperature = temp;
}

uint16_t app_get_vacation_temperature(void)
{
    return g_vacation_temperature;
}

void app_draw_desired_temp(void)
{
    gdisp_goto_col_line(0, 2);
    draw_temp(app_desired_temp);
}

void app_draw_current_temp(void)
{
    gdisp_goto_col_line(65, 2);
    draw_temp(app_current_temp);
}

void app_draw_window()
{
    gdisp_goto_col_line(45, 2);
    draw_window(g_window_open);
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
    input_initialize();
    gdisp_initialize();
    zagw_initialize();
    zagw_enable(true);
    dt_initialize();
    alarm_initialize();

    //TODO insert application specific initializations here!
    //register_set_u16(MOD_eReg_ModuleID, 0x00F);

    g_temperature_glitches = 0;
    g_display_timeout = DISP_TIMEOUT_DIM - 1;

    // initialize volatile registers
    app_current_temp = 27315 + 1000;
    app_desired_temp = 27315 + 2000;
    g_last_temperature = 0;
    g_old_desired_temp = 0;
    g_window_open = 0;

    // load application parameters
    app_register_load();

    timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));

    // display first screen
    gdisp_goto_col_line(15, 0);
    gdisp_put_text("ROOMTHERMOSTAT");
    app_draw_desired_temp();
    app_draw_current_temp();
    app_draw_window();
    zagw_start_sampling();
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
    case eCMD_STATE_8BIT:
        if (msglen >= 3)
            on_cmd_state_8bit_received(sender, msg[1], msg[2]);
        break;

    //TODO insert application specific command interpreter here!
    default:
        break;
    }
}

/**
 * Application specific background code.
 *
 * Executed once per main loop cycle.
 */
void app_background (sBus_t* bus)
{
    uint16_t raw_temp;

    input_background();

    if (input_on_activation(APP_INPUT_UP)) {
        reset_display_timeout();
        app_set_desired_temp(app_desired_temp + APP_TEMP_INCR, true);
    } else if (input_on_activation(APP_INPUT_DOWN)) {
        reset_display_timeout();
        app_set_desired_temp(app_desired_temp - APP_TEMP_INCR, true);
    }

    // do every 1s ...
    if (timer_is_elapsed(&g_seconds_timer)) {
        timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
        dt_tick_second();

        // read and display temperature
        if (zagw_sampling_done()) {
            raw_temp = zagw_get_raw();
            zagw_start_sampling();
            if (raw_temp != 0) {
                app_current_temp = zagw_get_temperature(raw_temp);
                app_current_temp += g_temperature_offset;
                if (app_current_temp != g_last_temperature) {
                    app_draw_current_temp();
                    register_send_u16(bus, BUS_BRDCSTADR, APP_eReg_TempCurrent, app_current_temp);

                    uint16_t diff;
                    if (app_current_temp > g_last_temperature)
                        diff = app_current_temp - g_last_temperature;
                    else
                        diff = g_last_temperature - app_current_temp;

                    if (diff > TEMP_GLITCH_LIMIT) {
                        g_temperature_glitches++;
                        //gdisp_goto_col_line(0, 4);
                        //draw_value8(g_temperature_glitches);
                    }
                    g_last_temperature = app_current_temp;
                }
            }
        }

        gdisp_goto_col_line(30, 6);
        draw_time();

        check_display_timeout();
    }
}

/**
 *  Check every minute if one of the up/down alarms triggers.
 */
void app_on_minute(void)
{
    int8_t alarm_idx = -1;

    if (alarm_check(&alarm_idx)) {
        uint16_t setpoint = app_get_alarm_setpoint(alarm_idx);
        app_set_desired_temp(setpoint, true);
    }
}

/** @} */
