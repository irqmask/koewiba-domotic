/**
 * @addtogroup SN74595
 * @brief This module contains functions to control a 74595 latch.
 *
 * @{
 * @file    sn74595.c
 * @brief   This module contains functions to control a 74595 latch.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

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

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------


/**
 * Initialize sn74595 port pins.
 */
void sn74595_initialize    	(void)
{
    LATCH_OE_DDR |= (1<<LATCH_OE_PIN);
    LATCH_STB_DDR |= (1<<LATCH_STB_PIN);
    sn74595_OE_off();
    LATCH_STB_PORT &= ~(1<<LATCH_STB_PIN);
	spi_transmit_blk(0);
	sn74595_latch();
}

/**
 * Latch current shift register data. Makes data appear on output lines.
 */
void sn74595_latch(void)
{
    LATCH_STB_PORT |= (1<<LATCH_STB_PIN);
    LATCH_STB_PORT &= ~(1<<LATCH_STB_PIN);
}

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

/**
 * Send and latch one data byte.
 *
 * @param[in] data  Databyte to send.
 */
void sn74595_send(uint8_t data)
{
    spi_transmit_blk(data);
    sn74595_latch();
}

/** @} */
