/**
 * @addtogroup BUS_GATEWAY
 *
 * @{
 * @file    main_gateway.c
 * @brief   TODO describe briefly.
 * @todo    describe file purpose
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "appconfig.h"
#include "bus.h"
#include "bus_gateway.h"
#include "cmddef_common.h"
#include "moddef_common.h"
#include "pcbconfig.h"
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

// --- Global variables --------------------------------------------------------

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
    DDRA  |= ((0<<DDA7) | (0<<DDA6) | (1<<DDA5) | (1<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0) );
    PORTA |= ((0<<PA7)   |  (0<<PA6)  |  (0<<PA5)  |  (0<<PA4)  |  (0<<PA3)  |  (0<<PA2)  |  (0<<PA1)  |  (0<<PA0)  );

    DDRB  |= ((1<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (1<<DDB1) | (1<<DDB0) );
    PORTB |= ((0<<PB7)   |  (0<<PB6)  |  (0<<PB5)  |  (0<<PB4)  |  (0<<PB3)  |  (0<<PB2)  |  (0<<PB1)  |  (0<<PB0)  );

    DDRC  |= ((0<<DDC7) | (0<<DDC6) | (1<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0) );
    PORTC |= ((0<<PC7)   |  (0<<PC6)  |  (0<<PC5)  |  (0<<PC4)  |  (0<<PC3)  |  (0<<PC2)  |  (0<<PC1)  |  (0<<PC0)  );

    DDRD  |= ((0<<DDD7) | (0<<DDD6) | (1<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0) );
    PORTD |= ((0<<PD7)   |  (0<<PD6)  |  (0<<PD5)  |  (0<<PD4)  |  (0<<PD3)  |  (0<<PD2)  |  (0<<PD1)  |  (0<<PD0)  );
}

void activate_wakeup_interrupt(void)
{

}

void deactivate_wakeup_interrupt(void)
{

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
        activate_wakeup_interrupt();
        LED_STATUS_OFF;
        LED_ERROR_OFF;
        bus_sleep(&g_bus);
        deactivate_wakeup_interrupt();
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

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t sender = 0;
    uint16_t module_id = 0;

    io_initialize();
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

    while (1) {
        // check for message and read it
        if (bus_get_message(&g_bus)) {
            if (bgw_forward_bus_msg(&g_bus, &g_serial_phy, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
        }
        bgw_forward_serial_msg(&g_bus, &g_serial_phy);

        if (timer_is_elapsed(&g_LED_timer)) {
        	// cyclic reset of error LED
            LED_ERROR_OFF;
            LED_STATUS_TOGGLE;
            timer_start(&g_LED_timer, TIMER_MS_2_TICKS(1000));
        }
    }
    return 0;
}

/** @} */
