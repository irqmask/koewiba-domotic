/**
 * @addtogroup SN74595
 * @brief This module contains functions to control a 74595 latch.
 *
 * @{
 * @file    sn74595.c
 * @brief   This module contains functions to control a 74595 latch.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
 *
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

#include "spi.h"

#include "sn74595.h"

// --- Definitions -------------------------------------------------------------

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

#ifndef SN74595_PCBCONFIG
 #define SN74595_PCBCONFIG   1

 #define LATCH_OE_DDR     	DDRD
 #define LATCH_OE_PORT      PORTD
 #define LATCH_OE_PIN       PD6
 #define LATCH_STB_DDR    	DDRD
 #define LATCH_STB_PORT     PORTD
 #define LATCH_STB_PIN      PD5
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------
static uint8_t sn74595_schadow_reg[SN74595_REG_BYTES];

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------


/**
 * Initialize sn74595 port pins.
 */
void sn74595_initialize    	(void)
{
    uint8_t i;
#ifdef LATCH_OE_ENABLED
    LATCH_OE_DDR |= (1<<LATCH_OE_PIN);
    sn74595_OE_off();
#endif
    LATCH_STB_DDR |= (1<<LATCH_STB_PIN);
    LATCH_STB_PORT &= ~(1<<LATCH_STB_PIN);
    for(i=0; i<SN74595_REG_BYTES; i++)
    {
        sn74595_schadow_reg[i] = 0;
        spi_transmit_blk(sn74595_schadow_reg[i]);
    }
    sn74595_latch();
}

/**
 * Latch current shift register data. Makes data appear on output lines.
 */
void sn74595_latch(void)
{
    LATCH_STB_PORT |=  (1<<LATCH_STB_PIN);
    LATCH_STB_PORT &= ~(1<<LATCH_STB_PIN);
}

#ifdef LATCH_OE_ENABLED
/**
 * Output enable. If output enabled, data is valid at output lines.
 */
void sn74595_OE_on(void)
{
    LATCH_OE_PORT &= ~(1<<LATCH_OE_PIN);
}

/**
 * Output disable. If output disabled, output lines are switch to tri-state.
 */
void sn74595_OE_off(void)
{
    LATCH_OE_PORT |= (1<<LATCH_OE_PIN);
}
#endif

/**
 * Set bits in the spi message.
 *
 * @param[in] byte_idx	Index of byte in message.
 * @param[in] data  	Databyte to send.
 * @param[in] mask  	Bit mask (which bits should be updated).
 */
void sn74595_set_byte(uint8_t byte_idx, uint8_t data, uint8_t mask)
{
    uint8_t byte = sn74595_schadow_reg[byte_idx] & ~mask;

    byte |= data & mask;
    sn74595_schadow_reg[byte_idx] = byte;
}

/**
 * Send and latch all data bytes.
 *
 */
void sn74595_send(void)
{
    uint8_t	i;

    for(i=0; i<SN74595_REG_BYTES; i++)
    {
        spi_transmit_blk(sn74595_schadow_reg[i]);
    }
    sn74595_latch();
}

/** @} */
