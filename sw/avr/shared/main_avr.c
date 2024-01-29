/**
 * @addtogroup MAIN
 * @brief Main entry point of every AVR based bus-application.
 *
 * @note Needs an application specific module which implements app_init(),
 * app_on_command() and app_background(). Usually these functions reside in
 * application.c in the project directory.
 * 
 * @{
 * @file    main_avr.c
 * @brief   Main entry point of every AVR based bus-application.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#ifdef __AVR_ATtiny1634__
 #include "bootloader.h"
#endif
#include "bus.h"
#include "cmddef_common.h"
#include "moddef_common.h"
#include "queue.h"
#include "register.h"
#include "sleepmode.h"
#include "timer.h"

#ifndef NO_BLOCK_MESSAGE
 #include "block_message.h"
#endif
#ifndef NO_EEPROM_SPI
 #include "eeprom_spi.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

sBus_t  g_bus;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

extern void app_init (void);

extern void app_on_pinchangeinterrupt(uint8_t pinchange_interruptflags);

extern void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg);

extern void app_background (sBus_t* bus);

ISR(INTERRUPT_PINCHANGE0)
{
    // ISR is needed for wake-up from sleep-mode. Inform application about the interrupt.
#ifdef __AVR_ATtiny1634__
    app_on_pinchangeinterrupt(GIFR);
#else
    app_on_pinchangeinterrupt(PCIFR);
#endif
}

ISR(INTERRUPT_PINCHANGE1)
{
    // ISR is needed for wake-up from sleep-mode. Inform application about the interrupt.
#ifdef __AVR_ATtiny1634__
    app_on_pinchangeinterrupt(GIFR);
#else
    app_on_pinchangeinterrupt(PCIFR);
#endif
}

ISR(INTERRUPT_PINCHANGE2)
{
    // ISR is needed for wake-up from sleep-mode. Inform application about the interrupt.
#ifdef __AVR_ATtiny1634__
    app_on_pinchangeinterrupt(GIFR);
#else   
    app_on_pinchangeinterrupt(PCIFR);
#endif
}

/**
 * Disable watchdog before main() starts.
 */
#ifdef __AVR_ATtiny1634__
//TODO implement watchdog initialization for attiny1634
#else
void init_wdt (void) __attribute__((naked)) __attribute__((section(".init3")));
void init_wdt(void)
{
    MCUSR = 0;
    wdt_disable();
}
#endif

extern const unsigned char app_versioninfo[MOD_LEN_CONTROLLERID + MOD_LEN_BOARDID + MOD_LEN_BOARDREV + MOD_LEN_APPID + MOD_LEN_APPVER];

/// send the version information
static inline void answer_version_info_request (uint16_t sender)
{
    uint8_t msg[sizeof(app_versioninfo) + 1], i;

    msg[0] = eCMD_STATE_VERSION;
    for (i=0; i<sizeof(app_versioninfo); i++) {
        msg[i + 1] = pgm_read_byte(&app_versioninfo[i]);
    }
    bus_send_message(&g_bus, sender, sizeof(msg), msg);
}

/// first entry point to interpret incoming messages.
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
        wdt_enable(WDTO_15MS);
        while (1); // wait until watchdog resets the controller.
        break;

    default:
        app_on_command(sender, msglen, msg);
        break;
    }
}

// --- Module global functions -------------------------------------------------

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

    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t sender = 0, module_id = 0x7F;

    timer_initialize();

    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

#ifndef NO_EEPROM_SPI
    spi_master_init_blk();
    eep_initialize();
#endif

    app_init();

    sei();

    while (1) {
        // check for message and read it
        if (bus_get_message(&g_bus)) {
            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
        }
        app_background(&g_bus);

        if (sleep_check_and_goodnight() == true) {
            // bus gone to sleep and now woken up
            // wait for first pending byte, then set module to running state
            g_bus.eState = eBus_InitWait;
        }
    }
    return 0;
}

/** @} */
