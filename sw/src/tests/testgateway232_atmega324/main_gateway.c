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
#include "bus.h"
#include "bus_gateway.h"
#include "clock.h"
#include "cmddef_common.h"
#include "moddef_common.h"
#include "pcbconfig.h"
#include "register.h"
#include "serialcomm.h"


// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static sClkTimer_t g_LED_timer;

// --- Global variables --------------------------------------------------------
static sBus_t       g_sBus; //
static sSerPhy_t    g_sSerPhy;
// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------


static void IO_vInitialize(void)
{
    DDRA  |= ((0<<DDRA7) | (0<<DDRA6) | (0<<DDRA5) | (0<<DDRA4) | (0<<DDRA3) | (0<<DDRA2) | (0<<DDRA1) | (0<<DDRA0) );
    PORTA |= ((0<<PA7)   |  (0<<PA6)  |  (0<<PA5)  |  (0<<PA4)  |  (0<<PA3)  |  (0<<PA2)  |  (0<<PA1)  |  (0<<PA0)  );

    DDRB  |= ((1<<DDRB7) | (0<<DDRB6) | (1<<DDRB5) | (0<<DDRB4) | (0<<DDRB3) | (1<<DDRB2) | (0<<DDRB1) | (0<<DDRB0) );
    PORTB |= ((0<<PB7)   |  (0<<PB6)  |  (0<<PB5)  |  (0<<PB4)  |  (0<<PB3)  |  (0<<PB2)  |  (0<<PB1)  |  (0<<PB0)  );

    DDRC  |= ((0<<DDRC7) | (0<<DDRC6) | (0<<DDRC5) | (0<<DDRC4) | (0<<DDRC3) | (0<<DDRC2) | (0<<DDRC1) | (0<<DDRC0) );
    PORTC |= ((0<<PC7)   |  (0<<PC6)  |  (0<<PC5)  |  (0<<PC4)  |  (0<<PC3)  |  (0<<PC2)  |  (0<<PC1)  |  (0<<PC0)  );

    DDRD  |= ((0<<DDRD7) | (0<<DDRD6) | (0<<DDRD5) | (1<<DDRD4) | (0<<DDRD3) | (0<<DDRD2) | (0<<DDRD1) | (0<<DDRD0) );
    PORTD |= ((0<<PD7)   |  (0<<PD6)  |  (0<<PD5)  |  (0<<PD4)  |  (0<<PD3)  |  (0<<PD2)  |  (0<<PD1)  |  (0<<PD0)  );
}

void activate_wakeup_interrupt(void)
{

}

void deactivate_wakeup_interrupt(void)
{

}






static void interpret_message (uint16_t sender, uint8_t msglen, uint8_t* msg)
{

    switch (msg[0]) {
    case eCMD_STATE_BITFIELDS:
        if (msglen < 2) break;
        /*
        if (msg[2] == 0) {
            q_put_byte(&g_sSerPhy.sSendQ, 'O');
            q_put_byte(&g_sSerPhy.sSendQ, 'F');
            q_put_byte(&g_sSerPhy.sSendQ, 'F');
        }
        else {
            q_put_byte(&g_sSerPhy.sSendQ, 'O');
            q_put_byte(&g_sSerPhy.sSendQ, 'N');
        }
        serial_phy_initiate_sending();
        */
        break;
    case eCMD_SLEEP:
        activate_wakeup_interrupt();
        bus_sleep(&g_sBus);
        deactivate_wakeup_interrupt();
        break;
    default:
        break;
    }
}



static BOOL interpret_and_forward_message (sBus_t *psBus, sSerPhy_t *psSerial)
{
    uint8_t  len = 0;
    uint8_t  length = 0;
    uint16_t sender = 0;

    do {
        // is there a new message pending?
        if (psBus->msg_receive_state != eBUS_RECV_MESSAGE) {
            return FALSE;
        }
        sender   = psBus->sRecvMsg.uSender;
        length   = psBus->sRecvMsg.length - 4;
        q_put_byte(&psSerial->sSendQ, (uint8_t)((sender & 0xFF00)>>8));
        q_put_byte(&psSerial->sSendQ, (uint8_t)( sender & 0x00FF));
        q_put_byte(&psSerial->sSendQ, length);

        while (len < length) {
            q_put_byte(&g_sSerPhy.sSendQ, psBus->sRecvMsg.auBuf[5 + len]);
            len ++;
        }
        serial_phy_initiate_sending(&g_sSerPhy);
        // reset bus to IDLE state, so we are ready to receive the next message
        psBus->msg_receive_state = eBUS_RECV_NOTHING;
        psBus->sRecvMsg.length = 0;
        psBus->sRecvMsg.uOverallLength = 0;
        psBus->eState = eBus_Idle;

        if(eCMD_SLEEP == psBus->sRecvMsg.auBuf[5]) {
            //sleep_pinchange2_enable();
            bus_sleep(psBus);
            //sleep_pinchange2_disable();
        }

    } while ( FALSE );
    return TRUE;

}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int main(void)
{
    //uint8_t msglen = 0;
    //uint8_t msg[BUS_MAXRECVMSGLEN];
    //uint16_t sender = 0,
    uint16_t module_id = 10;

    IO_vInitialize();
    clk_initialize();
    scomm_initialize_uart1(&g_sSerPhy);

    //register_set_u8(MOD_eReg_ModuleID, 23);
    register_get(MOD_eReg_ModuleID, 0, &module_id);
    bus_configure(&g_sBus, module_id);
    bus_initialize(&g_sBus, 0);// initialize bus on UART 0

    sei();

    clk_timer_start(&g_LED_timer, 1000);
    LED_ERROR_OFF;


    while (1) {
        // check for message and read it
        if (bus_get_message(&g_sBus)) {
            interpret_and_forward_message(&g_sBus, &g_sSerPhy);
            /*
            if (bus_read_message(&g_sBus, &sender, &msglen, msg)) {
                interpret_message(sender, msglen, msg);
            }
            */
        }
        if (serial_phy_msg_received(&g_sSerPhy)) {
            if(busgateway_forward_serial_message(&g_sSerPhy.sRecvQ, &g_sBus)) {
            }
        }
    }
    return 0;
}

/** @} */
