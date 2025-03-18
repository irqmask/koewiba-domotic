/**
 * @addtogroup BUS_GATEWAY
 *
 * @{
 * @file    main_gateway.c
 * @brief   Main entry point of bus gateway. 
 *          It translates bus messages to serial messages and vice-versa.
 *          Additionally 4 inputs which a connected to pi2-base PCB are 
 *          monitored.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
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

#include "appconfig.h"
#include "bus.h"
#include "bus_gateway.h"
#include "cmddef_common.h"
#include "moddef_common.h"
#include "pcbconfig.h"
#include "input.h"
#include "register.h"
#include "serialcomm.h"
#include "sleepmode.h"
#include "timer.h"

#ifndef NO_BLOCK_MESSAGE
 #include "block_message.h"
#endif
#ifndef NO_EEPROM_SPI
 #include "eeprom_spi.h"
#endif

// TODO remove after debug
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static timer_data_t g_LED_timer;
static timer_data_t g_input_timer;

// --- Global variables --------------------------------------------------------

extern const unsigned char app_versioninfo[];
static sBus_t   g_bus;
scomm_phy_t     g_serial_phy;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

ISR(INTERRUPT_PINCHANGE0)
{
    // nothing to do here, but ISR is needed for sleep-mode
}

ISR(INTERRUPT_PINCHANGE1)
{
    // nothing to do here, but ISR is needed for sleep-mode
}

ISR(INTERRUPT_PINCHANGE2)
{
    // nothing to do here, but ISR is needed for sleep-mode
}

static void io_initialize(void)
{
    DDRA  |= ((0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0) );
    PORTA |= ((0<<PA7)  |  (0<<PA6)  |  (0<<PA5)  |  (0<<PA4)  |  (1<<PA3)  |  (1<<PA2)  |  (1<<PA1)  |  (1<<PA0)  );

    DDRB  |= ((1<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (1<<DDB1) | (1<<DDB0) );
    PORTB |= ((0<<PB7)   |  (0<<PB6)  |  (0<<PB5)  |  (0<<PB4)  |  (0<<PB3)  |  (0<<PB2)  |  (0<<PB1)  |  (0<<PB0)  );

    DDRC  |= ((0<<DDC7) | (0<<DDC6) | (1<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0) );
    PORTC |= ((0<<PC7)   |  (0<<PC6)  |  (0<<PC5)  |  (0<<PC4)  |  (0<<PC3)  |  (0<<PC2)  |  (0<<PC1)  |  (0<<PC0)  );

    DDRD  |= ((0<<DDD7) | (0<<DDD6) | (1<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0) );
    PORTD |= ((0<<PD7)   |  (0<<PD6)  |  (0<<PD5)  |  (0<<PD4)  |  (0<<PD3)  |  (0<<PD2)  |  (0<<PD1)  |  (0<<PD0)  );
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
            answer_version_info_request(sender);
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

    case eCMD_SLEEP:
        bus_sleep(&g_bus);
        break;

    case eCMD_RESET:
        cli();
#ifdef __AVR_ATtiny1634__
        //TODO implement reset for attiny1634
#else
        wdt_enable(WDTO_15MS);
        while (1); // wait until watchdog resets the controller.
#endif
        break;

    default:
        break;
    }
}

static void send_input_state(uint8_t input, uint8_t value)
{
    uint8_t cmd[3];
    cmd[0] = eCMD_STATE_8BIT;
    cmd[1] = APP_eReg_Input1 + input;
    cmd[2] = value;

    bus_send_message(&g_bus, g_bus.sCfg.uOwnAddress & BUS_SEGBRDCSTMASK, sizeof(cmd), cmd);
    bgw_send_serial_msg(&g_serial_phy, g_bus.sCfg.uOwnAddress, BUS_BRDCSTADR, sizeof(cmd), cmd);
}

static void check_inputs(void)
{
    uint8_t opened = input_went_high();
    uint8_t closed = input_went_low();

    for (uint8_t i=0; i<INPUT_NUM_PINS; i++) {
        if ((closed & (1<<i)) != 0) {
            send_input_state(i, 0);
        }
        else if ((opened & (1<<i)) != 0) {
            send_input_state(i, 1);
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t sender = 0;
    uint16_t module_id = 0;

    io_initialize();
    input_initialize();
    timer_initialize();
    scomm_initialize_uart1(&g_serial_phy);

    //register_set_u16(MOD_eReg_ModuleID, 0x0003);
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

    sei();
    LED_STATUS_OFF;
    LED_ERROR_OFF;

    timer_start(&g_LED_timer, TIMER_MS_2_TICKS(1000));
    timer_start(&g_input_timer, TIMER_MS_2_TICKS(20));
    // activate pin-change-interrupts for the inputs
    PCMSK0 |= ((1<<PCINT3) | (1<<PCINT2) | (1<<PCINT1) | (1<<PCINT0));

    while (1) {
        // check for message and read it
        if (bus_get_message(&g_bus)) {
            if (bgw_forward_bus_msg(&g_bus, &g_serial_phy, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
        }
        bgw_forward_serial_msg(&g_bus, &g_serial_phy);

        if (timer_is_elapsed(&g_input_timer)) {
            timer_start(&g_input_timer, TIMER_MS_2_TICKS(20));
            input_background();
            check_inputs();
        }

        if (timer_is_elapsed(&g_LED_timer)) {
            // cyclic reset of error LED
            LED_ERROR_OFF;
            LED_STATUS_TOGGLE;
            timer_start(&g_LED_timer, TIMER_MS_2_TICKS(1000));
        }

        if (sleep_check_and_goodnight() == true) {
            // bus gone to sleep and now woken up
            // wait for first pending byte, then set module to running state
            g_bus.eState = eBus_InitWait;
        }
    }
    return 0;
}

/** @} */
