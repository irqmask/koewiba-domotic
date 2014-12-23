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

#include "pcbconfig.h"

#include "bus.h"
#include "clock.h"
#include "cmddef_common.h"
#include "led_debug.h"
#include "moddef_common.h"
#include "register.h"
#include "sleepmode.h"

//TODO remove after debug
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t       g_bus;
static sClkTimer_t  g_LED_timer;
static uint8_t      g_buttons = 0,
                    g_oldbuttons = 0;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

ISR(INTERRUPT_PINCHANGE2)
{

}

/**
 * Initialize LEDs and keys.
 */
static void init_LED_and_keys(void)
{
    DDRD |= (LED_ERROR | LED_STATUS);
    DDRD &= ~(BTN_TEST | BTN_EXP);

    PORTD &= ~(LED_ERROR | LED_STATUS); // switch LEDs off.
    PORTD |= (BTN_TEST | BTN_EXP);      // set pull-up for buttons

#if defined (__AVR_ATmega8__)    || \
    defined (__AVR_ATmega88__)   || \
    defined (__AVR_ATmega88A__)  || \
    defined (__AVR_ATmega88P__)
    // Pin-Change-Interrupt
    PCICR  = ((0<<PCIE2)   | (0<<PCIE1)   | (0<<PCIE0)); //disable IR_PinChange2
    PCMSK2 = ((1<<PCINT23) | (0<<PCINT22) | (1<<PCINT21) | (0<<PCINT20) | (0<<PCINT19) | (0<<PCINT18) | (0<<PCINT17) | (0<<PCINT16));
    PCMSK1 = (               (0<<PCINT14) | (0<<PCINT13) | (0<<PCINT12) | (0<<PCINT11) | (0<<PCINT10) | (0<<PCINT9 ) | (0<<PCINT8 ));
    PCMSK0 = ((0<<PCINT7 ) | (0<<PCINT6 ) | (0<<PCINT5 ) | (0<<PCINT4 ) | (0<<PCINT3 ) | (0<<PCINT2 ) | (0<<PCINT1 ) | (0<<PCINT0 ));
#endif
}

// select a receiver address. selector is used to select between different
// receivers per module.
static uint16_t get_receiver (uint8_t selector)
{
    if (selector == 1) {
        if (g_bus.sCfg.uOwnNodeAddress == 10) return 11;
        if (g_bus.sCfg.uOwnNodeAddress == 11) return 12;
        if (g_bus.sCfg.uOwnNodeAddress == 12) return 10;

        if (g_bus.sCfg.uOwnNodeAddress == 3) return 5;
        if (g_bus.sCfg.uOwnNodeAddress == 5) return 20;
        if (g_bus.sCfg.uOwnNodeAddress == 20) return 3;

        if (g_bus.sCfg.uOwnNodeAddress == 21) return 22;
        if (g_bus.sCfg.uOwnNodeAddress == 22) return 21;
    } else if (selector == 2) {
        if (g_bus.sCfg.uOwnNodeAddress == 10) return 0x0E; //14
        if (g_bus.sCfg.uOwnNodeAddress == 11) return 5;
        if (g_bus.sCfg.uOwnNodeAddress == 12) return 20;

        if (g_bus.sCfg.uOwnNodeAddress == 3) return 0x0E;
        if (g_bus.sCfg.uOwnNodeAddress == 5) return 11;
        if (g_bus.sCfg.uOwnNodeAddress == 20) return 12;

        if (g_bus.sCfg.uOwnNodeAddress == 21) return 22;
        if (g_bus.sCfg.uOwnNodeAddress == 22) return 21;
    }
    return 0;
}

static void interpret_message (uint16_t sender, uint8_t msglen, uint8_t* msg)
{

    switch (msg[0]) {
    case CMD_eStateBitfield:
        if (msglen < 2) break;
        if (msg[2] != 0) {
            PORTD |= LED_STATUS;
        } else {
            PORTD &= ~LED_STATUS;
        }
        break;
    case CMD_eSleep:
        sleep_pinchange2_enable();
        bus_sleep(&g_bus);
        sleep_pinchange2_disable();
        break;
    default:
        break;
    }
}

static void check_buttons (void)
{
    const uint8_t registers[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t temp, msglen, msg[8];
    static uint8_t regidx = 0, light = 0;

    // check button
    g_buttons = PIND & (BTN_TEST | BTN_EXP);
    temp = g_buttons ^ g_oldbuttons;
    g_oldbuttons = g_buttons;
    if ((g_buttons & BTN_TEST) && (temp & BTN_TEST)) {
        regidx++;
        if (regidx > 7) regidx = 0;

        msg[0] = CMD_eRequestRegister;
        msg[1] = registers[regidx];
        msglen = 2;
        bus_send_message(&g_bus, get_receiver(1), msglen, msg);
    }
    else if ((g_buttons & BTN_EXP) && (temp & BTN_EXP)) {
        if (light)  light = 0;
        else        light = 1;

        msg[0] = CMD_eStateBitfield;
        msg[1] = 1;
        msg[2] = light;
        msg[3] = 0b00000001;
        msglen = 4;
        bus_send_message(&g_bus, get_receiver(1), msglen, msg);
        bus_send_message(&g_bus, get_receiver(2), msglen, msg);
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGLEN];
    uint16_t sender = 0, module_id = 10;

    clk_initialize();

    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

    init_LED_and_keys();

    g_buttons = PIND & (BTN_TEST | BTN_EXP);
    g_oldbuttons = g_buttons;
    sei();

    clk_timer_start(&g_LED_timer, 1000);

    while (1) {
        // check for message and read it
        if (bus_get_message(&g_bus)) {
            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
        }
        check_buttons();
    }
    return 0;
}

/** @} */
