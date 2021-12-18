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
#include "alarmclock.h"
#include "datetime.h"
#include "gdisplay.h"
#include "inputs.h"
#include "register.h"
#include "timer.h"
#include "zagwire.h"

// --- Definitions -------------------------------------------------------------

#define APP_TEMP_INCR   50      //!< Temperature increment on keypress
#define APP_INPUT_DOWN  0       //!< Input key "down"
#define APP_INPUT_OK    1       //!< Input key "OK"
#define APP_INPUT_UP    2       //!< Input key "up"

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t g_seconds_timer;
static uint16_t g_last_temperature;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

uint16_t app_current_temp, app_desired_temp;
int16_t app_temp_offset;

// --- Local functions ---------------------------------------------------------

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

// --- Module global functions -------------------------------------------------

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

    app_current_temp = 0;
    g_last_temperature = 0;
    app_desired_temp = 27315 + 2000;
    app_temp_offset = 0;
    timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));

    gdisp_goto_col_line(15, 0);
    gdisp_put_text("ROOMTHERMOSTAT");
}

/**
 * Application specific command interpreter code.
 *
 * Executed if a not-common command is received.
 */
void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    switch (msg[0]) {
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
        if (app_desired_temp < 27315 + 4000 - APP_TEMP_INCR) {
            app_desired_temp += APP_TEMP_INCR;
            register_send_u16(bus, BUS_BRDCSTADR, APP_eReg_TempSetPoint, app_desired_temp);
            gdisp_goto_col_line(0, 2);
            draw_temp(app_desired_temp);

        }
    } else if (input_on_activation(APP_INPUT_DOWN)) {
        if (app_desired_temp > 27315 + APP_TEMP_INCR) {
            app_desired_temp -= APP_TEMP_INCR;
            register_send_u16(bus, BUS_BRDCSTADR, APP_eReg_TempSetPoint, app_desired_temp);
            gdisp_goto_col_line(0, 2);
            draw_temp(app_desired_temp);
        }
    }

    //TODO insert application specific background routines here!
    if (timer_is_elapsed(&g_seconds_timer)) {
        timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
        dt_tick_second();

        // read and display temperature
        if (zagw_get_raw(&raw_temp) == true) {
            gdisp_goto_col_line(65, 2);

            app_current_temp = zagw_get_temperature(raw_temp);
            if (app_current_temp != g_last_temperature) {
                draw_temp(app_current_temp);
                register_send_u16(bus, BUS_BRDCSTADR, APP_eReg_TempCurrent, app_current_temp);
                g_last_temperature = app_current_temp;
            }

        }
        gdisp_goto_col_line(30, 6);
        draw_time();
    }
}

/**
 *  Check every minute if one of the up/down alarms triggers.
 */
void app_on_minute(void)
{
    int8_t alarm_idx = -1;

    if (alarm_check(&alarm_idx)) {
        /// @todo implement timed heater settings
    }
}

/** @} */
