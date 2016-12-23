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

#include "inputs.h"
#include "motor.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

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
    //TODO insert application specific initializations here!
    //register_set_u16(MOD_eReg_ModuleID, 10);
	motor_initialize();
	input_initialize();
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
void app_background (void)
{
    //TODO insert application specific background routines here!
    input_background();

    if (input_up()) {
		motor_up();
	}
	if (input_down()) {
		motor_down();
	}
	motor_background();
}

/** @} */
