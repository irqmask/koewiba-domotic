/**
 * @addtogroup PUSHBUTTON8
 * @addtogroup APPLICATION
 * @brief Business logic of the application pushbutton8.
 *
 * @{
 * @file   application.h
 * @brief   Business logic of the application pushbutton8.
 *
 * @author  Christian Verhalen, Robert Mueller
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#include "cmddef_common.h"
#include "moddef_common.h"

#include "appconfig.h"
#include "sleepmode.h"

#include "bus.h"
#include "input.h"
#include "ledskeys.h"
#include "register.h"

#include "avr/io.h"

// --- Definitions -------------------------------------------------------------

#define OWN_NODE_ID 0x0a53
#define REC_NODE_ID 0x0a52

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------


app_on_key_set_register_t  g_on_key_set_register[APP_NUM_KEYS];
app_on_msg_received_t      g_on_msg_received[APP_NUM_MSG_STUBS];

// --- Module global functions -------------------------------------------------

extern void app_register_load (void);
extern void app_register_set  (uint8_t reg_no, uint32_t value);

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



static void app_execute_message_dependend_function(app_msg_dep_func_t function_id, uint8_t add_info, void* pvalue)
{
    uint8_t value       = 0;
    uint8_t led_idx     = 0;
    uint8_t led_mask    = 0;
    uint8_t led_mode    = 0;
    uint8_t bit_set_idx = 0;
    uint8_t led_reg_val = 0;
    uint8_t i;

    switch (function_id) {
    case eMsgDepFunc_Led_on_bitset: // LED active when bit is set
        value       = *(uint8_t*)pvalue;
        // [bit_idx=0..15 | led_idx=0..15]
        led_idx     = (add_info & 0x0F);
        bit_set_idx = (add_info & 0xF0)>>4;

        if(value & (1<<bit_set_idx))   led_reg_val = 0x0F; // constant full intensity
        else                           led_reg_val = 0x0;
        led_mask = 0x01<<led_idx;
        break;
    case eMsgDepFunc_Led_blindcontrol: // LED active when bit is set
        value       = *(uint8_t*)pvalue;

        // [led_mode=0..15 | led_idx=0..15]
        led_mode    = (add_info & 0xF0)>>4; // [0=off|15=on|2..14=slow..fast blinking]
        led_idx     = (add_info & 0x0F);    // [0=LED0 ... 7=LED7]

        if     (0xF==led_mode)  led_reg_val = 0x0F;
        else if(0x0==led_mode)  led_reg_val = 0x00;
        else                    led_reg_val = led_mode;
        led_mask = 0x11<<led_idx; // switch opposite LED in addition
        break;
    default:
        return;
    }
    for(i=0;i<APP_NUM_KEYS;i++) {
        if(led_mask & (1<<i)) app_register_set(i + APP_eReg_0_LEDState, led_reg_val);
    }
}

static void process_state_message(uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    uint8_t index;

    // Iterate over all stubs
    for (index=0; index<APP_NUM_MSG_STUBS; index++) {
        // check every entry if all criteria are met
        if((g_on_msg_received[index].command       == msg[0]) &&
           (g_on_msg_received[index].register_id   == msg[1]) &&
           (g_on_msg_received[index].receiver      == sender))
        {
            app_execute_message_dependend_function(g_on_msg_received[index].func_id, g_on_msg_received[index].add_info, &msg[2]);
        }
    }
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
    //register_set_u16(MOD_eReg_ModuleID, OWN_NODE_ID);
    app_register_load();
    leds_keys_init();
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
    case eCMD_STATE_8BIT:
        process_state_message(sender, msglen, msg);
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
void app_background (sBus_t* bus)
{
    input_background();
    leds_keys_background();
    app_check_keys(bus);
}

/** @} */
