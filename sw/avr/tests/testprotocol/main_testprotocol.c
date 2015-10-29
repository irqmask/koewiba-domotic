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

#include "appconfig.h"
#include "block_message.h"
#include "bus.h"
#include "clock.h"
#include "cmddef_common.h"
#include "eeprom_spi.h"
#include "led_debug.h"
#include "moddef_common.h"
#include "pcbconfig.h"
#include "queue.h"
#include "register.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_bus;
static clock_timer_t g_LED_timer;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------


static void io_initialize(void)
{
    DDRA  |= ((0<<DDA7)  | (1<<DDA6)  | (1<<DDA5)  | (1<<DDA4)  | (1<<DDA3)  | (1<<DDA2)  | (1<<DDA1)  | (1<<DDA0) );
    DDRB  |= (                                                    (1<<DDB3)  | (1<<DDB2)  | (1<<DDB1)  | (1<<DDB0) );
    DDRC  |= (                          (0<<DDC5)  | (0<<DDC4)  | (0<<DDC3)  | (1<<DDC2)  | (1<<DDC1)  | (1<<DDC0) );

    PORTA |= ((0<<PA7) | (0<<PA6) | (1<<PA5) | (1<<PA4) | (1<<PA3) | (0<<PA2) | (0<<PA1) | (0<<PA0));
    PORTB |= (                                                        (1<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0));
    PORTC |= (                            (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0));
}

void activate_wakeup_interrupt(void)
{

}

void deactivate_wakeup_interrupt(void)
{

}


// select a receiver address. selector is used to select between different
// receivers per module.
/* TODO remove. only for test purposes. static uint16_t get_receiver (uint8_t selector)
{
    if (selector == 1) {
        if (g_bus.sCfg.uOwnNodeAddress == 10) return 11;
        if (g_bus.sCfg.uOwnNodeAddress == 11) return 12;
        if (g_bus.sCfg.uOwnNodeAddress == 12) return 10;

        if (g_bus.sCfg.uOwnNodeAddress == 3) return 20;
        if (g_bus.sCfg.uOwnNodeAddress == 5) return 21;
        if (g_bus.sCfg.uOwnNodeAddress == 20) return 22;

        if (g_bus.sCfg.uOwnNodeAddress == 21) return 22;
        if (g_bus.sCfg.uOwnNodeAddress == 22) return 21;
    } else if (selector == 2) {
        if (g_bus.sCfg.uOwnNodeAddress == 10) return 0x0E; //14
        if (g_bus.sCfg.uOwnNodeAddress == 11) return 5;
        if (g_bus.sCfg.uOwnNodeAddress == 12) return 20;

        if (g_bus.sCfg.uOwnNodeAddress == 3) return 5;
        if (g_bus.sCfg.uOwnNodeAddress == 5) return 3;
        if (g_bus.sCfg.uOwnNodeAddress == 20) return 12;

        if (g_bus.sCfg.uOwnNodeAddress == 21) return 22;
        if (g_bus.sCfg.uOwnNodeAddress == 22) return 21;
    }
    return 0;
}*/

static void interpret_message (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    //eRegType_t rtype;
    //uint8_t    val;
    //uint8_t    i, regno = msg[1];
    //uint8_t    newmsg[6];

    switch (msg[0]) {

    case eCMD_STATE_BITFIELDS:
        if (msglen < 2) break;
        if (msg[2]) LED_ERROR_ON;
        else        LED_ERROR_OFF;
        break;

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

    case eCMD_SLEEP:
        activate_wakeup_interrupt();
        bus_sleep(&g_bus);
        deactivate_wakeup_interrupt();
        break;
    default:
        break;
    }
}


// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t sender = 0, module_id = 10;

    clk_initialize();

    //register_set_u8(MOD_eReg_ModuleID, 24);
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

    io_initialize();
    eep_initialize();
    sei();

    clk_timer_start(&g_LED_timer, CLOCK_MS_2_TICKS(1000));
    LED_ERROR_OFF;

    while (1) {
        // check for message and read it
        if (bus_get_message(&g_bus)) {
            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
        }
        if (block_timer_elapsed()) {
            uint8_t resetmsg = eCMD_BLOCK_RESET;
            interpret_message(0, 1, &resetmsg);
        }
        if (clk_timer_is_elapsed(&g_LED_timer)) {
            // TODO remove after debug
            clk_timer_start(&g_LED_timer, CLOCK_MS_2_TICKS(1000));
        }
    }
    return 0;
}

/** @} */
