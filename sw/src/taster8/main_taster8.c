/**
 * @addtogroup TEST_PROTOCOL
 *
 * @{
 * @file    main_testprotocol.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bus.h"
#include "clock.h"
#include "led_debug.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_sBus;
static sClkTimer_t g_sLedTimer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void IO_vInitialize(void)
{
    DDRB  |= ((0<<DDB7)   | (0<<DDB6)   | (1<<DDB5)   | (1<<DDB4)   | (0<<DDB3)   | (1<<DDB2)   | (1<<DDB1)   | (1<<DDB0)  );
    DDRC  |= (              (0<<DDC6)   | (1<<DDC5)   | (1<<DDC4)   | (1<<DDC3)   | (1<<DDC2)   | (1<<DDC1)   | (1<<DDC0)  );
    DDRD  |= ((0<<DDD7)   | (1<<DDD6)   | (0<<DDD5)   | (1<<DDD4)   | (1<<DDD3)   | (1<<DDD2)   | (1<<DDD1)   | (0<<DDD0)  );

    PORTB |= ((1<<PORTB7) | (1<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0));
    PORTC |= (              (1<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0));
    PORTD |= ((1<<PORTD7) | (0<<PORTD6) | (1<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0));
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
	uint8_t light = 0, oldbutton = 0;
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGLEN];
    uint16_t sender = 0;

    IO_vInitialize();
    CLK_vInitialize();

    //own address sends to receiver
    // 1 -> 3
    // 2 -> 4
    // 3 -> 5
    // 4 -> 6
    // 5 -> 1
    // 6 -> 2

    BUS_vConfigure(&g_sBus, 0x0C); // configure a bus node with address X
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0

    vInitLedAndKeys();
    sei();

    CLK_bTimerStart(&g_sLedTimer, 1000);

    while (1) {
        // check for message and read it
        if (BUS_bGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
                // switch light?
                // type bit-field?
                if (msg[0] == SLEEPCOMMAND) {
                	//LED_ERROR_ON;
                	IR_PinChange2_Enable();
                	BUS_vSleep(&g_sBus);
                	IR_PinChange2_Disable();
                	//LED_ERROR_OFF;
                }
                if (msg[0] == 0x01) {
                    if (msg[2] & 0b00000001) LED_STATUS_ON;
                    else                     LED_STATUS_OFF;
                }
            }
        }
        // check button
        if ((PIND & BTN_TEST) ^ oldbutton) {
            oldbutton = PIND & BTN_TEST;
            if (oldbutton != 0) {
                if (light)  light = 0;
                else        light = 1;

                msg[0] = 0x01;
                msg[1] = 1;
                msg[2] = light;
                msg[3] = 0b00000001;
                msglen = 4;
                BUS_bSendMessage(&g_sBus, 11, msglen, msg);
            }
        }
    }
    return 0;
}

/** @} */