/**
 * @addtogroup TEST_UART
 *
 * @{
 * @file    main_testuart.c
 * @brief   Test UART functionality.
 * @author  Robert M�ller
 */ //---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "bus_intern.h"

// --- Definitions -------------------------------------------------------------

// Port D pin assignments
#define LED_TEST        0b00010000
#define LED_EXP         0b01000000
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

sBusPhy_t   g_sBusPhy0;
uint8_t     g_auTestMsg[] = "Hallo Bus!\n\r";

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Toggle the test LED.
 */
void vToggleLED(void)
{
    PORTD ^= LED_TEST;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t buttonstatus    = 0,
            sendchar        = 0;
    BOOL rc = FALSE;
    
    DDRD |= (LED_TEST | LED_EXP);
    DDRD &= ~(BTN_TEST);

    PORTD |= LED_TEST;              // switch on test led
    PORTD |= BTN_TEST;              // set pull-up for button

    // initialize physical layer of bus
    BUS__vPhyInitialize(&g_sBusPhy0, 0);
    BUS__vPhyActivateSender(&g_sBusPhy0, TRUE);
            _delay_ms(500);
    BUS__vPhyActivateSender(&g_sBusPhy0, FALSE);
            _delay_ms(500);
    BUS__vPhyActivateSender(&g_sBusPhy0, TRUE);
            _delay_ms(500);
    BUS__vPhyActivateSender(&g_sBusPhy0, FALSE);
            _delay_ms(500);
    BUS__vPhyActivateSender(&g_sBusPhy0, TRUE);

    sei();

    while (1) {
        // If test-button is pressed
        if ((PIND & BTN_TEST) == 0) {
            // Is this the first time, we are seeing a pressed button?
            if (buttonstatus == FALSE) {
                vToggleLED(); // toggle test-led
                sendchar = TRUE;
            }
            buttonstatus = TRUE;
        }
        else {
            buttonstatus = FALSE;
        }

        // if the sendchar flag is set, send a char on the bus
        if (TRUE == sendchar) {
            sendchar = FALSE;
            PORTD |= LED_EXP;       // switch EXP LED on
            
            // Initiate sending of test message
            rc = BUS__bPhySend(&g_sBusPhy0, g_auTestMsg, sizeof(g_auTestMsg));
                       
            _delay_ms(500);
            PORTD &= ~LED_EXP;      // switch EXP LED off
        }
    }
}

/** @} */
