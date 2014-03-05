/**
 * @addtogroup PWM-DIMMER-3CHN
 *
 * @{
 * @file    main_roomthermostate.c
 * @brief   MAin entry point of roomthermostate.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "cmddef_common.h"

#include "bus.h"
#include "pwm.h"
#include "register.h"
#include "sleepmode.h"
#include "spi.h"
#include "ucontroller.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sBus_t      g_sBus;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// Interpret message
static void vInterpretMessage(sBus_t* psBus, uint8_t* puMsg, uint8_t uMsgLen, uint16_t uSender)
{
    if (puMsg[0] <= CMD_eStateDateTime) {
        // state messages
        if (puMsg[0] == CMD_eStateBitfield) {

        }
    } else if (puMsg[0] <= CMD_eSetRegister32bit) {
        // register messages
        REG_vDoCommand(psBus, puMsg, uMsgLen, uSender);
    } else {
        // system messages
        switch (puMsg[0]) {
        case CMD_eAck:
            g_sBus.eModuleState = eMod_Running;
            break;
        case CMD_eSleep:
            SLEEP_PinChange2_Enable();
            BUS_vSleep(psBus);
            SLEEP_PinChange2_Disable();
            break;
        default:
            break;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    uint8_t msglen = 0;
    uint8_t msg[BUS_MAXMSGLEN];
    uint16_t sender = 0;

    DDRC |= ((1<<PC3) | (1<<PC4));
    PORTC &= ~((1<<PC3) | (1<<PC4));

    CLK_vInitialize();

    BUS_vConfigure(&g_sBus, REG_uGetU16Register(MOD_eReg_ModuleID)); // configure a bus node with address X
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0

    SPI_vMasterInitBlk();
    //EEP_vInit();

    PWM_vInit();
    sei();

    while (1) {
        if (BUS_bGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
                vInterpretMessage(&g_sBus, msg, msglen, sender);
            }
        }
    }
    return 0;
}

/** @} */
