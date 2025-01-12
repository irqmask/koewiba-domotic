/**
 * @addtogroup SCHEDULER
 *
 * @{
 * @file    main_scheduler.c
 * Scheduler to address all modules on a bus using a token message to
 * permit a send slot for payload messages.
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include <util/delay.h>

#include "pcbconfig.h"

#include "block_message.h"
#include "bootloader.h"
#include "bus.h"
#include "bus_scheduler.h"
#include "cmddef_common.h"
#include "moddef_common.h"
#include "led_debug.h"
#include "register.h"
#include "timer.h"

// --- Definitions -------------------------------------------------------------

// Pin assignments of board keys and LEDs
// Port D pin assignments

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t           g_bus;
static sSched_t         g_sched;
static timer_data_t     g_led_timer;

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void io_initialize (void)
{
#if defined (__AVR_ATmega88__) || defined (__AVR_ATmega88A__)
    DDRB  |= ((0<<DDB7)   | (0<<DDB6)   | (1<<DDB5)   | (1<<DDB4)   | (0<<DDB3)   | (1<<DDB2)   | (1<<DDB1)   | (1<<DDB0)  );
    DDRC  |= (              (0<<DDC6)   | (1<<DDC5)   | (1<<DDC4)   | (1<<DDC3)   | (1<<DDC2)   | (1<<DDC1)   | (1<<DDC0)  );
    DDRD  |= ((0<<DDD7)   | (1<<DDD6)   | (0<<DDD5)   | (1<<DDD4)   | (1<<DDD3)   | (1<<DDD2)   | (1<<DDD1)   | (0<<DDD0)  );

    PORTB |= ((1<<PORTB7) | (1<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0));
    PORTC |= (              (1<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0));
    PORTD |= ((1<<PORTD7) | (0<<PORTD6) | (1<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (1<<PORTD0));
#elif defined (__AVR_ATtiny1634__)
    DDRA  |= ((0<<DDRA7)  | (1<<DDRA6)  | (0<<DDRA5)  | (1<<DDRA4)  | (1<<DDRA3)  | (1<<DDRA2)  | (1<<DDRA1)  | (1<<DDRA0) );
    PORTA |= ((0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (1<<PORTA4) | (1<<PORTA3) | (0<<PORTA2) | (1<<PORTA1) | (1<<PORTA0));

    DDRB  |= (                                                        (1<<DDRB3)  | (0<<DDRB2)  | (1<<DDRB1)  | (1<<DDRB0) );
    PORTB |= (                                                        (1<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0));

    DDRC  |= (                            (0<<DDRC5)  | (0<<DDRC4)  | (0<<DDRC3)  | (1<<DDRC2)  | (1<<DDRC1)  | (1<<DDRC0) );
    PORTC |= (                            (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0));

#endif
}

/// send the version information
static inline void answer_version_info_request (uint16_t sender)
{
    uint8_t msg[MOD_VERSIONINFO_LEN + 1], i;

    msg[0] = eCMD_STATE_VERSION;
    for (i=0; i<MOD_VERSIONINFO_LEN; i++) {
        msg[i + 1] = pgm_read_byte(&app_versioninfo[i]);
    }
    bus_send_message(&g_bus, sender, sizeof(msg), msg);
}

static inline void interpret_message (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    switch (msg[0]) {
    case eCMD_REQUEST_REG:
        // fallthrough
    case eCMD_SET_REG_8BIT:
        // fallthrough
    case eCMD_SET_REG_16BIT:
        // fallthrough
    case eCMD_SET_REG_32BIT:
        register_do_command(&g_bus, sender, msglen, msg);
        break;

    case eCMD_REQUEST_INFO_OF_TYPE:
        if (msglen == 2 && msg[1] == eINFO_VERSION) {
            ///@ todo reimplement when crash fixed
            // answer_version_info_request(sender);
        }
        break;

#ifndef NO_BLOCK_MESSAGE
    case eCMD_BLOCK_START:
        block_message_start(&g_bus, sender, msglen, msg);
        break;
    case eCMD_BLOCK_DATA:
        block_message_data (&g_bus, sender, msglen, msg);
        break;
    case eCMD_BLOCK_END:
        block_message_end  (&g_bus, sender, msglen, msg);
        //no break
    case eCMD_BLOCK_RESET:
        block_data_reset();
        break;
#endif

//    case eCMD_SLEEP:
//        sleep_pinchange_enable();
//        bus_sleep(&g_bus);
//        sleep_pinchange_disable();
//        break;

    case eCMD_RESET:
        cli();
        wdt_enable(WDTO_15MS);
        while (1); // wait until watchdog resets the controller.
        break;

    default:
        break;
    }
}

// --- Module global functions -------------------------------------------------

/**
 * Scheduler specific register_get function.
 *
 * @param[in] reg_no
 */
bool        app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            reg_type,
                                     void*                  value)
{
    // currently nothing to do
    return false;
}

/**
 * Scheduler specific register_set function.
 */
void        app_register_set        (uint8_t                reg_no,
                                     uint32_t               value)
{
    // currently nothing to do
}

// --- Global functions --------------------------------------------------------

int main(void)
{
    MCUSR = 0;
    wdt_disable();
#ifdef __AVR_ATtiny1634__
    void (*bld_entrypoint)( void ) = (void*)BLD_ENTRYPOINT;

    // call bootloader of ATtiny1634, since it has no dedicated bootloader section
    bld_entrypoint();
#endif

    uint16_t    module_id = 0;
    uint8_t     msglen = 0;
    uint16_t    sender = 0;
    uint8_t     msg[BUS_MAXRECVMSGLEN];

    io_initialize();
    timer_initialize();
    register_get(MOD_eReg_ModuleID, 0, &module_id);

    bus_configure(&g_bus, module_id); // configure a bus node with address 1
    bus_scheduler_initialize(&g_bus, &g_sched, 0);// initialize bus on UART 0

    sei();

    timer_start(&g_led_timer, TIMER_MS_2_TICKS(1000));

    while (1) {
    	if (bus_schedule_and_get_message(&g_bus, &g_sched)) {
    		if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
    		    interpret_message(sender, msglen, msg);
    		}
    	}
    	else bus_schedule_check_and_set_sleep(&g_bus);

        if (timer_is_elapsed(&g_led_timer)) {
        	// TODO remove after debug
            timer_start(&g_led_timer, TIMER_MS_2_TICKS(1000));
            LED_STATUS_TOGGLE;
        }
    }
    return 0;
}

/** @} */
