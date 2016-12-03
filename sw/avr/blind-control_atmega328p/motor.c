/**
 * @addtogroup MOTOR
 * @brief This module contains functions to control the motor relays.
 *
 * @{
 * @file    motor.c
 * @brief   This module contains functions to control the motor relays.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "motor.h"

// --- Definitions -------------------------------------------------------------

#define MOTOR_UPDOWN_PIN	PC0
#define MOTOR_OFFON_PIN		PC1

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize motor module. Configure output pins.
 */
void motor_initialize       (void)
{
    DDRC |= ((1<<MOTOR_UPDOWN_PIN) | (1<<MOTOR_OFFON_PIN));
    PORTC |= ((1<<MOTOR_UPDOWN_PIN) | (1<<MOTOR_OFFON_PIN));
}

void motor_up               (void)
{
    PORTC |= (1<<MOTOR_UPDOWN_PIN);
    PORTC &= ~(1<<MOTOR_OFFON_PIN);
}

void motor_down             (void)
{
    PORTC &= ~(1<<MOTOR_UPDOWN_PIN);
    PORTC &= ~(1<<MOTOR_OFFON_PIN);
}

void motor_stop             (void)
{
    PORTC |= (1<<MOTOR_OFFON_PIN);
    PORTC |= (1<<MOTOR_UPDOWN_PIN);
}

/** @} */
