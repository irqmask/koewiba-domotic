/**
 * @addtogroup BLINDCONTROL_5+4_ATMEGA328
 * @addtogroup APPLICATION
 * @brief Application specific code of "blind-control_5+4_atmega328p" project.
 *
 * Contains application specific initialization, command-interpreter, 
 * register code and background loop.
 *
 * @{
 * @file    application.c
 * @brief   Application specific code of "blind-control_5+4_atmega328p" project.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "prjtypes.h"

#include "blindctrl.h"
#include "bus.h"
#include "cmddef_common.h"
#include "motor.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

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
    motors_initialize();
    blinds_initialize();
    // load application parameters
    app_register_load();

    DDRB |= (1<<PB0);
    PORTB &= ~(1<<PB0);// switch off board's 24VDC supply
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
    motors_background();
    blinds_background(bus);

    // check if the 24V supply has to be switched on
    if (blinds_are_moving()) {
        PORTB |= (1<<PB0);
    } else {
        PORTB &= ~(1<<PB0);
    }
}

/** @} */
