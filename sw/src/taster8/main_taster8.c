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

#include "cmddef_common.h"
#include "moddef_common.h"

#include "bus.h"
#include "clock.h"
#include "pcbconfig.h"
#include "register.h"
#include "sleepmode.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_sBus;

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
void vInitLedAndKeys(void)
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

// Interpret message
static void vInterpretMessage(sBus_t* psBus, uint8_t* puMsg, uint8_t uMsgLen, uint16_t uSender)
{
    if (puMsg[0] <= CMD_eStateDateTime) {
        // state messages
        switch (puMsg[0]) {
        case CMD_eStateBitfield:
            if (puMsg[2] & 0b00000001) LED_STATUS_ON;
            else                       LED_STATUS_OFF;
            break;
        }

    } else if (puMsg[0] <= CMD_eSetRegister32bit) {
        // register messages
        REG_vDoCommand(psBus, puMsg, uMsgLen, uSender);

    } else {
        // system messages
        switch (puMsg[0]) {
        case CMD_eSleep:
            SLEEP_vPinChange2_Enable();
            bus_sleep(&g_sBus);
            SLEEP_vPinChange2_Disable();
            break;
        case CMD_eAck:
            g_sBus.eModuleState = eMod_Running;
            break;
        default:
            bus_send_ack_message(psBus, uSender);
            break;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
	uint8_t light1 = 0, light2 = 0, buttons = 0, oldbuttons = 0, temp = 0;
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGLEN];
    uint16_t sender = 0;

    IO_vInitialize();
    clk_initialize();

    //REG_vSetU16Register(MOD_eReg_ModuleID, 0x14);
    bus_configure(&g_sBus, REG_uGetU16Register(MOD_eReg_ModuleID));
    bus_initialize(&g_sBus, 0);// initialize bus on UART 0

    vInitLedAndKeys();
    sei();

    while (1) {
        // check for message and read it
        if (bus_get_message(&g_sBus)) {
            if (bus_read_message(&g_sBus, &sender, &msglen, msg)) {
                vInterpretMessage(&g_sBus, msg, msglen, sender);
            }
        }
        // check button
        buttons = PIND & (BTN_TEST | BTN_EXP);
        temp = buttons ^ oldbuttons;
        oldbuttons = buttons;
        if ((buttons & BTN_TEST) && (temp & BTN_TEST)) {
            /*
            regidx++;
            if (regidx > 7) regidx = 0;

            msg[0] = CMD_eRequestRegister;
            msg[1] = registers[regidx];
            msglen = 2;
            bus_send_message(&g_sBus, 0x0E, msglen, msg);*/
            if (light1)  light1 = 0;
            else         light1 = 1;

            msg[0] = CMD_eStateBitfield;
            msg[1] = 1;
            msg[2] = light1;
            msg[3] = 0b00000001;
            msglen = 4;
            bus_send_message(&g_sBus, 0x05, msglen, msg);
        }
        else if ((buttons & BTN_EXP) && (temp & BTN_EXP)) {
            if (light2)  light2 = 0;
            else         light2 = 1;

            msg[0] = CMD_eStateBitfield;
            msg[1] = 1;
            msg[2] = light2;
            msg[3] = 0b00000001;
            msglen = 4;
            bus_send_message(&g_sBus, 0x03, msglen, msg);
        }
    }
    return 0;
}

/** @} */
