/**
 * @addtogroup SPI
 * @brief Serial peripheral interface driver.
 *
 * Contains functions to send and receive data over the SPI interface. Two
 * versions exist: A blocking version which halts the application until the data
 * is transmitted and a non-blocking version, which does the transmission in
 * background. In this case optional start and end-callbacks are called at the
 * beginning and at the end of the transmission.
 *
 * Only use the non-blocking option if the CPU frequency is at least times 10
 * faster than the SPI frequency. Otherwise the control overhead takes more CPU
 * time than the transmission itself.
 *
 * Supports pcbconfig.h for individual PCB settings of the SPI pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    spi.h
 * @brief   Serial peripheral interface driver.
 *
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
#ifndef _SPI_H_
#define _SPI_H_

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "prjtypes.h"
#include "ucontroller.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection SPI_PCBCONFIG
 * Configure SPI Pinout.
 * @{
 */
#ifndef SPI_PCBCONFIG
 #define SPI_PCBCONFIG  1
 //! Defines the DDR of a port which contains the MOSI signal.
 #define SPI_DDR_MOSI   DDRB
 //! Defines the DDR of a port which contains the MISO signal.
 #define SPI_DDR_MISO   DDRB
 //! Defines the DDR of a port which contains the SCK signal.
 #define SPI_DDR_SCK    DDRB
 //! Defines the DDR of a port which contains the standard slave select signal.
 #define SPI_DDR_SS     DDRB
 //! Defines the PORT which contains the standard slave select signal.
 #define SPI_PORT_SS    PORTB
 //! Defines the MOSI pin.
 #define SPI_MOSI       PB3
 //! Defines the MISO pin.
 #define SPI_MISO       PB4
 //! Defines the SCK pin.
 #define SPI_SCK        PB5
 //! Defines the SS (slave select) pin.
 #define SPI_SS         PB2
#endif // SPI_PCBCONFIG

/** @} */

/**
 * @subsection SPI_APPCONFIG
 * Configure size of the SPI driver. E.g. only blocking functions.
 * @{
 */
#ifndef SPI_APPCONFIG
 #define SPI_APPCONFIG 1
 #define SPI_WITH_BLOCKING  1
 #undef  SPI_WITH_IRQ
 #define SPI_QUEUE_SIZE     32  //!< Size of the SPI queue. Per defined slave an
                                //!< overhead of 2 bytes is additionally used.
 #define SPI_NUM_SLAVES     2   //!< number of slaves connected to SPI. From 0
                                //!< up to 8 slaves can be configured.
#endif // SPI_APPCONFIG
/** @} */

#if defined (__AVR_ATtiny1634__)
#define SPI_DATA_REG                UDR1
#define SPI_TRANSMITION_COMPLETE    (UCSR1A & (1<<UDRE1))
#define SPI_RECEPTION_COMPLETE      (UCSR1A & (1<<RXC1 ))
#else // defined (__AVR_ATtiny1634__)
#define SPI_DATA_REG                REG_SPDR0
#define SPI_TRANSMITION_COMPLETE    ((REG_SPSR0) & (1 << REGBIT_SPIF0))
#define SPI_RECEPTION_COMPLETE      ((REG_SPSR0) & (1 << REGBIT_SPIF0))
#endif // defined (__AVR_ATtiny1634__)

// --- Type definitions --------------------------------------------------------

/**
 * Function pointer type for the routine which is called before the physical SPI
 * transmission is started.
 * It can be used e.g. to set the slave select line.
 * @param[in] arg
 * User specific byte which has been passed to the SPI_uTransmit() function.
 */
typedef void (*spi_start_send_func_t)(uint8_t arg);
/**
 * Function pointer type for the routine which is called after the physical SPI
 * transmission has ended.
 * It can be used e.g. to reset the slave select line.
 * @note This function is called from an interrupt context!
 * @param[in] arg
 * User specific byte which has been passed to the SPI_uTransmit() function.
 */
typedef void (*spi_end_send_func_t)(uint8_t arg);

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifdef SPI_WITH_BLOCKING

/**
 * Initialize the SPI as master.
 */
