/**
 * @addtogroup TEST_SCHEDULER
 *
 * @{
 * @file    main_testscheduler.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include "bus.h"
#include "bus_scheduler.h"
#include "clock.h"
// TODO remove after debug
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// Pin assignments of board keys and LEDs
// Port D pin assignments
#define LED_STATUS      0b01000000  //!< Yellow status LED
#define LED_ERROR       0b00010000  //!< Red error LED
#define BTN_TEST        0b00100000
#define BTN_EXP         0b10000000

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t 		g_sBus;
static sSched_t		g_sSched;
static sClkTimer_t 	g_sLedTimer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void io_initialize (void)
{
#if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__) || defined (__AVR_ATmega8__)
    DDRB  |= ((0<<DDB7)   | (0<<DDB6)   | (1<<DDB5)   | (1<<DDB4)   | (0<<DDB3)   | (1<<DDB2)   | (1<<DDB1)   | (1<<DDB0)  );
    DDRC  |= (              (0<<DDC6)   | (1<<DDC5)   | (1<<DDC4)   | (1<<DDC3)   | (1<<DDC2)   | (1<<DDC1)   | (1<<DDC0)  );
    DDRD  |= ((0<<DDD7)   | (1<<DDD6)   | (0<<DDD5)   | (1<<DDD4)   | (1<<DDD3)   | (1<<DDD2)   | (1<<DDD1)   | (0<<DDD0)  );

    PORTB |= ((1<<PORTB7) | (1<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0));
    PORTC |= (              (1<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0));
    PORTD |= ((1<<PORTD7) | (0<<PORTD6) | (1<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (1<<PORTD0));
#elif defined (__AVR_ATtiny1634__)
    DDRA  |= ((0<<DDRA7)  | (1<<DDRA6)  | (0<<DDRA5)  | (1<<DDRA4)  | (1<<DDRA3)  | (1<<DDRA2)  | (1<<DDRA1)  | (0<<DDRA0) );
    DDRB  |= (                                                        (1<<DDRB3)  | (0<<DDRB2)  | (1<<DDRB1)  | (0<<DDRB0) );
    DDRC  |= (                            (0<<DDRC5)  | (0<<DDRC4)  | (0<<DDRC3)  | (1<<DDRC2)  | (1<<DDRC1)  | (1<<DDRC0) );

    PORTA |= ((1<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (1<<PORTA4) | (1<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0));
    PORTB |= (                                                        (1<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0));
    PORTC |= (                            (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0));
#endif
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint16_t sender = 0;
    uint8_t msg[BUS_MAXMSGLEN];

    io_initialize();
    clk_initialize();
    bus_configure(&g_sBus, 1); // configure a bus node with address 1
    bus_scheduler_initialize(&g_sBus, &g_sSched, 0);// initialize bus on UART 0

    sei();

    clk_timer_start(&g_sLedTimer, CLOCK_MS_2_TICKS(1000));

    while (1) {
    	if (bus_schedule_and_get_message(&g_sBus, &g_sSched)) {
    		if (bus_read_message(&g_sBus, &sender, &msglen, msg)) {
                // interpret incomming message for scheduler
    		}
    	}
    	else bus_schedule_check_and_set_sleep(&g_sBus);

        if (clk_timer_is_elapsed(&g_sLedTimer)) {
            PORTD ^= LED_STATUS;
            clk_timer_start(&g_sLedTimer, CLOCK_MS_2_TICKS(1000));
        }
    }
    return 0;
}

/** @} */
