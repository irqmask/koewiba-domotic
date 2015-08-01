/**
 * @addtogroup PLAYGROUND
 *
 * @{
 * @file    main.c
 * @brief   Unsorted and unreviewed code for testing and getting things running.
 * Test things.
 * @author  You can never be sure
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include "comm.h"

// --- Definitions -------------------------------------------------------------

// Port D pin assignments
#define LED_TEST        0b00010000
#define LED_EXP         0b01000000
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000
#define RS485_DRIVER    0b00000100
#define RS485_RECVSTOP  0b00001000

// --- Type definitions --------------------------------------------------------

enum E_ISRFLAG
{
    e_uartrxflag    = 0b00000001,
    e_uarttxflag    = 0b00000010,
    e_uartrxerrflag = 0b00000100,
    e_uarttxerrflag = 0b00001000,
    e_timeout       = 0b00010000
};

typedef struct sTimer
{
    uint8_t val[8];
    uint8_t elapsed;
} sTimer_t;

typedef struct sBusMsg
{
    uint8_t* msg;
    uint8_t msglen;
    uint8_t msgcnt;
} sBusMsg_t;

typedef struct sGlobals
{
    unsigned char   isrflags;
    sTimer_t         timer;
    sBusMsg_t        uart;
} sGlobals_t;

// --- Local variables ---------------------------------------------------------

sGlobals_t sGlobal;

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

BOOL COMM_SendMessage (uint8_t* pcMessage, uint8_t uMsgLength)
{
    char byte, recv;

    sGlobal.uart.msg = pcMessage;
    UCSR0B |= ((1<<TXCIE0) | (1<<TXEN0) | (1<<RXEN0));
    while ( !( UCSR0A & (1<<UDRE0)) ) {};
    UDR0 = sGlobal.uart.msg[0];
    sGlobal.uart.msgcnt = 0;
    sGlobal.uart.msglen = uMsgLength;

    while(sGlobal.uart.msglen > sGlobal.uart.msgcnt)
    {
        while ( !(UCSR0A & (1<<RXC0)) ) {};
        recv = UDR0;
        byte = sGlobal.uart.msg[sGlobal.uart.msgcnt];
        PORTD |= (LED_EXP);
        if(recv == byte) {
            sGlobal.isrflags |= e_uarttxflag;
            while ( !( UCSR0A & (1<<UDRE0)) ) {};
            UDR0 = sGlobal.uart.msg[++sGlobal.uart.msgcnt];
            //else UCSR0B &= ~((1<<TXCIE0) | (1<<TXEN0));
        }
        else UDR0 = recv;
        //else
    }
    //PORT &= ~(ExpLED);
    return TRUE;
}

// --- Global functions --------------------------------------------------------

void main(void)
{
    uint8_t buttonstatus    = 0,
            sendchar        = 0;
    uint8_t msglen          = 0;
    uint8_t msg[COMM_MAXMSGSIZE];

    // Initialize port pins
    UART_DDR |= (UART_DRIVER | UART_RECVSTOP);

    DDRD |= (LED_TEST | LED_EXP);
    DDRD &= ~(BTN_TEST);

    PORTD |= LED_TEST;              // switch on test led
    PORTD |= BTN_TEST;              // set pull-up for button

    COMM_Initialize();

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
            COMM_SendMessage("Hallo Welt!", 11);
        }
//        COMM_Communicate();
//        if (COMM_RecvMessage(msg, &msglen)) {
//            COMM_AcknowledgeMessage();
//            //send copy of message back message
//            COMM_SendMessage(msg, msglen)
//        }
    }
}

/** @} */
