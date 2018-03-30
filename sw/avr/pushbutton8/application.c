/**
 * @addtogroup TASTER8
 * @addtogroup APPLICATION
 * @brief Business logic of the application taster8.
 *
 * @{
 * @file    application.h
 * @brief   Business logic of the application taster8.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#include "cmddef_common.h"
#include "moddef_common.h"

#include "appconfig.h"
#include "application.h"
#include "bus.h"
#include "ledskeys.h"
#include "prjtypes.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

uint8_t mode_0_mask;
uint8_t mode_1_mask;
uint8_t pressed_keys_mask;
uint8_t old_pressed_keys;
uint8_t shortpressed_keys_mask;

// --- Global variables --------------------------------------------------------

sBus_t  app_bus;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void        send_key_states         (uint8_t uPressedKeys,
                                     uint8_t uChangedKeys)
{
    uint16_t target_moduleids[APP_NUM_KEYS*2], receiver;
    uint8_t ii, msg[4], msglen, num_receiver;

    // build address list
    num_receiver = 0;
    for (ii=0; ii<APP_NUM_KEYS; ii++) {
        if (((1<<ii) & uChangedKeys) == 0) continue;
        // first remote address of channel [ii]
        if (!register_get(APP_eReg_0_RemoteAddr0 + ii, 0, &receiver)) continue;
        if (receiver == BUS_UNKNOWNADR) continue;
        target_moduleids[num_receiver++] = receiver;
        // second remote address of channel [ii]
        if (!register_get(APP_eReg_0_RemoteAddr1 + ii, 0, &receiver)) continue;
        if (receiver == BUS_UNKNOWNADR) continue;
        target_moduleids[num_receiver++] = receiver;
    }

    // send messages
    for (ii=0; ii<num_receiver; ii++) {
        msg[0] = eCMD_STATE_BITFIELDS;
        msg[1] = 1;
        msg[2] = uPressedKeys;
        msg[3] = uChangedKeys;
        msglen = 4;
        bus_send_message(&app_bus, target_moduleids[ii], msglen, msg);
    }
}

void        toggle_on_off           (uint8_t uPressedKeys)
{
    uint16_t receiver;
    uint8_t ii, current_value = 0;

    for (ii=0; ii<APP_NUM_KEYS; ii++) {
        if (((1<<ii) & uPressedKeys) == 0) continue;

        register_get(APP_eReg_0_RemChn0 + ii, 0, &current_value);
        if (current_value) {
            // target value for receiver
            current_value = 0;
            // switch local LED off
            register_set_u8(APP_eReg_0_LEDState + ii, 0);
        }
        else {
            // target value for receiver
            current_value = 255;
            // switch local LED on
            register_set_u8(APP_eReg_0_LEDState + ii, 1);
        }

        register_set_u8(APP_eReg_0_TgtChn0 + ii, current_value);
        if (register_get(APP_eReg_0_RemoteAddr0 + ii, 0, &receiver) &&
            (receiver != BUS_UNKNOWNADR)) {
            register_send_u8(&app_bus, receiver, APP_eReg_0_TgtChn0 + ii, current_value);
        }
        if (register_get(APP_eReg_0_RemoteAddr1 + ii, 0, &receiver) &&
            (receiver != BUS_UNKNOWNADR)) {
            register_send_u8(&app_bus, receiver, APP_eReg_0_TgtChn0 + ii, current_value);
        }
    }
}

// --- Module global functions -------------------------------------------------

void        app_initialize_modes    (void)
{
    uint8_t ii, mode, mask;

    pressed_keys_mask = 0;
    shortpressed_keys_mask = 0;
    mode_0_mask = 0;
    mode_1_mask = 0;
    for (ii=0; ii<APP_NUM_KEYS; ii++) {
        app_register_get(APP_eReg_0_Mode + ii, 0, &mode);
        mask = 1 << ii;
        switch (mode) {
        case 0: // send state of the key (e.g. pressed, released)
            mode_0_mask |= mask;
            pressed_keys_mask |= mask;
            break;
        case 1: // send register on/off
            mode_1_mask |= mask;
            shortpressed_keys_mask |= mask;
            break;
        default:
            // no action
            break;
        }
    }
}

// --- Global functions --------------------------------------------------------

void    app_initialize          (void)
{
    uint8_t ii;

    old_pressed_keys = 0;
    app_initialize_modes();

    for (ii=0; ii<APP_NUM_KEYS; ii++) {
        app_register_set(APP_eReg_0_LEDState + ii, 0);
        app_register_set(APP_eReg_0_RemChn0 + ii, 0);
        app_register_set(APP_eReg_0_RemChn1 + ii, 0);
        app_register_set(APP_eReg_0_RemChn2 + ii, 0);
    }
}

void    app_check_keys          (void)
{
    uint8_t pressed_keys, changed_keys;

    if (pressed_keys_mask != 0) {
        pressed_keys = key_get_pressed(pressed_keys_mask);
        changed_keys = pressed_keys ^ old_pressed_keys;
        old_pressed_keys = pressed_keys;
        // mode 0: send state of the keys
        send_key_states(pressed_keys & mode_0_mask,
                        changed_keys & mode_0_mask);
    }
    if (shortpressed_keys_mask != 0) {
        pressed_keys = key_get_pressed_short(shortpressed_keys_mask);
        // mode 1: toggle on/off register of the receiver
        toggle_on_off(pressed_keys & mode_1_mask);
    }
/*
    // check button
    buttons = PIND & (BTN_TEST | BTN_EXP);
    temp = buttons ^ oldbuttons;
    oldbuttons = buttons;
    if (buttons & BTN_TEST && temp & BTN_TEST) {
        regidx++;
        if (regidx > 7) regidx = 0;

        msg[0] = CMD_eRequestRegister;
        msg[1] = registers[regidx];
        msglen = 2;
        BUS_bSendMessage(&g_sBus, 0x0E, msglen, msg);
    }
    else if (buttons & BTN_EXP && temp & BTN_EXP) {
        if (light)  light = 0;
        else        light = 1;

        msg[0] = CMD_eStateBitfield;
        msg[1] = 1;
        msg[2] = light;
        msg[3] = 0b00000001;
        msglen = 4;
        BUS_bSendMessage(&g_sBus, 12, msglen, msg);
    }*/
}

void        app_factory_reset       (void)
{
    uint8_t ii, size;

    // delete register mapping lookup table
    size = APP_eReg_RemoteAddr31 - APP_eReg_RemoteAddr00 + 1;
    for (ii=0; ii<size; ii++) {
        register_set_u16(APP_eReg_RemoteAddr00 + ii, BUS_UNKNOWNADR);
        register_set_u8(APP_eReg_RemoteReg00 + ii, MOD_eReg_Undefined);
        register_set_u8(APP_eReg_TargetReg00 + ii, MOD_eReg_Undefined);
    }

    // reset mode register to mode 0 and reset send addresses.
    for (ii=0; ii<APP_NUM_KEYS; ii++) {
        register_set_u8(APP_eReg_0_Mode + ii, 0);
        register_set_u16(APP_eReg_0_RemoteAddr0 + ii, BUS_UNKNOWNADR);
        register_set_u16(APP_eReg_0_RemoteAddr1 + ii, BUS_UNKNOWNADR);
    }
}

/** @} */
