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

// --- Include section ---------------------------------------------------------

#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "bus.h"
#include "clock.h"
#include "cmddef_common.h"
#include "moddef_common.h"
#include "queue.h"
#include "register.h"
#include "sleepmode.h"

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

extern void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg);

extern void app_background (sBus_t* bus);

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
        sleep_pinchange_enable();
        bus_sleep(&g_bus);
        sleep_pinchange_disable();
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
        app_on_command(sender, msglen, msg);
        break;
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t sender = 0, module_id = 0x7F;

    clk_initialize();

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
    }
    return 0;
}

/** @} */
