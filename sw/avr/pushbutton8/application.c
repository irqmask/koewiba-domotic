/**
 * @addtogroup PUSHBUTTON8
 * @addtogroup APPLICATION
 * @brief Business logic of the application pushbutton8.
 *
 * @{
 * @file    application.h
 * @brief   Business logic of the application pushbutton8.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#include "cmddef_common.h"
#include "moddef_common.h"

#include "appconfig.h"

#include "bus.h"
#include "ledskeys.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

app_on_key_set_register_t g_on_key_set_register[APP_NUM_KEYS] = {
    { 0, 0x111, 17,   0 },
    { 0, 0x111, 17, 100 },
    { 0, 0x111, 25,   0 },
    { 0, 0x111, 25, 100 },
    { 0, 0x111, 33,   0 },
    { 0, 0x111, 33, 100 },
    { 0, 0x111, 41,   0 },
    { 0, 0x111, 41, 100 },
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

    pressed_keys = key_get_pressed_short(0xFF);
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
    //register_set_u16(MOD_eReg_ModuleID, 0x110);
    //app_register_load();
    leds_keys_init();
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
    leds_keys_background();
    app_check_keys(bus);
}

/** @} */
