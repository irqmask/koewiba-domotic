/**
 * @addtogroup DI2RW2_ATTINY1634
 * @addtogroup APPLICATION
 * @brief Application specific code of "di2rw2_attiny1634" project.
 *
 * Contains application specific initialization, command-interpreter,
 * register code and background loop.
 *
 * @{
 * @file    application.c
 * @brief   Application specific code of "di2rw2_attiny1634" project.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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
#include "moddef_common.h"

#include "appconfig.h"
#include "sleepmode.h"
#include "bus.h"

#include "relays_keys.h"
#include "input.h"
#include "register.h"

#include "avr/io.h"

// --- Definitions -------------------------------------------------------------
#define OWN_NODE_ID 0x0a54
#define REC_NODE_ID 0x0a51

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------
extern sBus_t  g_bus;
// --- Module global variables -------------------------------------------------

app_on_key_set_register_t g_on_key_set_register[APP_NUM_KEYS] = {
    { 0, REC_NODE_ID, 77, 100 },
    { 0, REC_NODE_ID, 77,   0 },
};

// --- Local functions ---------------------------------------------------------

static void on_keypress_send (sBus_t* bus, uint8_t key_index)
{
    uint16_t receiver;
    uint8_t msg[4];
    receiver = g_on_key_set_register[key_index].receiver;
    if (receiver != 0) {
        msg[0] = eCMD_SET_REG_8BIT;
        msg[1] = g_on_key_set_register[key_index].register_id;
        msg[2] = g_on_key_set_register[key_index].value;
        bus_send_message(bus, receiver, 3, msg);
    }
}

static void app_check_keys (sBus_t* bus)
{
    uint8_t pressed_keys, index;

    pressed_keys = input_went_low();
    if (pressed_keys != 0x00) sleep_prevent(0x01, 0); // Reset sleep prevention bit as soon as pinchange-interrupt is processed.
    for (index=0; index<APP_NUM_KEYS; index++) {
        if (pressed_keys & (1<<index)) {
            on_keypress_send(bus, index);
        }
    }
}

// --- Module global functions -------------------------------------------------
extern void app_register_load (void);
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
    //register_set_u16(MOD_eReg_ModuleID, OWN_NODE_ID);
    app_register_load();
    relays_keys_init();
    input_initialize();
}

/**
 * Application specific ISR for pin change interrupt.
 *
 * The value of PCIFR is for determining, which register has to be checked for the source of interrupt.
 */
void app_on_pinchangeinterrupt(uint8_t pinchange_interruptflags)
{
    sleep_prevent(0x01, 1); // prevent process from sleeping till pinchange event has been processed.
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
    input_background();
    relays_keys_background();
    app_check_keys(bus);
}


/**
 * Sends the current relay state as broadcast to the bus.
 *
 * @param[in]   index   Index of the relay.
 */
void app_send_relay_state (uint8_t index, uint8_t state)
{
    uint8_t msg[3];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_Output1_State + index;
    msg[2] = (0==state)?0:100;
    bus_send_message(&g_bus, BUS_BRDCSTADR, 3, msg);
}

bool app_call_mode_dep_func(uint8_t reg_no, uint8_t value)
{
    switch (reg_no) {
    // add handler for with application specific registers here!
    case APP_eReg_Output1_State:
        if(value == 0)  relay_switch(0, eRelay_off);
        else            relay_switch(0, eRelay_on);
        app_send_relay_state(0, value);
        break;
    case APP_eReg_Output2_State:
        if(value == 0)  relay_switch(1, eRelay_off);
        else            relay_switch(1, eRelay_on);
        app_send_relay_state(1, value);
        break;
    case APP_eReg_Output3_State:
        if(value == 0)  relay_switch(2, eRelay_off);
        else            relay_switch(2, eRelay_on);
        app_send_relay_state(2, value);
        break;
    case APP_eReg_Output4_State:
        if(value == 0)  relay_switch(3, eRelay_off);
        else            relay_switch(3, eRelay_on);
        app_send_relay_state(3, value);
        break;
    default:
        break;
    }
    return true;
}

/** @} */
