/**
 * @addtogroup ELEKTOR_BUS-NODE
 * @addtogroup APPLICATION
 * @brief Simple test application for Elektor bus-nodes.
 *
 * The bus-nodes are *expected to have the addresses 10, 11 and 12 or
 * 3, 5 and 20. If your addresses varies you have change the addresses in
 * get_receiver().
 * 
 * @{
 * @file    application.c
 * @brief   Simple test application for Elektor bus-nodes.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "prjtypes.h"

#include "bus.h"
#include "cmddef_common.h"
#include "led_debug.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t          g_buttons = 0,
                        g_oldbuttons = 0;

// --- Global variables --------------------------------------------------------

extern sBus_t g_bus;

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

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
        if (g_bus.sCfg.uOwnNodeAddress == 11) return 0x02;
        if (g_bus.sCfg.uOwnNodeAddress == 12) return 20;
        
        if (g_bus.sCfg.uOwnNodeAddress == 3) return 0x0E;
        if (g_bus.sCfg.uOwnNodeAddress == 5) return 11;
        if (g_bus.sCfg.uOwnNodeAddress == 20) return 12;
        
        if (g_bus.sCfg.uOwnNodeAddress == 21) return 22;
        if (g_bus.sCfg.uOwnNodeAddress == 22) return 21;
    }
    return 0;
}

// check button state and send messages if a button is pressed.
static void check_buttons (void)
{
    const uint8_t registers[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t temp, msglen, msg[8];
    static uint8_t regidx = 0, light = 0;
    
    // check button
    g_buttons = PIND & (1<<BTN_TEST | 1<<BTN_EXP);
    temp = g_buttons ^ g_oldbuttons;
    g_oldbuttons = g_buttons;
    if ((g_buttons & 1<<BTN_TEST) && (temp & 1<<BTN_TEST)) {
        regidx++;
        if (regidx > 7) regidx = 0;
        
        msg[0] = eCMD_REQUEST_REG;
        msg[1] = registers[regidx];
        msglen = 2;
        bus_send_message(&g_bus, get_receiver(1), msglen, msg);
    }
    else if ((g_buttons & 1<<BTN_EXP) && (temp & 1<<BTN_EXP)) {
        if (light)  light = 0;
        else        light = 1;
        
        msg[0] = eCMD_STATE_BITFIELDS;
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

/**
 * Application specific initializations.
 * 
 * Executed if common code initialization passed.
 * @note Global interrupts are still switched off! Will be switched on in global
 * main after this initialization code.
 */
void app_init (void) 
{
    DDRD |= (1<<LED_ERROR | 1<<LED_STATUS);
    DDRD &= ~(1<<BTN_TEST | 1<<BTN_EXP);
    
    PORTD &= ~(1<<LED_ERROR | 1<<LED_STATUS); // switch LEDs off.
    PORTD |= (1<<BTN_TEST | 1<<BTN_EXP);      // set pull-up for buttons
    
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
    
    g_buttons = PIND & (BTN_TEST | BTN_EXP);
    g_oldbuttons = g_buttons;
}

/**
 * Application specific command interpreter code.
 * 
 * Executed if a not-common command is received.
 */
void app_on_command (uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    switch (msg[0]) {
    case eCMD_STATE_BITFIELDS:
        if (msglen < 2) break;
        if (msg[2] != 0) {
            PORTD |= (1<<LED_STATUS);
        } else {
            PORTD &= ~(1<<LED_STATUS);
        }
        break;
        
    default:
        break;
    }
}

/**
 * Application specific background code.
 * 
 * Executed once per main loop cycle.
 */
void app_background (void)
{
    check_buttons();
}
/** @} */