#if defined (__AVR_ATmega8__)     || \
    defined (__AVR_ATmega88__)    || \
    defined (__AVR_ATmega88A__)   || \
    defined (__AVR_ATmega88P__)   || \
    defined (__AVR_ATmega328__)   || \
    defined (__AVR_ATmega328P__)  || \
    defined (__AVR_ATmega328PB__) || \
    defined (__AVR_ATmega324P__)  || \
    defined (__AVR_ATmega324PA__) || \
    defined (__AVR_ATmega324A__)    

inline void     spi_master_init_blk  (void)
{
    // Set MOSI and SCK output, all others input
    SPI_DDR_MISO &= ~(1 << SPI_MISO);
    SPI_DDR_MOSI |=  (1 << SPI_MOSI);
    SPI_DDR_SCK  |=  (1 << SPI_SCK);
    SPI_DDR_SS   |=  (1 << SPI_SS);    // set DDR for slave select as output to guarantee SPI master mode
    SPI_PORT_SS  |=  (1 << SPI_SS);    // set slave select to 1 (slave disabled)
    REG_SPSR0     =  (1 << REGBIT_SPI2X0);
    // Enable SPI, Master, set clock rate fck/2
    REG_SPCR0     =  (1 << REGBIT_SPE0) | (1 << REGBIT_MSTR0);
}
#elif defined (__AVR_ATtiny1634__)

inline void     spi_master_init_blk  (void)
{
	SPI_DDR_MISO &= ~(1 << SPI_MISO);
    SPI_DDR_MOSI |=  (1 << SPI_MOSI);
    // Setting the XCKn port pin as output, enables master mode.
    SPI_DDR_SCK  |=  (1 << SPI_SCK);
    SPI_DDR_SS   |=  (1 << SPI_SS);    // set DDR for slave select as output to guarantee SPI master mode
    SPI_PORT_SS  |=  (1 << SPI_SS);    // set slave select to 1 (slave disabled)
}

#endif // defined (__AVR_ATtiny1634__)

/**
 * Simple blocking SPI transmission. The function returns when the byte has been
 * sent.
 *
 * @param[in] data
 * Byte to be sent over SPI0.
 * @returns Byte received through MISO line during transmission.
 */
#if defined (__AVR_ATmega8__)     || \
    defined (__AVR_ATmega88__)    || \
    defined (__AVR_ATmega88A__)   || \
    defined (__AVR_ATmega88P__)   || \
    defined (__AVR_ATmega328__)   || \
    defined (__AVR_ATmega328P__)  || \
    defined (__AVR_ATmega328PB__) || \
    defined (__AVR_ATmega324P__)  || \
    defined (__AVR_ATmega324PA__) || \
    defined (__AVR_ATmega324A__)

inline uint8_t  spi_transmit_blk    (uint8_t                data )
{
    // Start transmission
    SPI_DATA_REG = data;
    // Wait for transmission complete
    while ( !(SPI_TRANSMITION_COMPLETE) );
    // SPDR contains received byte during transmission.
    return SPI_DATA_REG;
}

#elif defined (__AVR_ATtiny1634__)

inline uint8_t  spi_transmit_blk    (uint8_t                data )
{
    register uint8_t clk = 0x11;
    register uint8_t clk_and_shift = 0x13;

    USIDR = data;
    USICR = clk;
    USICR = clk_and_shift;
    USICR = clk;
    USICR = clk_and_shift;
    USICR = clk;
    USICR = clk_and_shift;
    USICR = clk;
    USICR = clk_and_shift;
    USICR = clk;
    USICR = clk_and_shift;
    USICR = clk;
    USICR = clk_and_shift;
    USICR = clk;
    USICR = clk_and_shift;
    USICR = clk;
    USICR = clk_and_shift;

    return USIDR;
}
#endif // defined (__AVR_ATtiny1634__)
#endif // SPI_WITH_BLOCKING


#ifdef SPI_WITH_IRQ
void            spi_master_init     (void);

uint8_t         spi_transmit        (uint8_t*               send_buf,
                                     uint8_t*               receive_buf,
                                     uint8_t                length,
                                     spi_start_send_func_t  before_transmission_func,
                                     spi_end_send_func_t    after_transmission_func,
                                     uint8_t                user_arg);

uint8_t         spi_queue_space     (void);

bool            spi_is_busy         (void);

#endif // SPI_WITH_IRQ

#endif // _SPI_H_
/** @} */
