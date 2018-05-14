/**
 * @addtogroup BLINDCONTROL
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
#include "datetime.h"
#include "inputs.h"
#include "motor.h"
#include "register.h"
#include "timer.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static bool g_window_state = false;
static bool g_last_window_state = true;
static timer_data_t g_seconds_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void send_window_state (sBus_t* bus)
{
    uint8_t msg[4];

    msg[0] = eCMD_STATE_8BIT;
    msg[1] = APP_eReg_WindowState;      // number of bitfiels bytes
    msg[2] = (g_window_state==true) ? (1<<0) : 0;
    bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
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
    motors_initialize();
    blinds_initialize();
    dt_initialize();

    // load application parameters
    app_register_load();
    // initialize window statemachine
    g_window_state = false;
    g_last_window_state = !g_window_state;

    timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
}

/**
 * Application specific command interpreter code.
 * 
 * Executed if a not-common command is received.
 */
void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    uint8_t blind_index = 0;

    switch (msg[0]) {
    case APP_eCmd_Stop:
        blind_index = msg[1];
        if (blind_index < BLIND_COUNT) blind_stop(blind_index);
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

    if (input_up()) {
        blind_move_to_position(0, 100);
    }
    if (input_down()) {
        blind_move_to_position(0, 0);
    }

    // check window position
    g_window_state = input_window_closed();
    if (g_last_window_state != g_window_state) {
        g_last_window_state = g_window_state;
        send_window_state(bus);
    }

    motors_background();
    blinds_background(bus);

    if (timer_is_elapsed(&g_seconds_timer)) {
        timer_start(&g_seconds_timer, TIMER_MS_2_TICKS(1000));
        dt_tick_second();
    }
}

/** @} */
