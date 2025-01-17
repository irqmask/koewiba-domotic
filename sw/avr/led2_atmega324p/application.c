/**
 * @addtogroup LED2_ATMEGA324P
 * @addtogroup APPLICATION
 * @brief Application specific code of "led2_atmega324p" project.
 *
 * Contains application specific initialization, command-interpreter, 
 * register code and background loop.
 * 
 * @{
 * @file    application.c
 * @brief   Application specific code of "led2_atmega324p" project.
 *///---------------------------------------------------------------------------
/*
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

#include <avr/eeprom.h>
#include <avr/io.h>

#include "cmddef_common.h"
#include "led2chn.h"
#include "prjtypes.h"
#include "register.h"
#include "sleepmode.h"
#include "timer.h"

// --- Definitions -------------------------------------------------------------

//! Interval in milliseconds of background loop
#define BACKGROUND_INTERVAL     10

//! Number of iterations until a button press is signaled
#define BUTTON_SHORT_PRESS      5
//! Number of iterations until a long button press is signaled
#define BUTTON_LONG_PRESS       100

//! Number of iterations until the intensity is saved
#define INTENSITY_SAVE_TIMEOUT  100

//! Pushbutton DDR register
#define BUTTON_DDR      DDRA
//! Pushbutton PORT register
#define BUTTON_PORT     PORTA
//! Pushbutton input PORT register
#define BUTTON_PIN      PINA
//! Pushbutton bit
#define BUTTON_BIT      PA0

#define APP_MAX_CHANNEL 2

// --- Type definitions --------------------------------------------------------

//! States of the LED controller (for all channels)
typedef enum {
    APP_IDLE,			//!< Nothing is currently changing
    APP_LED_ON,			//!< LEDs are switched on.
    APP_LED_CHANGING,   //!< LEDs intensity is changing due to keypress.
    APP_LED_OFF			//!< LEDs are switched off.
} app_states_t;

// --- Local variables ---------------------------------------------------------

static uint8_t g_buttoncnt;
static bool g_button_short_pressed = false;
static timer_data_t g_timer;
static app_states_t g_state = APP_IDLE;
static uint8_t g_led_setpoint[APP_MAX_CHANNEL];
static uint8_t g_led_current[APP_MAX_CHANNEL];
static bool g_led_changing[APP_MAX_CHANNEL];
static uint8_t g_new_intensity_timeout = 0;

static int8_t g_channel_change_dir[APP_MAX_CHANNEL];

// --- Global variables --------------------------------------------------------

extern sBus_t  g_bus;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void button_init(void)
{
    BUTTON_DDR &= ~(1 << BUTTON_BIT);
    BUTTON_PORT |= (1 << BUTTON_BIT);
    g_buttoncnt = 0;
    g_button_short_pressed = false;

    PCMSK0 |= (1<<PCINT0);
}

static void button_background(void)
{
    if ((BUTTON_PIN & (1 << BUTTON_BIT)) == 0) {

        if (g_buttoncnt < BUTTON_LONG_PRESS) g_buttoncnt++;
        if (g_buttoncnt == BUTTON_SHORT_PRESS) g_button_short_pressed = true;
    }
    else {
        g_buttoncnt = 0;
    }
}

static bool button_is_long_pressed(void)
{
    return (g_buttoncnt >= BUTTON_LONG_PRESS);
}

static bool button_is_short_pressed(void)
{
    if (g_button_short_pressed && g_buttoncnt == 0) {
        g_button_short_pressed = false;
        return true;
    }
    return false;
}

static bool button_is_released(void)
{
    return (g_buttoncnt == 0);
}

static void leds_cycle_intensity(void)
{
    if (g_channel_change_dir[0] > 0) {
        if (g_led_setpoint[0] < 255) g_led_setpoint[0]++;
        else {
            g_channel_change_dir[0] = 0;
            g_channel_change_dir[1] = 1;
        }
    } else if (g_channel_change_dir[1] > 0) {
        if (g_led_setpoint[1] < 255) g_led_setpoint[1]++;
        else {
            g_channel_change_dir[0] = -1;
            g_channel_change_dir[1] = 0;
        }
    } else if (g_channel_change_dir[0] < 0) {
        if (g_led_setpoint[0] > 0) g_led_setpoint[0]--;
        else {
            g_channel_change_dir[0] = 0;
            g_channel_change_dir[1] = -1;
        }
    } else if (g_channel_change_dir[1] < 0) {
        if (g_led_setpoint[1] > 0) g_led_setpoint[1]--;
        else {
            g_channel_change_dir[0] = 1;
            g_channel_change_dir[1] = 0;
        }
    }
}

static void send_current_intensity(sBus_t* bus, uint8_t channel)
{
    uint8_t msg[4];

    if (channel > APP_MAX_CHANNEL) return;

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_IntensityChn0Current + channel;
    msg[2] = g_led_current[channel];
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
}

static void send_intensity_setpoint (sBus_t* bus, uint8_t channel)
{
    uint8_t msg[4];

    if (channel > APP_MAX_CHANNEL) return;

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_IntensityChn0SetPoint + channel;
    msg[2] = g_led_setpoint[channel];
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
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
	led2_init();
	button_init();

    timer_start(&g_timer, TIMER_MS_2_TICKS(BACKGROUND_INTERVAL));

    g_state = APP_IDLE;
    g_led_current[0] = 0;
    g_led_current[1] = 0;
    g_led_setpoint[0] = 0;
    g_led_setpoint[1] = 0;
    g_channel_change_dir[0] = 1;
    g_channel_change_dir[1] = 0;
    g_led_changing[0] = false;
    g_led_changing[1] = false;

    // workaround, don't sleep
    sleep_prevent(1<<0, 1);
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
    // no application specific command interpreter here!
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
        timer_start(&g_timer, TIMER_MS_2_TICKS(BACKGROUND_INTERVAL));

        // check buttons
        button_background();
        if (button_is_long_pressed()) {
            if (g_state == APP_LED_OFF) {
            	app_register_get(APP_eReg_IntensityChn0Store, NULL, &g_led_setpoint[0]);
            	app_register_get(APP_eReg_IntensityChn1Store, NULL, &g_led_setpoint[1]);
            }
            g_state = APP_LED_CHANGING;
            leds_cycle_intensity();
            g_new_intensity_timeout = INTENSITY_SAVE_TIMEOUT;
        }
        if (button_is_short_pressed()) {
            switch (g_state) {
            case APP_IDLE:
            case APP_LED_OFF:
                g_state = APP_LED_ON;
            	app_register_get(APP_eReg_IntensityChn0Store, NULL, &g_led_setpoint[0]);
            	send_intensity_setpoint(&g_bus, 0);
            	app_register_get(APP_eReg_IntensityChn1Store, NULL, &g_led_setpoint[1]);
                send_intensity_setpoint(&g_bus, 1);
                break;

            case APP_LED_ON:
                g_state = APP_LED_OFF;
                g_led_setpoint[0] = 0;
                send_intensity_setpoint(&g_bus, 0);
                g_led_setpoint[1] = 0;
                send_intensity_setpoint(&g_bus, 1);
                break;

            default:
                break;
            }
        }
        if (button_is_released()) {
           if (g_state == APP_LED_CHANGING) {
               g_state = APP_LED_ON;
           }
           if (g_new_intensity_timeout > 0) {
               g_new_intensity_timeout--;
               if (g_new_intensity_timeout == 0) {
                   // save intensity
                   app_register_set(APP_eReg_IntensityChn0Store, g_led_setpoint[0]);
                   app_register_set(APP_eReg_IntensityChn1Store, g_led_setpoint[1]);
               }
           }
        }

        // check if intensity changed
        if (g_led_current[0] > g_led_setpoint[0]) {
            g_led_current[0]--;
            led2_set_intensity(0, g_led_current[0]);
            g_led_changing[0] = true;
        }
        else if (g_led_current[0] < g_led_setpoint[0]) {
            g_led_current[0]++;
            led2_set_intensity(0, g_led_current[0]);
            g_led_changing[0] = true;
        }
        else {
            if (g_led_changing[0] == true) {
                send_current_intensity(&g_bus, 0);
            }
            g_led_changing[0] = false;
        }

        if (g_led_current[1] > g_led_setpoint[1]) {
            g_led_current[1]--;
            led2_set_intensity(1, g_led_current[1]);
            g_led_changing[1] = true;
        }
        else if (g_led_current[1] < g_led_setpoint[1]) {
            g_led_current[1]++;
            led2_set_intensity(1, g_led_current[1]);
            g_led_changing[1] = true;
        }
        else {
            if (g_led_changing[1] == true) {
                send_current_intensity(&g_bus, 1);
            }
            g_led_changing[1] = false;
        }
        led2_background();
    }

}

/**
 * Get intensity set-point.
 * @param[in] channel	Channel index starting with 0.
 * @returns Channel intensity set-point.
 */
uint8_t app_led_get_intensity(uint8_t channel)
{
    if (channel > APP_MAX_CHANNEL) return 0;
    return g_led_setpoint[channel];
}

/**
 * Set intensity set-point. LED channels will start changing to this set-point.
 * @param[in] channel	Channel index starting with 0.
 * @param[in] intensity	Intensity value, Range 0..255.
 */
void app_led_set_intensity(uint8_t channel, uint8_t intensity)
{
    if (channel > APP_MAX_CHANNEL) return;
    g_led_setpoint[channel] = intensity;
    if (g_led_setpoint[0] == 0 && g_led_setpoint[1] == 0) {
        g_state = APP_LED_OFF;
    }
    else {
        g_state = APP_LED_ON;
    }
    send_intensity_setpoint(&g_bus, channel);
}

/** @} */
