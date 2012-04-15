/**
 * @addtogroup TEST_UART
 *
 * @{
 * @file    main.c
 * @brief   Test UART functionality.
 * @author  Robert Müller
 */ //---------------------------------------------------------------------------
// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define UART_BAUDRATE   38400

#define UBRRVAL ((uint16_t)(((F_CPU / (16.0 * UART_BAUDRATE)) - 1.0) + 0.5))
#define UBRRVALH ((uint8_t)(UBRRVAL>>8))
#define UBRRVALL ((uint8_t)UBRRVAL)

// Port D pin assignments
#define LED_TEST        0b00010000
#define LED_EXP         0b01000000
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000
#define RS485_DRIVER    0b00000100
#define RS485_RECVSTOP  0b00001000

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

/**
 * Toggle the test LED.
 */
void ToggleLED(void)
{
    PORTD ^= LED_TEST;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t buttonstatus    = 0,
            sendchar        = 0;

    DDRD |= (LED_TEST | LED_EXP | RS485_DRIVER | RS485_RECVSTOP);
    DDRD &= ~(BTN_TEST);

    PORTD |= LED_TEST;              // switch on test led
    PORTD |= BTN_TEST;              // set pull-up for button

    // initialize UART
#if defined (__AVR_ATmega8__)
    UBRRH = UBRRVALH;
    UBRRL = UBRRVALL;
    UCSRB |= 0b10011000;
#elif defined (__AVR_ATmega88__)
    UBRR0H = UBRRVALH;
    UBRR0L = UBRRVALL;
    UCSR0B |= 0b10011000;
#endif

    // activate RS485 driver for sending and receiving
    PORTD |= RS485_DRIVER;
    PORTD &= ~(RS485_RECVSTOP);

    while (1) {
        // If test-button is pressed
        if ((PIND & BTN_TEST) == 0) {
            // Is this the first time, we are seeing a pressed button?
            if (buttonstatus == FALSE) {
                ToggleLED(); // toggle test-led
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
#if   defined (__AVR_ATmega8__)
            // wait until transmit-buffer is ready for next char
            while (!(UCSRA & 0b00100000)) {}
            UDR = 'Z';              // put next char in buffer

            while (!(UCSRA & 0b00100000)) {}
#elif defined (__AVR_ATmega88__)
            // wait until transmit-buffer is ready for next char
            while (!(UCSR0A & 0b00100000)) {
            }
            UDR0 = 'Z';             // put next char in buffer
            while (!(UCSR0A & 0b00100000)) {
            }
#endif
            _delay_ms(500);
            PORTD &= ~LED_EXP;      // switch EXP LED off
        }
    }
}

/** @} */
