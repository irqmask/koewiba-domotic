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

static sBus_t      g_bus;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// Interpret incomming messages
static void interpret_message (sBus_t* bus, uint8_t* msg, uint8_t msg_len, uint16_t sender)
{
    if (msg[0] <= eCMD_STATE_DATE_TIME) {
        // state messages
        if (msg[0] == eCMD_STATE_BITFIELDS) {

        }
    } else if (msg[0] <= eCMD_SET_REG_32BIT) {
        // register messages
        register_do_command(bus, msg, msg_len, sender);
    } else {
        // system messages
        switch (msg[0]) {
        case eCMD_ACK:
            g_bus.eModuleState = eMod_Running;
            break;
        case eCMD_SLEEP:
            sleep_pinchange2_enable();
            bus_sleep(bus);
            sleep_pinchange2_disable();
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
    uint8_t msg[BUS_MAXRECVMSGLEN];
    uint16_t module_id = BUS_UNKNOWNADR, sender = 0;

    sClkTimer_t pwm_demotimer;
    uint8_t pwm_r = 0, pwm_g = 127, pwm_b = 33;
    int8_t  pwm_r_incr = 2, pwm_g_incr = -1, pwm_b_incr = 3;

    DDRD |= (LED_ERROR | LED_STATUS);
    register_set_u16(MOD_eReg_ModuleID, 10);
    clk_initialize();
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_bus, module_id);
    bus_initialize(&g_bus, 0);// initialize bus on UART 0

    spi_master_init_blk();
    //eep_init();
    pwm_init();

    sei();
    pwm_set(0,0);
    pwm_set(1,0);
    pwm_set(2,0);
    pwm_update();
    clk_timer_start(&pwm_demotimer, 5);

    while (1) {
        if (bus_get_message(&g_bus)) {
            if (bus_read_message(&g_bus, &sender, &msglen, msg)) {
                interpret_message(&g_bus, msg, msglen, sender);
            }
        }
        if (clk_timer_is_elapsed(&pwm_demotimer)) {
            if (pwm_r > 255-2) pwm_r_incr = -2;
            if (pwm_r < 2) pwm_r_incr = 2;
            if (pwm_g > 255-1) pwm_g_incr = -1;
            if (pwm_g < 1) pwm_g_incr = 1;
            if (pwm_b > 255-3) pwm_b_incr = -3;
            if (pwm_b < 3) pwm_b_incr = 3;

            pwm_set(0, pwm_r);
            pwm_set(1, pwm_g);
            pwm_set(2, pwm_b);
            pwm_r += pwm_r_incr;
            pwm_g += pwm_g_incr;
            pwm_b += pwm_b_incr;
            pwm_update();
            clk_timer_start(&pwm_demotimer, 5);
        }
    }
    return 0;
}

/** @} */
