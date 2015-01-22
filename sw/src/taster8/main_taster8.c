/**
 * @addtogroup TASTER8
 *
 * @{
 * @file    main_taster8.c
 * @brief   main entry point of taster8 application.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>

#include "appconfig.h"
#include "pcbconfig.h"

#include "cmddef_common.h"
#include "moddef_common.h"

#include "application.h"
#include "bus.h"
#include "clock.h"
#include "led_debug.h"
#include "prjtypes.h"
#include "register.h"
#include "sleepmode.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

void IO_vInitialize(void)
{
    DDRB  |= ((0<<DDB7)   | (0<<DDB6)   | (1<<DDB5)   | (1<<DDB4)   | (0<<DDB3)   | (1<<DDB2)   | (1<<DDB1)   | (1<<DDB0)  );
    DDRC  |= (              (0<<DDC6)   | (1<<DDC5)   | (1<<DDC4)   | (1<<DDC3)   | (1<<DDC2)   | (1<<DDC1)   | (1<<DDC0)  );
    DDRD  |= ((0<<DDD7)   | (1<<DDD6)   | (0<<DDD5)   | (1<<DDD4)   | (1<<DDD3)   | (1<<DDD2)   | (1<<DDD1)   | (0<<DDD0)  );

    PORTB |= ((1<<PORTB7) | (1<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0));
    PORTC |= (              (1<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0));
    PORTD |= ((1<<PORTD7) | (0<<PORTD6) | (1<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0));
}

/**
 * Initialize LEDs and keys.
 */
void init_led_and_keys(void)
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

// Interpret message received by taster8 application
static void interpret_message(sBus_t* bus, uint8_t* msg, uint8_t msg_len, uint16_t sender)
{
    uint32_t value;
    
    if (msg[0] <= eCMD_STATE_DATE_TIME) {
        // state messages
        switch (msg[0]) {
        case eCMD_STATE_BITFIELDS:
            if (msg[2] & 0b00000001) LED_STATUS_ON;
            else                       LED_STATUS_OFF;
            break;
        case eCMD_STATE_8BIT:
            value = msg[2];
            register_do_mapping(sender, msg[1], value);
            break;
        case eCMD_STATE_16BIT:
            value = msg[2];
            value |= ((uint16_t)msg[3] << 8);
            register_do_mapping(sender, msg[1], value);
            break;
        case eCMD_STATE_32BIT:
            value = msg[2];
            value |= ((uint32_t)msg[3]<<8);
            value |= ((uint32_t)msg[4]<<16);
            value |= ((uint32_t)msg[5]<<24);
            register_do_mapping(sender, msg[1], value);
            break;
        }

    } else if (msg[0] <= eCMD_SET_REG_32BIT) {
        // register messages
        register_do_command(bus, msg, msg_len, sender);

    } else {
        // system messages
        switch (msg[0]) {
        case eCMD_SLEEP:
            sleep_pinchange2_enable();
            bus_sleep(bus);
            sleep_pinchange2_disable();
            break;
        case eCMD_ACK:
            bus->eModuleState = eMod_Running;
            break;
        default:
            //bus_send_ack_message(psBus, uSender);
            break;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t     msglen = 0;
    uint8_t     msg[BUS_MAXRECVMSGLEN];
    uint16_t    module_id = 0, sender = 0;

    IO_vInitialize();
    clk_initialize();

    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&app_bus, module_id);
    bus_initialize(&app_bus, 0);// initialize bus on UART 0

    init_led_and_keys();
    sei();

    while (1) {
        // check for message and read it
        if (bus_get_message(&app_bus)) {
            if (bus_read_message(&app_bus, &sender, &msglen, msg)) {
                interpret_message(&app_bus, msg, msglen, sender);
            }
        }
        app_check_keys();
    }
    return 0;
}

/** @} */
