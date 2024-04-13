/**
 * @addtogroup BLINDCONTROL_5_ATMEGA328
 * @brief This module controls additional outputs on blind-control_5 PCB.
 *
 * @{
 * @file    digital_output.c
 * @brief   This module contains functions to control digital outputs.
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// --- Include section ---------------------------------------------------------

#include "digital_output.h"
#include "sn74595.h"
#include "bus.h"

// --- Type definitions --------------------------------------------------------

typedef struct {
    uint8_t value;
    uint8_t threshold_on;
    uint8_t threshold_off;
} output_data_t;

// --- Local variables ---------------------------------------------------------
//! Bitfield for changed output states since last message transmission
static uint8_t g_output_changed_index;
//! Shadow register of shift register data
static uint8_t g_output_shadow;
//! Mask of port bits of shift registers where on/off relays are connected to
static const uint8_t g_output_mask[OUTPUT_COUNT] = {
    1<<OUTPUT_0,
    1<<OUTPUT_1,
    1<<OUTPUT_2,
    1<<OUTPUT_3,
    1<<OUTPUT_4,
    1<<OUTPUT_5,
};

output_data_t g_output[OUTPUT_COUNT];


// --- Module global functions -------------------------------------------------


/**
 * Initialize motor relays.
 */
void digital_output_initialize (void)
{
    uint8_t i;
    uint8_t mask = 0;

    g_output_shadow = 0;
    g_output_changed_index = 0;
    sn74595_initialize();
    for(i=0; i<OUTPUT_COUNT; i++)
    {
        mask |= g_output_mask[i];
        g_output[i].value = 0;
    }
    sn74595_set_byte(0, g_output_shadow, mask);
    sn74595_send();
    sn74595_OE_on();
}

void digital_output_set(uint8_t index, uint8_t value)
{
  if (index >= OUTPUT_COUNT) return;

    if (value > g_output[index].threshold_on) {
        g_output_shadow |= g_output_mask[index];
        g_output[index].value = 0xFF;
    }
    else if (value < g_output[index].threshold_off) {
        g_output_shadow &= ~g_output_mask[index];
        g_output[index].value = 0x00;
    }
    sn74595_set_byte(0, g_output_shadow, g_output_mask[index]);
    sn74595_send();
    g_output_changed_index |= (1<<index);
}

uint8_t digital_output_get_current_state(uint8_t chn)
{
    if (chn >= OUTPUT_COUNT) {
        return 0;
    }
    return g_output[chn].value;
}

void digital_output_set_threshold_off(uint8_t chn, uint8_t throff)
{
    if (chn >= OUTPUT_COUNT) {
        return;
    }
    g_output[chn].threshold_off = throff;
}

void digital_output_set_threshold_on(uint8_t chn, uint8_t thron)
{
    if (chn >= OUTPUT_COUNT) {
        return;
    }
    g_output[chn].threshold_on = thron;
}

void digital_output_background(sBus_t* bus)
{
    uint8_t index;

    if(0 == g_output_changed_index) return;

    for(index=0; index<OUTPUT_COUNT; index++) {
        if(g_output_changed_index & (1<<index)) {
            uint8_t msg[3];

            msg[0] = eCMD_STATE_8BIT;
            msg[1] = APP_eReg_Chn0_SwitchCurrent + (index * APP_NUM_REGS_PER_CHN);
            msg[2] = g_output[index].value;
            bus_send_message(bus, BUS_BRDCSTADR, 3, msg);
        }
    }
    g_output_changed_index = 0;
}
