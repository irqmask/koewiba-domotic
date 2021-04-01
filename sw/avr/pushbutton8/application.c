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
#include "sleepmode.h"

#include "bus.h"
#include "ledskeys.h"
#include "register.h"

#include "avr/io.h"

// --- Definitions -------------------------------------------------------------

#define OWN_NODE_ID 0x51

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

#if (OWN_NODE_ID == 0x50)
app_on_key_set_register_t g_on_key_set_register[APP_NUM_KEYS] = {
    { 0, 0x051, 17, 100 },
    { 0, 0x051, 25, 100 },
    { 0, 0x051, 33, 100 },
    { 0, 0x051, 41, 100 },
    { 0, 0x051, 17,   0 },
    { 0, 0x051, 25,   0 },
    { 0, 0x051, 33,   0 },
    { 0, 0x051, 41,   0 },
};
#elif (OWN_NODE_ID == 0x51)
app_on_key_set_register_t g_on_key_set_register[APP_NUM_KEYS] = {
    { 0, 0x050, 17, 100 },
    { 0, 0x050, 25, 100 },
    { 0, 0x050, 33, 100 },
    { 0, 0x050, 41, 100 },
    { 0, 0x050, 17,   0 },
    { 0, 0x050, 25,   0 },
    { 0, 0x050, 33,   0 },
    { 0, 0x050, 41,   0 },
};
#endif

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
    register_set_u16(MOD_eReg_ModuleID, OWN_NODE_ID);
    //app_register_load();
    leds_keys_init();
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
    leds_keys_background();
    app_check_keys(bus);
}

/** @} */
