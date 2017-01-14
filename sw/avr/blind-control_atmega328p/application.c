/**
 * @addtogroup BASIC_APP_ATMEGA328
 * @addtogroup APPLICATION
 * @brief Application specific code of "blind-control_atmega328p" project.
 *
 * Contains application specific initialization, command-interpreter, 
 * register code and background loop.
 * 
 * @todo Implement control of motor relays
 * @todo Implement blind statemachines
 * @todo Implement key-debounce
 * @todo Implement window open/close notifications
 * @todo Implement date and time functions
 * @todo Implement general register receive functions
 *
 * @{
 * @file    application.c
 * @brief   Application specific code of "blind-control_atmega328p" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

#include "blindctrl.h"
#include "bus.h"
#include "cmddef_common.h"
#include "inputs.h"
#include "motor.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static bool g_window_state = false;
static bool g_last_window_state = true;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void send_window_state (sBus_t* bus)
{
    uint8_t msg[4];

    msg[0] = eCMD_STATE_BITFIELDS;
    msg[1] = 1;      // number of bitfiels bytes
    msg[2] = (g_window_state==true) ? (1<<0) : 0;
    msg[3] = (1<<0); // changed bits (here only bit 0)
    bus_send_message(bus, BUS_BRDCSTADR, 4, msg);
}

// --- Module global functions -------------------------------------------------

extern void        app_register_load       (void);

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
    motor_initialize();
    blind_initialize();
    // load application parameters
    app_register_load();
    // initialize window statemachine
    g_window_state = false;
    g_last_window_state = !g_window_state;
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

    if (input_up()) {
        blind_move_to_position(100);
    }
    if (input_down()) {
        blind_move_to_position(0);
    }

    // check window position
    g_window_state = input_window_closed();
    if (g_last_window_state != g_window_state) {
        g_last_window_state = g_window_state;
        send_window_state(bus);
    }

    motor_background();
    blind_background(bus);
}

/** @} */
