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

#include "pcbconfig.h"

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
            SLEEP_vPinChange2_Enable();
            BUS_vSleep(psBus);
            SLEEP_vPinChange2_Disable();
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

    sClkTimer_t pwm_demotimer;
    uint8_t pwm_r = 0, pwm_g = 127, pwm_b = 33;
    int8_t  pwm_r_incr = 2, pwm_g_incr = -1, pwm_b_incr = 3;

    DDRD |= LED_ERROR | LED_STATUS;

    CLK_vInitialize();
    BUS_vConfigure(&g_sBus, REG_uGetU16Register(MOD_eReg_ModuleID)); // configure a bus node with address X
    BUS_vInitialize(&g_sBus, 0);// initialize bus on UART 0

    SPI_vMasterInitBlk();
    //EEP_vInit();
    PWM_vInit();

    sei();
    PWM_vSet(0,0);
    PWM_vSet(1,0);
    PWM_vSet(2,0);
    PWM_vUpdate();
    CLK_bTimerStart(&pwm_demotimer, 5);

    while (1) {
        if (BUS_bGetMessage(&g_sBus)) {
            if (BUS_bReadMessage(&g_sBus, &sender, &msglen, msg)) {
                vInterpretMessage(&g_sBus, msg, msglen, sender);
            }
        }
        if (CLK_bTimerIsElapsed(&pwm_demotimer)) {
            if (pwm_r > 255-2) pwm_r_incr = -2;
            if (pwm_r < 2) pwm_r_incr = 2;
            if (pwm_g > 255-1) pwm_g_incr = -1;
            if (pwm_g < 1) pwm_g_incr = 1;
            if (pwm_b > 255-3) pwm_b_incr = -3;
            if (pwm_b < 3) pwm_b_incr = 3;

            PWM_vSet(0, pwm_r);
            PWM_vSet(1, pwm_g);
            PWM_vSet(2, pwm_b);
            pwm_r += pwm_r_incr;
            pwm_g += pwm_g_incr;
            pwm_b += pwm_b_incr;
            PWM_vUpdate();
            CLK_bTimerStart(&pwm_demotimer, 5);
        }
    }
    return 0;
}

/** @} */
