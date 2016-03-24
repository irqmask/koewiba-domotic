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

extern void app_background (void);

ISR(INTERRUPT_PINCHANGE2)
{

}

static inline void interpret_message (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    switch (msg[0]) {
    case eCMD_REQUEST_REG:
        //if (TRUE != app_register_get(msg[1], &rtype, &val)) bus_send_nak_message(&g_bus, sender); break;
        //newmsg[0] = eCMD_STATE_8BIT + rtype-1;
        //newmsg[1] = regno;
        //for (i=0; i<rtype; ++i) newmsg[2+i] = val[i];
        //bus_send_message(&g_bus, sender, rtype, newmsg);
        break;
    case eCMD_SET_REG_8BIT:
        break;
    case eCMD_SET_REG_16BIT:
        break;
    case eCMD_SET_REG_32BIT:
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
        sleep_pinchange2_enable();
        bus_sleep(&g_bus);
        sleep_pinchange2_disable();
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
        app_background();
    }
    return 0;
}

/** @} */
