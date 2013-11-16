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

#include "uart.h"

// --- Definitions -------------------------------------------------------------

// Port D pin assignments
#define LED_TEST        0b00010000
#define LED_EXP         0b01000000
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint16_t val;
    char cc;

    DDRB |= (1<<PB0 | 1<<PB1);
    UART_vInit(9600);
    sei();

    UART_vPutString("TESTUART\r");
    UART_vPutString("--------\r");
   
    while (1) {
        PORTB ^= (1<<PB0);
        val = UART_uReceive();
        if (val & (1<<eUartFlag_NoData)) {
            continue;
        }
        if (val & (1<<eUartFlag_BufOverrun)) {
            UART_vPutString("OVR");
        }
        if (val & (1<<eUartFlag_FramingError)) {
            UART_vPutString("FRM");
        }
        cc = (char)(val & 0x00FF);
        UART_vPutChar(val);
    }
}

/** @} */
